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
import uvm_pkg::*;
`include "uvm_macros.svh"
import uvm_ml::*;

//----------------------------------------------------------------------
// producer
//----------------------------------------------------------------------
class producer extends uvm_component;

  uvm_tlm_nb_initiator_socket #(producer) initiator_socket;

  bit done;

  function new(string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  function void build();
    initiator_socket = new("initator_socket", this);
  endfunction

  function uvm_tlm_sync_e nb_transport_bw(uvm_tlm_generic_payload t,
                                      ref uvm_tlm_phase_e p,
                                      input uvm_tlm_time delay);
    uvm_report_warning("producer", "nb_transport_bw is not implemented");
  endfunction

  task run_phase(uvm_phase phase);

    int unsigned i;
    uvm_tlm_time delay = new;
    uvm_tlm_phase_e ph;
    uvm_tlm_sync_e sync;
    uvm_tlm_generic_payload t;
     realtime ctime;
    phase.raise_objection(this);

    delay.incr(1, 1ns);

    // not really nb - consumer should accept the trans nb fashion, but consume time executing it
    for(i = 0; i < 10; i++) begin
      t = generate_transaction(i);
      uvm_report_info("producer", t.convert2string());
       ctime = $realtime;
      sync = initiator_socket.nb_transport_fw(t, ph, delay);
       assert($realtime==ctime);
       assert(sync == UVM_TLM_ACCEPTED);
    end

    phase.drop_objection(this);
  endtask

  //--------------------------------------------------------------------
  // generate_transaction
  //
  // generat a new, non randomized transaction
  //--------------------------------------------------------------------
  function uvm_tlm_generic_payload generate_transaction(int base);

    bit [63:0] addr;
    int unsigned length;
    byte unsigned data[];
     
     uvm_tlm_generic_payload t = new();
      addr = base;
    length = base;
    data = new[length];

    t.set_data_length(length);
    t.set_address(addr);

    for(int unsigned i = 0; i < length; i++) begin
      data[i] = base+i;
    end
    
    t.set_data(data);
    t.set_command(UVM_TLM_WRITE_COMMAND);
     
    return t;
  endfunction

endclass

//----------------------------------------------------------------------
// consumer
//----------------------------------------------------------------------
class consumer extends uvm_component;

  uvm_tlm_nb_target_socket #(consumer) target_socket;

  int unsigned transaction_count;

  function new(string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  `uvm_component_utils(consumer)

  function void build();
    target_socket = new("target_socket", this);
  endfunction

  function void connect();
  endfunction

  function uvm_tlm_sync_e nb_transport_fw(uvm_tlm_generic_payload t,
                                          ref uvm_tlm_phase_e p,
                                          input uvm_tlm_time delay);
     uvm_tlm_generic_payload expected;
    uvm_report_info("consumer", t.convert2string());
      expected =  generate_transaction(transaction_count);
     check_data(expected,t);
    transaction_count++;
    return UVM_TLM_ACCEPTED;
  endfunction

   function uvm_tlm_generic_payload generate_transaction(int base);

    bit [63:0] addr;
    int unsigned length;
    byte unsigned data[];
     
     uvm_tlm_generic_payload t = new();
      addr = base;
    length = base;
    data = new[length];

    t.set_data_length(length);
    t.set_address(addr);

    for(int unsigned i = 0; i < length; i++) begin
      data[i] = base+i;
    end
    
    t.set_data(data);
    t.set_command(UVM_TLM_WRITE_COMMAND);
     
    return t;
   endfunction // generate_transaction
   
   function void check_data(uvm_tlm_generic_payload expected,  uvm_tlm_generic_payload actual);
      
      byte unsigned d[];
      byte unsigned ed[];
      
      if(expected.get_address() != actual.get_address()) begin
	 $display("expected address ",expected.get_address()," actual address is ",actual.get_address());
	 assert(0);
      end
      
      if(expected.get_command() != actual.get_command()) begin
	 $display("expected command ",expected.get_command()," actual command is ",actual.get_command());
	 assert(0);
      end

       if(expected.get_data_length() != actual.get_data_length()) begin
	 $display("expected data_length ",expected.get_data_length()," actual data_length is ",actual.get_data_length());
	 assert(0);
       end
      
      expected.get_data(ed);
      actual.get_data(d);
      if(ed != d) begin
	 $display("expected data[0] ",ed[0]," actual data[0] is ",d[0]);
	 assert(0);
      end
      
   endfunction // check_that
   
  function void report();
    if(transaction_count == 10)
      $display("** UVM TEST PASSED **");
    else
      $display("** UVM TEST FAILED **"); 
  endfunction

endclass

//----------------------------------------------------------------------
// env
//----------------------------------------------------------------------
class env extends uvm_env;

  `uvm_component_utils(env)

  producer p;
  consumer c;

  function new(string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  function void build();
    p = new("producer", this);
    c = new("consumer", this);
  endfunction

  function void phase_ended(uvm_phase phase);
    if (phase.get_name() == "build") begin
      ml_tlm2 #()::register(c.target_socket);
      ml_tlm2 #()::register(p.initiator_socket);
    end
  endfunction // void

  // Bind the initiator socket to the target socket
  function void connect();
    bit result = uvm_ml::connect(p.initiator_socket.get_full_name(), c.target_socket.get_full_name());
  endfunction

endclass

//----------------------------------------------------------------------
// test
//----------------------------------------------------------------------
class test extends uvm_test;

  `uvm_component_utils(test)

  env e;

  function new(string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  function void build();
    e = new("env", this);
  endfunction

  task run_phase(uvm_phase phase);
    phase.raise_objection(this);

    #1000 phase.drop_objection(this);
  endtask
endclass

//----------------------------------------------------------------------
// top
//----------------------------------------------------------------------
module top;
`ifdef USE_UVM_ML_RUN_TEST
initial begin
   string tops[1];

   tops[0] = "";

   
   uvm_ml_run_test(tops, "SV:test");
end
`endif
endmodule
