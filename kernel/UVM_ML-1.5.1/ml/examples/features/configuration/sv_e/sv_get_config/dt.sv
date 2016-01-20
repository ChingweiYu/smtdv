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

// Definition of data for configuration 
class data extends uvm_transaction;

  int    addr;
  int    trailer;
  string txt;

  `uvm_object_utils_begin(data)
    `uvm_field_int(addr, UVM_ALL_ON)
     `uvm_field_int(trailer, UVM_ALL_ON)
     `uvm_field_string(txt, UVM_ALL_ON)
  `uvm_object_utils_end

  function new(string name = "data");
    super.new(name);
  endfunction
endclass // data

// Definition of pdata for configuration
typedef struct packed {
   int 	       data;
   bit [3:0]   addr;
   int 	       payload;   
} pdata;


