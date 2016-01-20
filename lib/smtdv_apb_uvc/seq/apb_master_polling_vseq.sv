
`ifndef __APB_MASTER_POLLING_VSEQ_SV__
`define __APB_MASTER_POLLING_VSEQ_SV__

class apb_master_polling_vseq#(
  ADDR_WIDTH = 14,
  DATA_WIDTH = 32
  ) extends
    apb_master_base_vseq#(
      .ADDR_WIDTH(ADDR_WIDTH),
      .DATA_WIDTH(DATA_WIDTH)
  );

  typedef apb_master_polling_vseq#(ADDR_WIDTH, DATA_WIDTH) vseq_t;
  typedef apb_master_cfg_seq#(ADDR_WIDTH, DATA_WIDTH) seq_cfg_t;
  typedef apb_master_1r_seq#(ADDR_WIDTH, DATA_WIDTH) seq_1r_t;
  typedef apb_master_polling_seq#(ADDR_WIDTH, DATA_WIDTH) seq_pol_t;

  seq_cfg_t seq_cfg;
  seq_1r_t  seq_rd;
  seq_pol_t seq_pol;

  static const bit [ADDR_WIDTH-1:0] start_addr = `APB_START_ADDR(0)
  static const bit [ADDR_WIDTH-1:0] end_addr = `APB_END_ADDR(0)
  static const bit [DATA_WIDTH-1:0] pats[$] = '{ 'h0, 'h4, 'h8, 'hc };
  bit [ADDR_WIDTH-1:0] cur_addr;

  `uvm_object_param_utils_begin(vseq_t)
  `uvm_object_utils_end

  function new(string name = "apb_master_polling_seq");
    super.new(name);
  endfunction : new

  // polling until expected value assert
  virtual task do_polling_cfg_seq();
    seq_pol.start(seqr, this, -1);
  endtask : do_polling_cfg_seq

  virtual task do_cfg_rd_seq();
    seq_pol.set_polling_start();
    assert(seq_pol.is_ready_to_polling());

    `uvm_create_on(seq_rd, seqr)
    `SMTDV_RAND_WITH(seq_rd,
    {
      seq_rd.start_addr == cur_addr;
    })
    seq_rd.start(seqr, this, 0);

    wait(seq_pol.is_finish_to_polling());
  endtask : do_cfg_rd_seq

  virtual task do_cfg_wr_seq();
    cur_addr = start_addr;

    foreach(pats[i]) begin
      `uvm_create_on(seq_cfg, seqr)
      `SMTDV_RAND_WITH(seq_cfg,
        {
          seq_cfg.start_addr == cur_addr;
          seq_cfg.write_data == pats[i];
        })
      seq_cfg.start(seqr, this, 0);
    end
  endtask : do_cfg_wr_seq

  virtual task pre_body();
    super.pre_body();
    `uvm_create_on(seq_pol, seqr)
    `SMTDV_RAND(seq_pol)
    seq_pol.register_watch_table(start_addr, end_addr);
    seq_pol.register_polling_table(start_addr, pats[$]);
  endtask : pre_body

  virtual task body();
    super.body();

    fork
      do_polling_cfg_seq();
    join_none

    fork
      do_cfg_wr_seq();
      do_cfg_rd_seq();
    join

    seqr.finish = TRUE;
  endtask : body

endclass : apb_master_polling_vseq


`endif // __APB_MASTER_PRELOAD_VSEQ_SV__


