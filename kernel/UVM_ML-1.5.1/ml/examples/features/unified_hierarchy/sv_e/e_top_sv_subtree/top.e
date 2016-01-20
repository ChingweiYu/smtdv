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
// e is the top component and it creates an SV subtree from its env
<'
import packet;

extend sys {
    u is instance;
};

unit u {
  
    inport  :  in interface_port of tlm_analysis of (packet) is instance;
       keep bind(inport,external);
    outport : out interface_port of tlm_analysis of (packet) is instance;
       keep bind(outport,external);
    
    sv_uvc_proxy: child_component_proxy is instance;
    keep sv_uvc_proxy.type_name == "SV:sv_uvc";
    
    pre_generate() is also {
        message(LOW,"[SN] pre_generate()");
    };
    
    post_generate() is also {
        message(LOW,"[SN] post_generate()");
    };
    
    connect_ports() is also {
        message(LOW,"[SN] connect_ports()");
        compute uvm_ml.connect_names("sys.u.outport", "sys.u.sv_uvc_proxy.sv_env.consumer.aimp");
        compute uvm_ml.connect_names("sys.u.sv_uvc_proxy.sv_env.producer.aport", "sys.u.inport");
    };
    
    run() is also {
        message(LOW,"[SN] run()");
        start tcm();
    };
    
    tcm()@sys.any is {
       var p : packet;
       p = new;
       wait delay (50);
       
       for i from 0 to 4 {
          p.data = 17+i;
          message(LOW,"[SN] sending packet ", p.data);
          outport$.write(p);
          wait delay (10);
       }; // for i from 1 to...
       stop_run();
    };
    
    data_ref: int;
    keep data_ref == 101;
    
    write(p:packet) is {
        message(LOW,append("[SN] Got packet p.data = ",p.data));
        check that p.data == data_ref;
        data_ref+=1;
    };
    
    check() is also {
        message(LOW,"[SN] check()");
        check that data_ref == 106;
        message(LOW,"** UVM TEST PASSED");
    };

};


'>
