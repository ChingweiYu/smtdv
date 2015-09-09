//----------------------------------------------------------------------
//   Copyright 2009 Cadence Design Systems, Inc.
//   Copyright 2012 Advanced Micro Devices Inc.
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

/*! \file uvm_globals.h
  \brief Header file for global UVM-SC functionality.
*/

#ifndef UVM_GLOBALS_H
#define UVM_GLOBALS_H

#include <algorithm>

#ifndef UVM_UNUSED_ATTR
#ifdef __GNUC__
#define UVM_UNUSED_ATTR __attribute__ ((unused))
#else
#define UVM_UNUSED_ATTR
#endif
#endif // UVM_UNUSED_ATTR

#include "base/uvm_manager.h"
#include "base/uvm_phase_controller.h"
#include "base/uvm_phase.h"
#include "base/uvm_factory.h"       // Note:  Provides global factory fns

//////////////

namespace uvm {

class uvm_resource_base;
class uvm_object;

//------------------------------------------------------------------------------
//
// Global UVM functions.
//
//------------------------------------------------------------------------------

void run_test(const std::vector<sc_core::sc_module *>& tops);
//void run_test(const std::vector<sc_core::sc_module *>& tops, const sc_core::sc_time & duration); 


//------------------------------------------------------------------------------
//
// Functions to control termination behavior.
//
//------------------------------------------------------------------------------

void uvm_set_stop_mode(uvm::stop_mode_enum mode = UVM_SC_STOP);
void uvm_set_global_timeout(sc_core::sc_time t);
void uvm_stop_request();

//------------------------------------------------------------------------------
//
// Global function to look up an uvm_component with a given name.
//
//------------------------------------------------------------------------------

uvm_component* uvm_find_component(const char* name);
uvm_component* uvm_find_module(const std::string name);
std::vector<uvm_component*> uvm_find_all(const std::string name);
std::vector<uvm_component*> uvm_find_all_module(const std::string name);
std::vector<uvm_component*> uvm_get_uvm_component_tops();
std::vector<sc_core::sc_module*> uvm_get_tops();

//uvm_pool<uvm_event> *get_global_event_pool(); 


bool uvm_add_schedule(puvm_schedule pschedule);



//------------------------------------------------------------------------------
//
// Global phase callback agent retrieval function (convenience)
//
//------------------------------------------------------------------------------

puvm_phase_callback_agent uvm_get_phase_callback_agent(const std::string schedule, const std::string phase, uvm_phase_state state);
    


//------------------------------------------------------------------------------
//
// Global configuration set-up functions.
//
//------------------------------------------------------------------------------

template <typename T> 
void uvm_set_config_int(
  const std::string& instname,
  const std::string& field,
  const T& val
);

void uvm_set_config_string(
  const std::string& instname,
  const std::string& field,
  const std::string& val
);

void uvm_set_config_object(
  const std::string& instname,
  const std::string& field,
  uvm_object* val,
  bool clone = true
);


///////////////


//------------------------------------------------------------------------------
//
// Global utils functions
//
//------------------------------------------------------------------------------

enum uvm_rsrc_action
{
    UVM_RESOURCE_SET = 0,
    UVM_RESOURCE_SET_DEFAULT,
    UVM_RESOURCE_SET_OVERRIDE,
    UVM_RESOURCE_SET_OVERRIDE_NAME,
    UVM_RESOURCE_WRITE_BY_NAME,
    UVM_CONFIG_SET
};

std::vector<sc_core::sc_module*> uvm_get_children (sc_core::sc_module * pmod);
std::string uvm_get_cntxt_name(uvm_component *comp);
std::string uvm_glob_to_regex(const std::string &glob);
bool uvm_regex_match(const char * re, const string & arg);

void uvm_register_config_callback(uvm_component *comp);
void uvm_unregister_config_callback(uvm_component *comp);
void uvm_config_callback(std::string cntxt, std::string inst_name, std::string field_name, uvm_resource_base * rsrc);
void uvm_register_rsrc_callback(uvm_component *comp);
void uvm_rsrc_callback(uvm_rsrc_action action, std::string scope, std::string name, uvm_resource_base * rsrc);

} // namespace uvm

#endif
