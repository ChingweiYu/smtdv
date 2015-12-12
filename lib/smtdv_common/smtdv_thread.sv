`ifndef __SMTDV_THREAD_SV__
`define __SMTDV_THREAD_SV__

// *********************************************************************
class smtdv_run_thread extends uvm_object;

  smtdv_component cmp;
  bit [0:0] has_finalize = 0;
  // status run, idle, dead...

  `uvm_object_param_utils_begin(smtdv_run_thread)
    `uvm_field_int(has_finalize, UVM_DEFAULT)
  `uvm_object_utils_end

  function new(string name = "smtdv_run_thread", smtdv_component parent=null);
    super.new(name);
    cmp = parent;
  endfunction

  virtual function void pre_do();
  endfunction

  virtual function void post_do();
  endfunction

  virtual task run();
    pre_do();
    `uvm_info(get_full_name(), $sformatf("Starting run thread ..."), UVM_HIGH)
    post_do();
  endtask

endclass

`endif // end of __SMTDV_THREAD_SV__
