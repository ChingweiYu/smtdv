
`ifndef __APB_ITEM_SV__
`define __APB_ITEM_SV__

class apb_item #(
  ADDR_WIDTH = 14,
  DATA_WIDTH = 32
) extends
    smtdv_sequence_item;

  rand bit [ADDR_WIDTH-1:0] addr;
  rand bit [ADDR_WIDTH-1:0] addrs[$];
  rand bit [(DATA_WIDTH>>3)-1:0][7:0] data_beat[$];
  int                      offset = DATA_WIDTH>>3;

  rand bit [15:0]           sel;
  rand trx_rsp_t	          rsp;

  rand int psel_L2H;
  rand int penable_L2H;
  rand int pready_L2H;

  constraint c_rsp_value { rsp inside {[OK:ERR]}; }
  constraint c_data_size { data_beat.size() == 1; }

  // used only two slaves
  constraint c_sel_size { sel inside {[0:1]}; }

  constraint c_psel_L2H { psel_L2H inside {[0:10]}; }
  constraint c_penable_L2H { penable_L2H inside {[0:10]}; }
  constraint c_pready_L2H { pready_L2H inside {[0:10]}; }

  `uvm_object_param_utils_begin(`APB_ITEM)
    `uvm_field_int(addr, UVM_DEFAULT)
    `uvm_field_queue_int(addrs, UVM_DEFAULT)
    `uvm_field_queue_int(data_beat, UVM_DEFAULT)
    `uvm_field_enum(trx_rsp_t, rsp, UVM_ALL_ON)
    `uvm_field_int(offset, UVM_ALL_ON)
    `ifdef APB_DEBUG
      `uvm_field_int(psel_L2H, UVM_DEFAULT)
      `uvm_field_int(penable_L2H, UVM_DEFAULT)
      `uvm_field_int(pready_L2H, UVM_DEFAULT)
    `endif
  `uvm_object_utils_end

  function new (string name = "apb_item");
    super.new(name);
  endfunction

  function void pack_data(bit [DATA_WIDTH-1:0] idata);
    int n = (DATA_WIDTH % 8 == 0)? DATA_WIDTH>>3 : DATA_WIDTH>>3+1;
    for (int i=0; i<n; i+=1) begin
      data_beat[0][i] = idata[i*8+:8];
    end
  endfunction

  function bit[DATA_WIDTH-1:0] unpack_data();
    bit [DATA_WIDTH-1:0] odata;
    int n = (DATA_WIDTH % 8 == 0)? DATA_WIDTH>>3 : DATA_WIDTH>>3+1;
    for (int i=0; i<n; i+=1) begin
      odata[i*8+:8] = data_beat[0][i];
    end
    return odata;
  endfunction

//  extern function void pre_randomize();
//  extern function void post_randomize();
endclass

`endif // end of __APB_ITEM_SV__

