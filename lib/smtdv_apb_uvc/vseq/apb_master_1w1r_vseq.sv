
`ifndef __APB_MASTER_1W1R_VSEQ_SV__
`define __APB_MASTER_1W1R_VSEQ_SV__

/*
* sequence graph test
*/
class apb_master_1w1r_vseq
  extends
  apb_master_base_vseq;

  parameter ADDR_WIDTH = 32;
  parameter DATA_WIDTH = 32;

  typedef apb_master_1w1r_vseq vseq_t;
  typedef apb_virtual_sequencer vseqr_t;
  typedef apb_master_1w_seq#(ADDR_WIDTH, DATA_WIDTH) seq_1w_t;
  typedef apb_master_1r_seq#(ADDR_WIDTH, DATA_WIDTH) seq_1r_t;
  typedef apb_master_stop_seqr_seq#(ADDR_WIDTH, DATA_WIDTH) seq_stop_t;
  typedef uvm_component bcmp_t;
  typedef uvm_object obj_t;

  bcmp_t bseqr;
  obj_t bseq;
  vseqr_t vseqr;

  seq_1w_t seq_1w;
  seq_1r_t seq_1r;
  seq_stop_t seq_stop;

  static const bit [ADDR_WIDTH-1:0] start_addr = `APB_START_ADDR(0)
  static const bit [ADDR_WIDTH-1:0] incr_addr = 'h100;
  bit [ADDR_WIDTH-1:0] cur_addr;

  `uvm_object_param_utils_begin(vseq_t)
  `uvm_object_utils_end

  `uvm_declare_p_sequencer(vseqr_t)

  function new(string name = "apb_master_1w1r_seq");
    super.new(name);
  endfunction : new

  virtual task pre_body();
    cur_addr = start_addr;
    super.pre_body();

    if (!$cast(vseqr, this.get_sequencer()))
      `uvm_error("SMTDV_UCAST_V/PSEQR",
         {$psprintf("UP CAST TO SMTDV V/PSEQR FAIL")})

    `uvm_create_on(seq_1w, vseqr.apb_magts[0].seqr)
    `SMTDV_RAND_WITH(seq_1w,
      {
        start_addr == cur_addr;
      })

    `uvm_create_on(seq_1r, vseqr.apb_magts[0].seqr)
    `SMTDV_RAND_WITH(seq_1r,
      {
        start_addr == cur_addr;
      })

    `uvm_create_on(seq_stop, vseqr.apb_magts[0].seqr)

    graph = '{
        nodes:
           '{
               '{
                   uuid: 0,
                   seq: seq_1w,
                   seqr: vseqr.apb_magts[0].seqr,
                   prio: -1,
                   desc: {$psprintf("bind Node[%0d] as %s", 0, "seq_1w")}
               },
               '{
                   uuid: 1,
                   seq: seq_1r,
                   seqr: vseqr.apb_magts[0].seqr,
                   prio: -1,
                   desc: {$psprintf("bind Node[%0d] as %s", 1, "seq_1r")}
               },
               '{
                   uuid: 2,
                   seq: seq_stop,
                   seqr: vseqr.apb_magts[0].seqr,
                   prio: -1,
                   desc: {$psprintf("bind Node[%0d] as %s", 2, "seq_stop")}
                }
           },
        edges:
           '{
               '{
                   uuid: 0,
                   sourceid: 0,
                   sinkid: 1,
                   desc: {$psprintf("bind Edge[%0d] from Node[%0d] to Node[%0d]", 0, 0, 1)}
               }
           }
     };
  endtask : pre_body

  // read after write
  virtual task body();
    super.body();
    fork
      begin seq_1w.start(vseqr.apb_magts[0].seqr, this, 0); end
      begin seq_1r.start(vseqr.apb_magts[0].seqr, this, 0); end
      begin seq_stop.start(vseqr.apb_magts[0].seqr, this, 0); end
    join_none
    #10;
  endtask : body

  virtual task post_body();
    super.post_body();
    wait(vseqr.apb_magts[0].seqr.finish);
  endtask : post_body

endclass : apb_master_1w1r_vseq

`endif // __APB_MASTER_1W1R_VSEQ_SV__


