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
unit consumer {
    b_tsocket     : tlm_target_socket of tlm_generic_payload is instance;
    nb_tsocket    : tlm_target_socket of tlm_generic_payload is instance;
    save: list of int;
   
   connect_ports() is also {
      b_tsocket.connect(external);
      nb_tsocket.connect(external);
   };
    
    nb_transport_fw(trans: tlm_generic_payload, phase: *tlm_phase_enum, t: *time): tlm_sync_enum is {
       if(trans.m_command == TLM_WRITE_COMMAND) then {
          message(LOW,"Received nb_transport_fw WRITE ", hex(trans.m_data));
          save.clear();
          for i from 0 to trans.m_length-1 do {
             save.push(trans.m_data[i]);
          }; // for i from 0 to...
       } else {
          for i from 0 to trans.m_length-1 do {
             trans.m_data[i] = save.pop0();
          }; // for i from 0 to...         
          message(LOW,"Received nb_transport_fw READ ", hex(trans.m_data));
       };
       trans.m_response_status = TLM_OK_RESPONSE;
       return TLM_COMPLETED;
    };
    
    b_transport(trans: tlm_generic_payload, t: *time)@sys.any is {
       if(trans.m_command == TLM_WRITE_COMMAND) then {
          message(LOW,"Received b_transport WRITE ", hex(trans.m_data));
          save.clear();
          for i from 0 to trans.m_length-1 do {
             save.push(trans.m_data[i]);
          }; // for i from 0 to...
       } else {
          for i from 0 to trans.m_length-1 do {
             trans.m_data[i] = save.pop0();
          }; // for i from 0 to...         
          message(LOW,"Received b_transport READ ", hex(trans.m_data));
       };
       trans.m_response_status = TLM_OK_RESPONSE;
       wait delay(5 ns);
    };

    
   transport_dbg(trans: tlm_generic_payload) : uint is  {
      message(LOW,"Received transport_dbg ", trans);
   }; // transport_dbg 
   
}; // unit consumer
'>