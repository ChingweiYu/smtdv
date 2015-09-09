//----------------------------------------------------------------------
//   Copyright 2012 Cadence Design Systems, Inc.
//   All Rights Reserved Worldwide
//
//   Licensed under the Apache License, Version 2.0 (the
//   "License"); you may not use this file except in
//   compliance with the License.  You may obtain a copy of
//   the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in
//   writing, software distributed under the License is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//   CONDITIONS OF ANY KIND, either express or implied.  See
//   the License for the specific language governing
//   permissions and limitations under the License.
//----------------------------------------------------------------------

// Producer class sending TLM2 transactions
class producer extends uvm_component;
  uvm_tlm_b_initiator_socket #() b_initiator_socket;
  uvm_tlm_nb_initiator_socket #(producer) nb_initiator_socket;
  int address;

  function new(string name, uvm_component parent=null);
      super.new(name,parent);
      `uvm_info(get_type_name(),"SV producer::new",UVM_LOW);
  endfunction

  typedef producer prod_type;
  `uvm_component_utils_begin(prod_type)
    `uvm_field_int(address, UVM_DEFAULT)
  `uvm_component_utils_end
  
  function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      `uvm_info(get_type_name(),"SV producer::build",UVM_LOW);
      nb_initiator_socket = new("nb_initiator_socket", this);
      b_initiator_socket  = new("b_initiator_socket", this);
  endfunction

  function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      `uvm_info(get_type_name(),"SV producer::connect",UVM_LOW);
  endfunction

  function uvm_tlm_sync_e nb_transport_bw(uvm_tlm_generic_payload t,
                                      ref uvm_tlm_phase_e p,
                                      input uvm_tlm_time delay);
     uvm_report_warning("producer", "nb_transport_bw is not implemented");
  endfunction // nb_transport_bw

  task run_phase(uvm_phase phase);
      uvm_tlm_generic_payload t;
      int addr;
      uvm_tlm_time delay = new();
      delay.incr(2ns, 1ns);
      `uvm_info(get_type_name(),"SV producer::run",UVM_LOW);
      
      phase.raise_objection(this);

      addr = address;
      #10ns;
      
      // Send a blocking WRITE transaction
      t = generate_transaction(addr, UVM_TLM_WRITE_COMMAND);
      `uvm_info(get_type_name(),$sformatf("SV producer::b_transport %s", t.convert2string()),UVM_LOW);
      b_initiator_socket.b_transport(t, delay);
      `uvm_info(get_type_name(),$sformatf("SV producer::b_transport done %s", t.convert2string()),UVM_LOW);
      #8ns;
      
      // Send a blocking READ transaction
      t = generate_transaction(addr, UVM_TLM_READ_COMMAND);
      `uvm_info(get_type_name(),$sformatf("SV producer::b_transport %s", t.convert2string()),UVM_LOW);
      b_initiator_socket.b_transport(t, delay);
      `uvm_info(get_type_name(),$sformatf("SV producer::b_transport done %s", t.convert2string()),UVM_LOW);
      phase.drop_objection(this);
  endtask // run_phase

  function uvm_tlm_generic_payload generate_transaction(bit[63:0] addr, 
							uvm_tlm_command_e cmd);
      int unsigned length;
      byte unsigned data[];
      
      uvm_tlm_generic_payload t = new();
      length = 4;
      data = new[length];
      
      t.set_data_length(length);
      t.set_address(addr);
      
      for(int unsigned i = 0; i < length; i++) begin
	 if(cmd == UVM_TLM_WRITE_COMMAND) begin
	    data[i] = $urandom();
	 end
	 if(cmd == UVM_TLM_READ_COMMAND) begin
	    data[i] = 0;  
	 end
      end
      
      t.set_data(data);
      t.set_byte_enable_length(0);
      t.set_response_status(UVM_TLM_INCOMPLETE_RESPONSE);
      t.set_command(cmd);
      
      return t;
  endfunction

endclass

