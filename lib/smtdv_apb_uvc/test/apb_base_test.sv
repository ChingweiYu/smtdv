
`ifndef __APB_TEST_SV__
`define __APB_TEST_SV__

// 1Mx2S cluster
class apb_base_test extends smtdv_test;

  parameter ADDR_WIDTH = `APB_ADDR_WIDTH;
  parameter DATA_WIDTH = `APB_DATA_WIDTH;
  parameter NUM_OF_INITOR = 1;
  parameter NUM_OF_TARGETS = 2;

  `APB_RST_VIF apb_rst_vif;

  `APB_SLAVE_AGENT slave_agent[$];
  `APB_SLAVE_CFG slave_cfg[$];

  `APB_MASTER_AGENT master_agent[$];
  `APB_MASTER_CFG master_cfg[$];

  `APB_BASE_SCOREBOARD master_scb[$];

  smtdv_reset_model #(`APB_RST_VIF) apb_rst_model;

  `uvm_component_utils(`APB_BASE_TEST)

  function new(string name = "apb_base_test", uvm_component parent=null);
    super.new(name, parent);
  endfunction

  virtual function void build_phase(uvm_phase phase);
    bit [ADDR_WIDTH-1:0] start_addr, end_addr;
    super.build_phase(phase);

    //create sqlite3 db
    smtdv_sqlite3::delete_db("apb_db.db");
    smtdv_sqlite3::new_db("apb_db.db");

    // slave0 cfg, agent
    slave_cfg[0] = `APB_SLAVE_CFG::type_id::create({$psprintf("slave_cfg[%0d]", 0)}, this);
    `SMTDV_RAND_WITH(slave_cfg[0], {
      has_force == 1;       // force orignal vif behavior
      has_coverage == 1;
      has_export == 1;
    })
    slave_agent[0] = `APB_SLAVE_AGENT::type_id::create({$psprintf("slave_agent[%0d]", 0)}, this);
    uvm_config_db#(uvm_bitstream_t)::set(null, "/.+slave_agent[*0]*/", "is_active", UVM_ACTIVE);
    uvm_config_db#(`APB_SLAVE_CFG)::set(null, "/.+slave_agent[*0]*/", "cfg", slave_cfg[0]);

    // slave1 cfg, agent
    slave_cfg[1] = `APB_SLAVE_CFG::type_id::create({$psprintf("slave_cfg[%0d]", 1)}, this);
    `SMTDV_RAND_WITH(slave_cfg[1], {
      has_force == 1;
      has_coverage == 1;
      has_export == 1;
    })
    slave_agent[1] = `APB_SLAVE_AGENT::type_id::create({$psprintf("slave_agent[%0d]", 1)}, this);
    uvm_config_db#(uvm_bitstream_t)::set(null, "/.+slave_agent[*1]*/", "is_active", UVM_ACTIVE);
    uvm_config_db#(`APB_SLAVE_CFG)::set(null, "/.+slave_agent[*1]*/", "cfg", slave_cfg[1]);

    // master cfg, agent
    master_cfg[0] = `APB_MASTER_CFG::type_id::create({$psprintf("master_cfg[%0d]", 0)}, this);
    `SMTDV_RAND_WITH(master_cfg[0], {
      has_force == 1;
      has_coverage == 1;
      has_export == 1;
    })
    start_addr = `APB_START_ADDR(0)
    end_addr = `APB_END_ADDR(0)
    master_cfg[0].add_slave(slave_cfg[0], 0, start_addr, end_addr);
    start_addr = `APB_START_ADDR(1)
    end_addr = `APB_END_ADDR(1)
    master_cfg[0].add_slave(slave_cfg[1], 1, start_addr, end_addr);

    master_agent[0] = `APB_MASTER_AGENT::type_id::create({$psprintf("master_agent[%0d]", 0)}, this);
    uvm_config_db#(uvm_bitstream_t)::set(null, "/.+master_agent[*0]*/", "is_active", UVM_ACTIVE);
    uvm_config_db#(`APB_MASTER_CFG)::set(null, "/.+master_agent[*0]*/", "cfg", master_cfg[0]);

    // scoreboard num of masters cross all slaves ex: 3*all, 2*all socreboard
    master_scb[0] = `APB_BASE_SCOREBOARD::type_id::create({$psprintf("master_scb[%0d]", 0)}, this);
    uvm_config_db#(`APB_MASTER_AGENT)::set(null, "/.+master_scb[*0]*/", "initor_m[0]", master_agent[0]);
    uvm_config_db#(`APB_SLAVE_AGENT)::set(null, "/.+master_scb[*0]*/", "targets_s[0]", slave_agent[0]);
    uvm_config_db#(`APB_SLAVE_AGENT)::set(null, "/.+master_scb[*0]*/", "targets_s[1]", slave_agent[1]);

    // resetn
    apb_rst_model = smtdv_reset_model#(`APB_RST_VIF)::type_id::create("apb_rst_model");
    if(!uvm_config_db#(`APB_RST_VIF)::get(this, "", "apb_rst_vif", apb_rst_vif))
      `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".apb_rst_vif"});
    apb_rst_model.create_rst_monitor(apb_rst_vif);

  endfunction

  virtual function void connect_phase(uvm_phase phase);
    super.connect_phase(phase);
    master_agent[0].mon.item_collected_port.connect(master_scb[0].initor[0]);
    slave_agent[0].mon.item_collected_port.connect(master_scb[0].targets[0]);
    slave_agent[1].mon.item_collected_port.connect(master_scb[0].targets[1]);
  endfunction

  virtual function void end_of_elaboration_phase(uvm_phase phase);
    super.end_of_elaboration_phase(phase);
    apb_rst_model.add_component(this);
    apb_rst_model.set_rst_type(ALL_RST);
    apb_rst_model.show_components(0);
  endfunction


endclass

`endif // __APB_TEST_SV__
