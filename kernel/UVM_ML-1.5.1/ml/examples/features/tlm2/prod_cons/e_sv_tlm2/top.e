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

<'
import uvm_e/e/uvm_e_top.e;
import producer.e;

unit env like uvm_env {
   producer is instance;

   connect_ports() is also {
      compute uvm_ml.connect_names(producer.b_socket.name(), 
       "svtop.sv_env.consumer.b_target_socket");
      compute uvm_ml.connect_names(producer.nb_socket.name(), 
       "svtop.sv_env.consumer.nb_target_socket");
   };
}; // unit env


extend sys {
   env is instance;
};

'>