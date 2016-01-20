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

// Test demonstrates hierarchical construction
// SV is the top component and it creates an e subtree from its env

import uvm_pkg::*;
`include "uvm_macros.svh"
 import uvm_ml::*;

  class packet extends uvm_transaction;
    int    data;
    `uvm_object_utils_begin(packet)
      `uvm_field_int(data, UVM_ALL_ON)
    `uvm_object_utils_end

    function void next();
      static int d = 101;
      data = d++;
    endfunction

    function int get_data();
      return data;
    endfunction

    function new(string name = "packet");
      super.new(name);
    endfunction

  endclass
    
  // Producer class sends 5 packets (101-105) through the analysis port
  class producer #(type T=packet) extends uvm_component;
    uvm_analysis_port #(T) aport;
  
    function new(string name, uvm_component parent=null);
      super.new(name,parent);
      $display("SV producer::new");
      aport=new("aport",this);
      $display("SV producer.aport.get_full_name() = ", aport.get_full_name());
    endfunction
  
    typedef producer#(T) prod_type;
    `uvm_component_utils_begin(prod_type)
    `uvm_component_utils_end
  
    function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      $display("SV producer::connect %s", get_full_name());
    endfunction

    function void resolve_bindings();
      $display("SV producer::resolve_bindings %s", get_full_name());
    endfunction
   
    function void end_of_elaboration();
      $display("SV producer::end_of_elaboration %s", get_full_name());
    endfunction
   
    function void start_of_simulation();
      $display("SV producer::start_of_simulation %s", get_full_name());
    endfunction
   
    task run_phase (uvm_phase phase);
      T p;
      phase.raise_objection(this);
      for (integer i = 0; i < 5; i++) begin
         p = new();
         p.next();
         $display("[SV ",$realtime, " ns] producer::run, writing ", p.get_data());
         aport.write(p);
         #10;
      end
      #50;
      phase.drop_objection(this);
    endtask
  
    function void extract_phase(uvm_phase phase);
      $display("SV producer::%s %s", phase.get_name(), get_full_name());
    endfunction
  
    function void check_phase(uvm_phase phase);
      $display("SV producer::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void report_phase(uvm_phase phase);
      $display("SV producer::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void final_phase(uvm_phase phase);
      $display("SV producer::%s %s", phase.get_name(), get_full_name());
    endfunction

  endclass

  // Consumer class responds to packets received through the 
  // analysis port
  class consumer #(type T=packet) extends uvm_component;
    uvm_analysis_imp #(T,consumer #(T)) aimp;
  
    function new(string name, uvm_component parent=null);
      super.new(name,parent);
      $display("SV consumer::new");
      aimp=new("aimp",this);
      $display("SV consumer.aimp.get_full_name() = ", aimp.get_full_name());
    endfunction
  
    typedef consumer#(T) cons_type;
    `uvm_component_utils_begin(cons_type)
    `uvm_component_utils_end
  
    function void write (T p);
      $display("[SV ",$realtime," ns] consumer::write(",p.data,") ", get_full_name());
    endfunction 
  
    function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      $display("SV consumer::connect %s", get_full_name());
    endfunction

    function void resolve_bindings();
      $display("SV consumer::resolve_bindings %s", get_full_name());
    endfunction
   
    function void end_of_elaboration();
      $display("SV consumer::end_of_elaboration %s", get_full_name());
    endfunction
   
    function void start_of_simulation();
      $display("SV consumer::start_of_simulation %s", get_full_name());
    endfunction
   
    function void extract_phase(uvm_phase phase);
      $display("SV consumer::%s %s", phase.get_name(), get_full_name());
    endfunction
  
    function void check_phase(uvm_phase phase);
      $display("SV consumer::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void report_phase(uvm_phase phase);
      $display("SV consumer::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void final_phase(uvm_phase phase);
      $display("SV consumer::%s %s", phase.get_name(), get_full_name());
    endfunction
    
  endclass
    
  // The environment contains one instance of the producer and 
  // one instance of the consumer
  class env extends uvm_env;
    producer #(packet) prod;
    consumer #(packet) cons;
    uvm_component sctop1;
     
    function new (string name, uvm_component parent=null);
      super.new(name,parent);
      $display("SV env::new %s", name);
    endfunction

    function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $display("SV env::build");
      prod = producer#(packet)::type_id::create("producer", this);
      cons = consumer#(packet)::type_id::create("consumer", this);       
      // create ML junction node in e
      sctop1 = uvm_ml_create_component("e", "my_unit", "my_unit", this);
    endfunction // void
   
    // Connect ports in connect phase
    function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      $display("SV env::connect %s", get_full_name());
    endfunction

    function void resolve_bindings();
      $display("SV env::resolve_bindings %s", get_full_name());
    endfunction
   
    function void end_of_elaboration();
      $display("SV env::end_of_elaboration %s", get_full_name());
    endfunction
   
    function void start_of_simulation();
      $display("SV env::start_of_simulation %s", get_full_name());
    endfunction
   
    function void extract_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endfunction
  
    function void check_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void report_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void final_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endfunction

    task reset_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endtask

    task configure_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endtask

    task main_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endtask

    task shutdown_phase(uvm_phase phase);
      $display("SV env::%s %s", phase.get_name(), get_full_name());
    endtask
  
    `uvm_component_utils(env)

  endclass 
  
  // Test instantiates the "env"
  class test extends uvm_env;
    env sv_env;
    bit result;

    function new (string name, uvm_component parent=null);
      super.new(name,parent);
      $display("SV test::new %s", get_full_name());
    endfunction

    function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $display("SV test::build %s", get_full_name());
      sv_env = new("sv_env", this);
    endfunction
   
    function void phase_ended(uvm_phase phase);
      if (phase.get_name() == "build") begin
         uvm_ml::ml_tlm1#(packet)::register(sv_env.prod.aport);
         uvm_ml::ml_tlm1#(packet)::register(sv_env.cons.aimp);
      end
    endfunction

    function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      $display("SV test::connect %s", get_full_name());
      result = uvm_ml::connect(sv_env.prod.aport.get_full_name(), "test.sv_env.my_unit.inport");
      result = uvm_ml::connect("test.sv_env.my_unit.outport", sv_env.cons.aimp.get_full_name());     
    endfunction

    function void resolve_bindings();
      $display("SV test::resolve_bindings %s", get_full_name());
    endfunction

    function void end_of_elaboration();
      $display("SV test::end_of_elaboration %s", get_full_name());
    endfunction

    function void start_of_simulation();
      $display("SV test::start_of_simulation %s", get_full_name());
    endfunction
   
    task run_phase(uvm_phase phase);
      $display("SV test::run_phase %s", get_full_name());
    endtask

    function void extract_phase(uvm_phase phase);
      $display("SV test::%s %s", phase.get_name(), get_full_name());
    endfunction
  
    function void check_phase(uvm_phase phase);
      $display("SV test::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void report_phase(uvm_phase phase);
      $display("SV test::%s %s", phase.get_name(), get_full_name());
    endfunction

    function void final_phase(uvm_phase phase);
      $display("SV test::%s %s", phase.get_name(), get_full_name());
    endfunction

    `uvm_component_utils(test)
  endclass    

module topmodule;
`ifdef USE_UVM_ML_RUN_TEST
initial begin
   string tops[1];
   
   tops[0] = "SV:test";
   
   uvm_ml_run_test(tops, "");
end
`endif   
endmodule
