/*
//----------------------------------------------------------------------
//   Copyright 2012 Cadence Design Systems, Inc.
//   Copyright 2013 Advanced Micro Devices Inc.
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
*/
#ifndef BP_COMMON_C_H
#define BP_COMMON_C_H

#ifdef __cplusplus
extern "C" {
#endif

#define UVM_ML_VERSION "1.5.1"

typedef unsigned * uvm_ml_stream_t;

typedef enum
{
    TIME_UNIT_FS = 0,
    TIME_UNIT_PS,
    TIME_UNIT_NS,
    TIME_UNIT_US,
    TIME_UNIT_MS,
    TIME_UNIT_SEC,
    TIME_UNIT_MIN,
    TIME_UNIT_HR,
    TIME_UNIT_UNDEFINED
} uvm_ml_time_unit;

typedef struct uvm_ml_time 
{
    uvm_ml_time_unit units;
    double           value;
} uvm_ml_time;

typedef unsigned int uvm_ml_type_id_t;

typedef enum
{
    UVM_ML_PHASE_STARTED,
    UVM_ML_PHASE_EXECUTING,
    UVM_ML_PHASE_READY_TO_END,
    UVM_ML_PHASE_ENDED
} uvm_ml_phase_action;

typedef enum
{
    UVM_ML_TLM2_UNINITIALIZED_PHASE = 0,
    UVM_ML_TLM2_BEGIN_REQ,
    UVM_ML_TLM2_END_REQ,
    UVM_ML_TLM2_BEGIN_RESP,
    UVM_ML_TLM2_END_RESP  
} uvm_ml_tlm_phase;

typedef enum
{
    UVM_ML_TLM2_ACCEPTED = 0, 
    UVM_ML_TLM2_UPDATED, 
    UVM_ML_TLM2_COMPLETED
} uvm_ml_tlm_sync_enum;

typedef enum
{
    UVM_ML_RESOURCE_SET = 0,
    UVM_ML_RESOURCE_SET_DEFAULT,
    UVM_ML_RESOURCE_SET_OVERRIDE,
    UVM_ML_RESOURCE_SET_OVERRIDE_NAME,
    UVM_ML_RESOURCE_WRITE_BY_NAME,
    UVM_ML_CONFIG_SET
} uvm_ml_resource_notify_action;

typedef enum
{
    UVM_ML_EVENT_NEW = 0,
    UVM_ML_EVENT_TRIGGER,
    UVM_ML_EVENT_RESET,
    UVM_ML_EVENT_CANCEL,
    UVM_ML_EVENT_DELETE
} uvm_ml_event_notify_action;

typedef enum
{
    UVM_ML_BARRIER_NEW = 0,
    UVM_ML_BARRIER_RESET,
    UVM_ML_BARRIER_SET_AUTO_RESET,
    UVM_ML_BARRIER_SET_THRESHOLD,
    UVM_ML_BARRIER_CANCEL,
    UVM_ML_BARRIER_DELETE
} uvm_ml_barrier_notify_action;

typedef struct bp_srv_provider_struct
{
    const char * phase_srv_provider;
    const char * wakeup_srv_provider;
    const char * stopper_srv_provider;
    const char * print_srv_provider;
} bp_srv_provider_struct;


//-------------------------------
//
// C Callback Function Typedef's
//
//-------------------------------

// Test start phasing callback function type. BpInterconnect::Elaborate() is being used as such a callback
typedef int   (bp_preInitialCB_t)(void *cbInfo);

// Start phasing callback registration function type
typedef void  (bp_preInitial_t) ( bp_preInitialCB_t *func, 
                                  void *             cbInfo); // Reserved for potential use

//! Error handling function type; It should be provided by the master framework
typedef void (bp_serror_t)(int msgId, const char *format,...);

// Printing callback type; It can be used if an adapter needs to stream the output on top of the printing service (e.g. for GUI support)
typedef void (bp_output_cb_t)(const char * msg);

typedef long long uvm_ml_handle;

#define UVM_ML_ERROR   0
#define UVM_ML_WARNING 1
#define UVM_ML_DEBUG   2
#define UVM_ML_INFO    3

void * BpDlsym(const char * sym);



//-------------------------------
//
// Helper Functions for the C++ adapters
//
//-------------------------------

void bp_utils_uvm_ml_printf(int level, const char * caller_name, const char *string,... );

#ifdef __cplusplus
}
#endif 

#endif /* BP_COMMON_C_H */
