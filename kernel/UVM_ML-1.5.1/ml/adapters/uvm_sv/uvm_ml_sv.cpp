//----------------------------------------------------------------------
//   Copyright 2012 Cadence Design Systems, Inc.
//   Copyright 2012-2013 Advanced Micro Devices Inc.
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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <math.h>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <vector>
#include <sstream>
#include <set>
#include "bp_common_c.h"
#include "bp_required.h"
#include "bp_provided.h"
#include "bp_framework_proxy.h"

/* ======================================
   Helper functions
   ====================================== */
#define FIND_GLOBAL_API(NAME) \
  if (NAME##_ptr == 0 ) { \
    NAME##_ptr = (NAME##_type)GlobalDlsym(#NAME);\
    list_of_backplane_exports.push_back((void**)&NAME##_ptr);\
  }

namespace {
    static std::vector<void**> list_of_dpi_exports;
    static std::vector<void**> list_of_backplane_exports;

    static void set_to_zero(void **ptr) { 
        *ptr = (void*)0;
    }
    /* Similar to BpDlsym of the backplane */
    void * GlobalDlsym(const char * sym)
    {
        static int   init = 0;
        static void *libHandle = NULL;

        if(!init) 
            {
                libHandle = dlopen(0, RTLD_LAZY | RTLD_GLOBAL);
                if(!libHandle) return NULL;
                init = 1;
            }
        return dlsym(libHandle, sym);
    }

    typedef  int (*bp_interconnect_get_trace_mode_type)();
    static bp_interconnect_get_trace_mode_type bp_interconnect_get_trace_mode_ptr = 0;

    typedef  void (*bp_utils_uvm_ml_printf_type)(int level, const char * caller_name, const char *string,...);
    static bp_utils_uvm_ml_printf_type bp_utils_uvm_ml_printf_ptr = 0;

    typedef const char* (*bp_utils_convert_uvm_ml_time_unit_to_c_str_type)(uvm_ml_time_unit unit);
    static bp_utils_convert_uvm_ml_time_unit_to_c_str_type bp_utils_convert_uvm_ml_time_unit_to_c_str_ptr = 0;

    static void internal_printf (bool only_in_trace_mode, int level, const char * caller_name, const char *string,... )  { 
        static int trace_mode_flag = -1;

        if (only_in_trace_mode == true) {
          if ( trace_mode_flag == -1 ) { 
              FIND_GLOBAL_API(bp_interconnect_get_trace_mode);
              if ( bp_interconnect_get_trace_mode_ptr ) { 
                  trace_mode_flag = bp_interconnect_get_trace_mode_ptr();
              } else { 
                  trace_mode_flag = -2;
              }
	  }
        }  

        FIND_GLOBAL_API(bp_utils_uvm_ml_printf);

        if (  bp_utils_uvm_ml_printf_ptr && ((trace_mode_flag == 1) || (only_in_trace_mode == false))) {
            va_list args;
            va_start(args, string);
            bp_utils_uvm_ml_printf_ptr(level, caller_name, string, args);
            va_end(args);
        }
    }
}

extern "C" {

#include <acc_user.h>
#include "veriuser.h"
#include "vpi_user.h"
#include "sv_vpi_user.h"
#define DPI_COMPATIBILITY_VERSION_1800v2005
#include "svdpi.h"

// Local (SV) function signatures that differ from Required API functions because
// they do not accept time stamp arguments

typedef int (*local_put_uvm_ml_stream_type) 
(
    unsigned           connector_id, 
    unsigned           stream_size, 
    uvm_ml_stream_t    stream
);

typedef int (*local_put_request_type) 
(
    unsigned        connector_id, 
    unsigned        call_id, 
    unsigned        callback_adapter_id, 
    unsigned        stream_size, 
    uvm_ml_stream_t stream
);

typedef int (*local_try_put_type)
(
    unsigned        connector_id, 
    unsigned        stream_size , 
    uvm_ml_stream_t stream
);

typedef int (*local_can_put_type) 
(
    unsigned         connector_id
);

typedef int (*local_get_type) 
(
    unsigned           connector_id, 
    unsigned*          stream_size_ptr, 
    uvm_ml_stream_t    stream_ptr
);

typedef int (*local_get_request_type) 
(
    unsigned connector_id, 
    unsigned call_id, 
    unsigned callback_adapter_id
);

typedef unsigned (*local_get_requested_type) 
(
    unsigned        connector_id, 
    unsigned        call_id, 
    uvm_ml_stream_t stream
);

typedef int (*local_try_get_type)
(
    unsigned        connector_id, 
    unsigned *      stream_size_ptr, 
    uvm_ml_stream_t stream
);

typedef int (*local_can_get_type) 
(
    unsigned         connector_id
);

typedef int (*local_peek_type) 
(
    unsigned        connector_id, 
    unsigned *      stream_size_ptr, 
    uvm_ml_stream_t stream_ptr
);

typedef int (*local_peek_request_type) 
(
    unsigned         connector_id, 
    unsigned         call_id, 
    unsigned         callback_adapter_id
);

typedef unsigned (*local_peek_requested_type) 
(
    unsigned        connector_id, 
    unsigned        call_id, 
    uvm_ml_stream_t stream
);

typedef int (*local_try_peek_type)
(
    unsigned        connector_id, 
    unsigned *      stream_size_ptr, 
    uvm_ml_stream_t stream
);

typedef int (*local_can_peek_type) 
(
    unsigned         connector_id
);

typedef int (*local_transport_type) 
(
    unsigned        connector_id, 
    unsigned        req_stream_size, 
    uvm_ml_stream_t req_stream,
    unsigned *      rsp_stream_size_ptr,
    uvm_ml_stream_t rsp_stream
);

typedef int (*local_transport_request_type) 
(
    unsigned        connector_id, 
    unsigned        call_id, 
    unsigned        callback_adapter_id, 
    unsigned        req_stream_size, 
    uvm_ml_stream_t req_stream,
    unsigned *      rsp_stream_size_ptr,
    uvm_ml_stream_t rsp_stream
);

typedef unsigned (*local_transport_response_type) 
(
    unsigned        connector_id, 
    unsigned        call_id, 
    uvm_ml_stream_t rsp_stream
);

typedef int (*local_nb_transport_type) 
(
    unsigned        connector_id, 
    unsigned        req_stream_size, 
    uvm_ml_stream_t req_stream,
    unsigned*       rsp_stream_size_ptr,
    uvm_ml_stream_t rsp_stream
);

typedef void (*local_tlm_write_type)
(
    unsigned        connector_id, 
    unsigned        stream_size, 
    uvm_ml_stream_t stream
);

typedef void (*local_notify_end_blocking_type) 
(
    unsigned         call_id
);

typedef uvm_ml_tlm_sync_enum (*local_tlm2_nb_transport_incoming_type) (
  unsigned            connector_id,
  unsigned *          stream_size,
  uvm_ml_stream_t     stream,
  uvm_ml_tlm_phase *  phase,
  unsigned int        transaction_id,
  uvm_ml_time_unit *  delay_unit,
  double *            delay_value
);

typedef int (*local_tlm2_b_transport_request_type) (
  unsigned          target_connector_id, 
  unsigned          call_id,
  unsigned          callback_adapter_id,
  unsigned          stream_size,
  uvm_ml_stream_t   stream,
  uvm_ml_time_unit  delay_unit,
  double            delay_value
);

typedef int (*local_tlm2_b_transport_response_type) (
  unsigned          target_connector_id, 
  unsigned          call_id,
  unsigned *        stream_size,
  uvm_ml_stream_t   stream
);

typedef int (*local_notify_runtime_phase_type) (
  const char *      phase_group,
  const char *      phase_name,
  unsigned int      phase_action,
  unsigned int *    participate
);

typedef int (*local_notify_resource_type) (
  uvm_ml_resource_notify_action action,
  const char *                  item_scope,
  const char *                  item_name,
  const char *                  accessor_name,
  unsigned int                  stream_size,
  uvm_ml_stream_t               stream
);

typedef int (*local_notify_config_type) (
  const char *    cntxt,
  const char *    instance_name,
  const char *    field_name,
  unsigned int    stream_size,
  uvm_ml_stream_t stream
);

typedef void (*local_notify_time_cb_helper_type) ();

typedef int (*local_notify_event_type) (
  const char *                  scope_name,
  const char *                  event_name,
  uvm_ml_event_notify_action    action,
  unsigned int                  stream_size,
  uvm_ml_stream_t               stream
);

typedef int (*local_notify_barrier_type) (
  const char *                  scope_name,
  const char *                  barrier_name,
  uvm_ml_barrier_notify_action  action,
  int                           data
);

// Command-specific API:
// UVM SV adapter parses the incoming commands in the C part and calls the command-specific 
// export DPI functions

typedef int (*local_notify_trace_register_command_type)
(
  int turn_on // 1 - turn on, 0 - off
);

// The rest of the local (SV) function signatures replicate the required API types because
// those also do not specify time stamp arguments:

typedef frmw_startup_type local_startup_type;
typedef frmw_construct_top_type local_construct_top_type;

typedef frmw_set_trace_mode_type local_set_trace_mode_type;

typedef frmw_find_connector_id_by_name_type  local_find_connector_id_by_name_type;
typedef frmw_get_connector_intf_name_type local_get_connector_intf_name_type;
typedef frmw_get_connector_T1_name_type local_get_connector_T1_name_type;
typedef frmw_get_connector_T2_name_type local_get_connector_T2_name_type;
typedef frmw_get_connector_type_type local_get_connector_type_type;
typedef frmw_tlm2_turn_off_transaction_mapping_type local_tlm2_turn_off_transaction_mapping_type;
typedef frmw_external_connect_type local_external_connect_type;
typedef frmw_create_child_junction_node_type local_create_child_junction_node_type;
typedef frmw_transmit_phase_type local_transmit_phase_type;
typedef frmw_notify_phase_type local_notify_phase_type;
typedef frmw_phase_srv_notify_phase_done_type local_phase_srv_notify_phase_done_type;
typedef frmw_phase_srv_start_type local_phase_srv_start_type;

typedef frmw_stopper_srv_stop_request_type local_stopper_srv_stop_request_type;
typedef frmw_phase_srv_check_phase_type local_phase_srv_check_phase_type;
typedef frmw_phase_srv_check_future_phase_type local_phase_srv_check_future_phase_type;
typedef frmw_get_num_children_type local_get_num_children_type;
typedef frmw_get_child_name_type local_get_child_name_type;
typedef frmw_get_component_type_name_type local_get_component_type_name_type;
typedef frmw_is_port_type local_is_port_type;
typedef frmw_print_srv_print_request_type local_print_srv_print_request_type;


/* =======================================
   SV ADAPTER AUXILIARY FUNCTION POINTERS
   ====================================== */

extern bool bp_automatic_fake_blocking;

typedef void (*sv_blocking_calls_helper_t)();
static sv_blocking_calls_helper_t sv_blocking_calls_helper_ptr = 0;

static bool set_sv_scope()
{
    svScope s = svGetScopeFromName("uvm_ml_adapter_imp");

    if (s == 0) { 
        s = svGetScopeFromName("uvm_ml_adapter_imp::");
    }
    // ===================================================

    if (s == 0) 
    {
        FIND_GLOBAL_API(bp_utils_uvm_ml_printf);
        if ( bp_utils_uvm_ml_printf_ptr ) {
            bp_utils_uvm_ml_printf_ptr(UVM_ML_ERROR, "SV C", "svGetScopeFromName returned 0\n");
        }
        return false;
    };
    svSetScope(s);
    return true;
}


static void legacy_blocking_call_helper()
{
    if (!sv_blocking_calls_helper_ptr)
        sv_blocking_calls_helper_ptr = (sv_blocking_calls_helper_t) GlobalDlsym("unilang_sv_blocking_calls_helper");
    (*sv_blocking_calls_helper_ptr)();
}

#if __cplusplus < 201103L
    // pre-C++11
    #define TYPEOF __typeof__
#else
    // C++11
    #define TYPEOF decltype
#endif

#define SV_REQUIRED_C_FUNC_COMMON(NAME,ERR_VAL)			  \
    static local_##NAME##_type NAME##_ptr = 0; \
    static string str1 = string("In ") + #NAME + "\n"; \
    static string sv_name = string("uvm_ml_uvmsv_export_") + #NAME;  \
    static string str2 = sv_name + " not found\n"; \
    internal_printf(true, UVM_ML_DEBUG, "SV C", str1.c_str());	\
    if (NAME##_ptr == NULL) \
    { \
        NAME##_ptr = (TYPEOF(NAME##_ptr)) GlobalDlsym(sv_name.c_str());	\
        if (NAME##_ptr == NULL) \
        { \
            FIND_GLOBAL_API(bp_utils_uvm_ml_printf); \
            if (bp_utils_uvm_ml_printf_ptr ) { \
                bp_utils_uvm_ml_printf_ptr(UVM_ML_ERROR, "SV C", str2.c_str()); \
            } \
            return ERR_VAL; \
        } \
        list_of_dpi_exports.push_back((void**)(&NAME##_ptr));       \
    } \
    if (set_sv_scope() == false) {				\
        FIND_GLOBAL_API(bp_utils_uvm_ml_printf); \
        if ( bp_utils_uvm_ml_printf_ptr ) { \
            bp_utils_uvm_ml_printf_ptr(UVM_ML_ERROR, "SV C", "set_sv_scope() failed"); \
        } \
       return ERR_VAL; \
    }

static void set_trace_mode(int mode)
{
   SV_REQUIRED_C_FUNC_COMMON(set_trace_mode, )
   (*set_trace_mode_ptr)(mode);
}

static int find_connector_id_by_name (const char * path)
{
    SV_REQUIRED_C_FUNC_COMMON(find_connector_id_by_name,(-1))
    return (*find_connector_id_by_name_ptr)(path);
}

static const char * get_connector_intf_name (unsigned connector_id)
{
    SV_REQUIRED_C_FUNC_COMMON(get_connector_intf_name, 0)
    return (*get_connector_intf_name_ptr)(connector_id);
}

static const char * get_connector_T1_name (unsigned connector_id)
{
    SV_REQUIRED_C_FUNC_COMMON(get_connector_T1_name,NULL)
    return (*get_connector_T1_name_ptr)(connector_id);
}

static const char * get_connector_T2_name (unsigned connector_id)
{
    SV_REQUIRED_C_FUNC_COMMON(get_connector_T2_name,NULL)
    return (*get_connector_T2_name_ptr)(connector_id);
}

static int get_connector_type (unsigned connector_id)
{
    SV_REQUIRED_C_FUNC_COMMON(get_connector_type,0)
    return (*get_connector_type_ptr)(connector_id);
}

static void external_connect(unsigned connector_id)
{
    SV_REQUIRED_C_FUNC_COMMON(external_connect, )
    (*external_connect_ptr)(connector_id);
}

static int startup_phases()
{
    SV_REQUIRED_C_FUNC_COMMON(startup,0)
    return (*startup_ptr)();
}

static int construct_top(const char * top_identifier, const char * instance_name)
{
    SV_REQUIRED_C_FUNC_COMMON(construct_top,(-1))
    return (*construct_top_ptr)(top_identifier, instance_name);
}

static int tlm1_put_request(unsigned         export_connector_id, 
                            unsigned         call_id, 
                            unsigned         callback_adapter_id, 
                            unsigned         stream_size, 
                            uvm_ml_stream_t  stream, 
                            uvm_ml_time_unit UNUSED time_unit, 
                            double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(put_request,0)
    (*put_request_ptr)(export_connector_id,call_id,callback_adapter_id,stream_size,stream);
    legacy_blocking_call_helper();
    return 0; // TODO: add support for disabled processes
}

static int tlm1_try_put(unsigned                export_connector_id, 
                        unsigned                stream_size, 
                        uvm_ml_stream_t         stream, 
                        uvm_ml_time_unit UNUSED time_unit, 
                        double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(try_put,0)
    return (*try_put_ptr)(export_connector_id,stream_size,stream);
}

static int tlm1_can_put(unsigned                export_connector_id, 
                        uvm_ml_time_unit UNUSED time_unit, 
                        double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(can_put,0)
    return (*can_put_ptr)(export_connector_id);
}

static int tlm1_get_request(unsigned                export_connector_id, 
                            unsigned                call_id,
                            unsigned                callback_adapter_id, 
                            uvm_ml_time_unit UNUSED time_unit, 
                            double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(get_request,0)
    (*get_request_ptr)(export_connector_id,call_id,callback_adapter_id);
    legacy_blocking_call_helper();
    return 0;
}

static int tlm1_try_get(unsigned                export_connector_id, 
                        unsigned *              stream_size_ptr,
                        uvm_ml_stream_t         stream, 
                        uvm_ml_time_unit UNUSED time_unit, 
                        double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(try_get,0)
    return (*try_get_ptr)(export_connector_id, stream_size_ptr, stream);
}

static int tlm1_can_get(unsigned                export_connector_id, 
                        uvm_ml_time_unit UNUSED time_unit, 
                        double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(can_get,0)
    return (*can_get_ptr)(export_connector_id);
}

static unsigned tlm1_get_requested(unsigned        export_connector_id, 
                                   unsigned        call_id,
                                   uvm_ml_stream_t stream)
{
    SV_REQUIRED_C_FUNC_COMMON(get_requested,0)
    return (*get_requested_ptr)(export_connector_id,call_id,stream); // Returns stream size
}

static int tlm1_peek_request(unsigned                export_connector_id, 
                             unsigned                call_id,
                             unsigned                callback_adapter_id, 
                             uvm_ml_time_unit UNUSED time_unit, 
                             double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(peek_request,0)
    (*peek_request_ptr)(export_connector_id,call_id,callback_adapter_id);
    legacy_blocking_call_helper();
    return 0;
}

static unsigned tlm1_peek_requested(unsigned        export_connector_id, 
                                    unsigned        call_id,
                                    uvm_ml_stream_t stream)
{
    // same function is used for get and peek
    SV_REQUIRED_C_FUNC_COMMON(get_requested,0)
    return (*get_requested_ptr)(export_connector_id,call_id,stream); // Returns stream size
}

static int tlm1_try_peek(unsigned         export_connector_id, 
                         unsigned *       stream_size_ptr,
                         uvm_ml_stream_t  stream, 
                         uvm_ml_time_unit UNUSED time_unit, 
                         double           UNUSED time_value) 
{
    SV_REQUIRED_C_FUNC_COMMON(try_peek,0)
    return (*try_peek_ptr)(export_connector_id, stream_size_ptr, stream);
}

static int tlm1_can_peek(unsigned                export_connector_id, 
                         uvm_ml_time_unit UNUSED time_unit, 
                         double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(can_peek,0)
    return (*can_peek_ptr)(export_connector_id);
}

static int tlm1_transport_request(unsigned                export_connector_id, 
                                  unsigned                call_id, 
                                  unsigned                callback_adapter_id, 
                                  unsigned                req_stream_size, 
                                  uvm_ml_stream_t         req_stream,
                                  unsigned *              rsp_stream_size, 
                                  uvm_ml_stream_t         rsp_stream, 
                                  uvm_ml_time_unit UNUSED time_unit, 
                                  double           UNUSED time_value
)
{
    SV_REQUIRED_C_FUNC_COMMON(transport_request,0)
    (*transport_request_ptr)(export_connector_id, call_id, callback_adapter_id, 
                             req_stream_size, req_stream, rsp_stream_size, rsp_stream);
    legacy_blocking_call_helper();
    return 0;
}

static unsigned tlm1_transport_response(unsigned        export_connector_id, 
                                        unsigned        call_id,
                                        uvm_ml_stream_t rsp_stream)
{
    SV_REQUIRED_C_FUNC_COMMON(transport_response,0)
    return (*transport_response_ptr)(export_connector_id, call_id, rsp_stream);
}

static int tlm1_nb_transport(unsigned                export_connector_id, 
                             unsigned                req_stream_size, 
                             uvm_ml_stream_t         req_stream,
                             unsigned *              rsp_stream_size, 
                             uvm_ml_stream_t         rsp_stream, 
                             uvm_ml_time_unit UNUSED time_unit, 
                             double           UNUSED time_value
)
{
    SV_REQUIRED_C_FUNC_COMMON(nb_transport,0)
    return (*nb_transport_ptr)(export_connector_id, req_stream_size, req_stream, rsp_stream_size, rsp_stream);
}

static void tlm_write(unsigned                connector_id, 
                      unsigned                stream_size , 
                      uvm_ml_stream_t         stream, 
                      uvm_ml_time_unit UNUSED time_unit, 
                      double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(tlm_write, )
    (*tlm_write_ptr)(connector_id, stream_size,stream);
}

static void tlm_notify_end_blocking(unsigned                call_id, 
                                    uvm_ml_time_unit UNUSED time_unit, 
                                    double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(notify_end_blocking, )
    (*notify_end_blocking_ptr)(call_id);
}

static int transmit_phase(int                 target_id,
                          const char *        phase_group,
                          const char *        phase_name,
                          uvm_ml_phase_action phase_action)
{
    SV_REQUIRED_C_FUNC_COMMON(transmit_phase, (-1))
    return (*transmit_phase_ptr)(target_id, phase_group, phase_name, phase_action);
}

static int notify_phase(const char *        phase_group,
                        const char *        phase_name,
                        uvm_ml_phase_action phase_action)
{
    SV_REQUIRED_C_FUNC_COMMON(notify_phase, (-1))
    return (*notify_phase_ptr)(phase_group, phase_name, phase_action);
}

int notify_runtime_phase(const char *            phase_group,
                         const char *            phase_name,
                         uvm_ml_phase_action     phase_action,
                         uvm_ml_time_unit UNUSED time_unit,
                         double           UNUSED time_value,
                         unsigned int *          participate)
{
    SV_REQUIRED_C_FUNC_COMMON(notify_runtime_phase, (-1))
    return (*notify_runtime_phase_ptr)(phase_group, phase_name, phase_action, participate);
}

static uvm_ml_tlm_sync_enum tlm2_nb_transport_incoming(unsigned                target_connector_id,
                                                       unsigned *              stream_size,
                                                       uvm_ml_stream_t *       stream,
                                                       uvm_ml_tlm_phase *      phase,
                                                       unsigned int            transaction_id,
                                                       uvm_ml_time_unit *      delay_unit,
                                                       double *                delay_value, 
                                                       uvm_ml_time_unit UNUSED time_unit, 
                                                       double           UNUSED time_value)
{
   SV_REQUIRED_C_FUNC_COMMON(tlm2_nb_transport_incoming,UVM_ML_TLM2_COMPLETED);
   return (*tlm2_nb_transport_incoming_ptr)(target_connector_id,stream_size, *stream,phase,transaction_id,delay_unit,delay_value);
}

static int tlm2_b_transport_request(unsigned                target_connector_id, 
                                    unsigned                call_id,
                                    unsigned                callback_adapter_id,
                                    unsigned                stream_size,
                                    uvm_ml_stream_t         stream,
                                    uvm_ml_time_unit        delay_unit,
                                    double                  delay_value, 
                                    uvm_ml_time_unit UNUSED time_unit, 
                                    double           UNUSED time_value)
{
    SV_REQUIRED_C_FUNC_COMMON(tlm2_b_transport_request,1)
    return (*tlm2_b_transport_request_ptr)(target_connector_id,call_id,callback_adapter_id,stream_size,stream,delay_unit,delay_value);
}

static int tlm2_b_transport_response(unsigned          target_connector_id, 
                                     unsigned          call_id,
                                     unsigned *        stream_size,
                                     uvm_ml_stream_t * stream)
{
    SV_REQUIRED_C_FUNC_COMMON(tlm2_b_transport_response,1)
    return (*tlm2_b_transport_response_ptr) (target_connector_id,call_id,stream_size, *stream);
}

static void tlm2_turn_off_transaction_mapping(const char * socket_name)
{
    SV_REQUIRED_C_FUNC_COMMON(tlm2_turn_off_transaction_mapping, )
    (*tlm2_turn_off_transaction_mapping_ptr) (socket_name);
}

static int create_child_junction_node(const char * component_type_name, 
                                      const char * instance_name, 
                                      const char * parent_full_name,
                                      int          parent_framework_id, 
                                      int          parent_junction_node_id)
{
    SV_REQUIRED_C_FUNC_COMMON(create_child_junction_node, (-1))
    return (*create_child_junction_node_ptr)(component_type_name, instance_name, parent_full_name, parent_framework_id, parent_junction_node_id);
}

void phase_srv_start ()
{
    SV_REQUIRED_C_FUNC_COMMON(phase_srv_start, )
    (*phase_srv_start_ptr)();
}

void phase_srv_notify_phase_done
(
    unsigned int     framework_id,
    const char *     phase_group,
    const char *     phase_name,
    uvm_ml_time_unit time_unit,
    double           time_value
)
{
    SV_REQUIRED_C_FUNC_COMMON(phase_srv_notify_phase_done, )
    (*phase_srv_notify_phase_done_ptr)(framework_id, phase_group, phase_name, time_unit, time_value);
}

int phase_srv_check_phase
(
    const char *     phase_name
)
{
    SV_REQUIRED_C_FUNC_COMMON(phase_srv_check_phase, (-1))
    return (*phase_srv_check_phase_ptr)(phase_name);
}

int phase_srv_check_future_phase
(
    const char *        phase_name,
    uvm_ml_phase_action phase_action
)
{
    SV_REQUIRED_C_FUNC_COMMON(phase_srv_check_future_phase, (-1))
      return (*phase_srv_check_future_phase_ptr)(phase_name, phase_action);
}

int notify_resource
(
    uvm_ml_resource_notify_action resource_action,
    const char *                  item_scope,
    const char *                  item_name,
    const char *                  accessor_name,
    unsigned int                  stream_size,
    uvm_ml_stream_t               stream,
    uvm_ml_time_unit UNUSED       time_unit,
    double           UNUSED       time_value
)
{
    SV_REQUIRED_C_FUNC_COMMON(notify_resource, (-1))
    return (*notify_resource_ptr)(resource_action, item_scope, item_name, accessor_name, stream_size, stream);
}

void notify_config
(
    const char *            cntxt,
    const char *            instance_name,
    const char *            field_name,
    unsigned int            stream_size,
    uvm_ml_stream_t         stream,
    uvm_ml_time_unit UNUSED time_unit,
    double           UNUSED time_value
)
{
    SV_REQUIRED_C_FUNC_COMMON(notify_config, )
    (*notify_config_ptr)(cntxt, instance_name, field_name, stream_size, stream);
}

/*
 * ============================================================================================
 * Print service
 * ============================================================================================
 */

void print_srv_print_request(const char * message)
{
    SV_REQUIRED_C_FUNC_COMMON(print_srv_print_request, )
    (*print_srv_print_request_ptr)(message);
}

/*
 * ============================================================================================
 * Stopper service
 * ============================================================================================
 */

void stopper_srv_stop_request()
{
    SV_REQUIRED_C_FUNC_COMMON(stopper_srv_stop_request, )
    (*stopper_srv_stop_request_ptr)();
}

/*
 * ============================================================================================
 * Debug service print tree
 * ============================================================================================
 */

static int get_num_children(const char * component_name)
{
    SV_REQUIRED_C_FUNC_COMMON(get_num_children, (-1))
    return (*get_num_children_ptr)(component_name);
}

static const char * get_child_name(const char * parent_name, unsigned num)
{
    SV_REQUIRED_C_FUNC_COMMON(get_child_name, "")
      return (*get_child_name_ptr)(parent_name, num);
}

static const char * get_component_type_name(const char * component_name)
{
    SV_REQUIRED_C_FUNC_COMMON(get_component_type_name, "")
      return (*get_component_type_name_ptr)(component_name);
}

static int is_port(const char * component_name)
{
    SV_REQUIRED_C_FUNC_COMMON(is_port, (-1))
    return (*is_port_ptr)(component_name);
}

/*
 * ============================================================================================
 * Time callback registration and handling
 * ============================================================================================
 */

typedef 
struct uvm_ml_time_callback_data {
    vpiHandle a_handle;
    int adapter_id;
    void *user_data;

    void do_notify_time_cb(uvm_ml_time * time); 

} t_callback_data,*p_callback_data;

static vector <p_callback_data> current_time_cb_data_v;

static int convert_time_units_from_uvm_ml(uvm_ml_time_unit unit) {
    switch (unit) {
	case TIME_UNIT_FS:
		return -15;
	case TIME_UNIT_PS:
		return -12;
	case TIME_UNIT_NS:
		return -9;
	case TIME_UNIT_US:
		return -6;
	case TIME_UNIT_MS:
		return -3;
	case TIME_UNIT_SEC:
		return 0;
	case TIME_UNIT_MIN:
		return 3;
	case TIME_UNIT_HR:
		return 6;
	default:
		return 0; /* or throw an exception here */
    }
}

static vpiHandle get_package_handle()
{
    vpiHandle aHandle;

    aHandle = vpi_handle_by_name((char *)"uvm_ml_adapter_imp",NULL);

    if ( aHandle == NULL )
    {
      aHandle = vpi_handle_by_name((char *)"uvm_ml_adapter_imp::",NULL);
    }

    return aHandle;
}

static void map_time_unit(int vpi_unit, uvm_ml_time * uvm_ml_unit)
{
    int tmp = 15 + vpi_unit; // VPI returns precision as magnitude of 1 sec
                              // 100 sec -> 2
                              // 1 sec -> 0
                              // 1 ms -> (-3)
                              // 1 fs -> (-15)
    uvm_ml_unit->units = (uvm_ml_time_unit) (tmp/3);
    uvm_ml_unit->value = pow(10.0,(tmp%3));
}

void uvm_ml_uvmsv_get_current_simulation_time(uvm_ml_time_unit * units, double * value)
{
    static uvm_ml_time * simulation_time_unit = NULL;

    if (simulation_time_unit == NULL) 
    {
        simulation_time_unit = new uvm_ml_time;
        int tmp_sim_u = vpi_get(vpiTimeUnit, NULL);
        map_time_unit(tmp_sim_u, simulation_time_unit);
    }

    static s_vpi_time t = {vpiScaledRealTime,0,0,0.0};

    vpi_get_time(NULL,&t);
    *units = simulation_time_unit->units;
    *value = t.real * simulation_time_unit->value;
}

class time_callbacks_container_class {
  std::set<vpiHandle> time_callbacks; // Set guarantees all the elements are unique (just in case, if we got the same cb handle twice)
public:
  time_callbacks_container_class() {}
  virtual ~time_callbacks_container_class() {
    std::set<vpiHandle>::iterator itr;
    for (itr = time_callbacks.begin();itr != time_callbacks.end(); ++itr) { 
        vpi_remove_cb(*itr);
    }
  }
  void add(vpiHandle cb) { time_callbacks.insert(cb); }
  void remove(vpiHandle cb) { time_callbacks.erase(cb); }
};

static time_callbacks_container_class time_callbacks_container;

static bool in_ncsim() {
    static bool is_cached = false;
    static bool in_ncsim_flag = false;

    if ( ! is_cached ) {
        s_vpi_vlog_info tmp_info;
        vpi_get_vlog_info(&tmp_info);
        
        std::string product(tmp_info.product);
        
        std::string ncsim_product("ncsim");
        std::string ncsim_product_64("ncsim(64)");
        in_ncsim_flag = ( product == ncsim_product || product == ncsim_product_64 );
        is_cached = true;
    }

    return in_ncsim_flag;
}

static PLI_INT32 vpi_time_callback (s_cb_data *cb_data)
{
    if ( cb_data ) {
        p_callback_data custom_struct_p = (p_callback_data)cb_data->user_data;
        assert(custom_struct_p != NULL);

        time_callbacks_container.remove(custom_struct_p->a_handle);

        if (in_ncsim()) {
            vpi_remove_cb(custom_struct_p->a_handle);

            if (current_time_cb_data_v.size() == 0) {
                static s_vpi_time time_delay = {vpiSimTime,0,0,0.0};
                vpiHandle uvm_ml_notify_time_e_handle = vpi_handle_by_name((char *) "uvm_ml_adapter_imp::uvm_ml_notify_time_e", NULL);
                assert (uvm_ml_notify_time_e_handle != 0);
                vpi_put_value(uvm_ml_notify_time_e_handle,NULL,&time_delay,vpiNoDelay);
            }
	    current_time_cb_data_v.push_back(custom_struct_p);
	}
        else {
            uvm_ml_time time;
            uvm_ml_uvmsv_get_current_simulation_time(&time.units, &time.value);
	    custom_struct_p->do_notify_time_cb(&time);
	}
    }
    return 0;
}

#define ROUNDING_EDGE 0.0000001
static uvm_ml_handle register_time_cb(int              adapter_id,
                               uvm_ml_time_unit time_unit, 
                               double           time_value, 
                               void *           user_data ) 
{
    FIND_GLOBAL_API(bp_utils_convert_uvm_ml_time_unit_to_c_str);
    
    static s_vpi_time time;
    s_vpi_time current_time;
    double time_normalized;

    if (time_unit != TIME_UNIT_UNDEFINED) 
    {
        // Requesting framework precision may differ from simulator precision.
        // Normalize the requested time to sim precision if time unit specified
        // Adjust time requested to add the minimum precision if fractional

        signed int converted_time_unit = convert_time_units_from_uvm_ml(time_unit);
        signed int global_time_precision=vpi_get(vpiTimePrecision,NULL);
        double normalize_coeff = (double)pow((double)10.0, converted_time_unit-global_time_precision);

        time_normalized = time_value*normalize_coeff;
        if(remainder(time_normalized, 1.) > ROUNDING_EDGE)
            time_normalized = ceil(time_normalized);
    }
    else
    {
        time_normalized = time_value;  // ie. already normalized to sim time
    }

    current_time.type = vpiSimTime;
    vpi_get_time((vpiHandle) NULL, &current_time);

    uint64_t int_time = static_cast<uint64_t>(time_normalized);
    int64_t int_curr_time; 
    int64_t delay;
    if (current_time.high == 0)
    {
        int_curr_time = current_time.low;
        delay = int_time - current_time.low;
    }
    else {
        int_curr_time = current_time.high;
        int_curr_time <<= 32;
        int_curr_time += current_time.low;
        delay = int_time - int_curr_time;
    }

    if (delay < 0L) {
        FIND_GLOBAL_API(bp_utils_uvm_ml_printf);
        if ( bp_utils_uvm_ml_printf_ptr)
            bp_utils_uvm_ml_printf_ptr(UVM_ML_ERROR, "SV C", 
                                       "In uvm_ml_sv_register_time_cb() >> The requested time %llu global simulation time units is in the past\n", int_time);
        return 0;
    }
    time.type = vpiSimTime;
    time.low = (PLI_UINT32) (delay & 0x00000000ffffffffL);
    time.high = (PLI_UINT32)(delay >> 32);
    time.real = 0.0;

    internal_printf(true, UVM_ML_DEBUG, "SV C", 
                    "Registering time callback requested by adapter %d for time %llu global simulation time units", 
                     adapter_id, 
                     int_time);

    s_cb_data data;

    data.time = &time;
    data.cb_rtn = vpi_time_callback;
    data.obj = NULL;
    data.reason = cbAfterDelay; // By definition, so far

    static int first_time = 1;
    if (first_time) {
        first_time = 0;
        if (in_ncsim()) {
            SV_REQUIRED_C_FUNC_COMMON(notify_time_cb_helper, 0);
            (*notify_time_cb_helper_ptr)();
	}
    }

    p_callback_data callback_data_p = (p_callback_data)calloc(1,sizeof(t_callback_data));
    callback_data_p->adapter_id = adapter_id;
    callback_data_p->user_data = user_data;

    data.user_data = (PLI_BYTE8*)callback_data_p;

    vpiHandle callback_handle;
    callback_handle = vpi_register_cb(&data);

    if ( callback_handle != NULL )
    {
        callback_data_p->a_handle = callback_handle;

        time_callbacks_container.add(callback_handle);

        return (uvm_ml_handle) callback_handle;
    }

    free(callback_data_p);
    return 0;
}

static int remove_time_cb(uvm_ml_handle cb_handle)
{
    internal_printf(true, UVM_ML_DEBUG,"SV C", 
                    "Removing time callback %ld", cb_handle);
    return vpi_remove_cb((vpiHandle) cb_handle);
}

static void notify_event
(
    const char *                          scope_name,
    const char *                          event_name,
    uvm_ml_event_notify_action            action,
    unsigned int                          stream_size,
    uvm_ml_stream_t                       stream,
    uvm_ml_time_unit UNUSED               time_unit,
    double           UNUSED               time_value
)
{
    SV_REQUIRED_C_FUNC_COMMON(notify_event, )
    (*notify_event_ptr)(scope_name, event_name, action, stream_size, stream);
}

static void notify_barrier
(
    const char *                          scope_name,
    const char *                          barrier_name,
    uvm_ml_barrier_notify_action          action,
    int                                   data,
    uvm_ml_time_unit UNUSED               time_unit,
    double           UNUSED               time_value
)
{
    SV_REQUIRED_C_FUNC_COMMON(notify_barrier, )
    (*notify_barrier_ptr)(scope_name, barrier_name, action, data);
}

/*
 * ========================
 * UVM-ML Command analysis
 * ========================
 */

static int receive_command
(
    const char *     command,
    bp_output_cb_t * output_cb,
    uvm_ml_time_unit time_unit,
    double           time_value
)
{
    string         command_s = command;
    stringstream   ss(command_s);
    string         token;
    vector<string> tokens;

    while (std::getline(ss, token, ' '))
        if (!token.empty())
            tokens.push_back(token);

    // Meantime the adapter supports only one command: "trace_register"
    if (tokens[0] == "trace_register" || 
        tokens[0] == "trace_register_tlm" || 
	tokens[0] == "uvm_ml_trace_register") {
	// The adapter does not check legality of syntax of the predefined command
        // The backplane is responsible for checking the syntax and printing the help for this command

        if (output_cb != NULL)
            internal_printf(false, UVM_ML_WARNING, "UVM SV C", "Currently command trace_register does not support re-direction of the output");    

        int turn_on;
        if (tokens[1] == "-on")
            turn_on = 1;
        else if (tokens[1] == "-off")
            turn_on = 0;
        else {
	    // See above: The adapter does not check legality of syntax of the predefined command
            return 0;
	}

        SV_REQUIRED_C_FUNC_COMMON(notify_trace_register_command, 0)
        (*notify_trace_register_command_ptr)(turn_on);
	return 1;

    }
    return -1; // Other commands return UNRECOGNIZED_COMMAND (-1) 
}


/* ===============================
   SV ADAPTER REQUIRED API SETUP
   =============================== */

static bp_frmw_c_api_struct * sv_get_required_api()
{
    bp_frmw_c_api_struct * required_api = new bp_frmw_c_api_struct();

    memset(required_api, '\0', sizeof(bp_frmw_c_api_struct));

    required_api->startup_ptr = startup_phases;

    required_api->construct_top_ptr = construct_top;

    required_api->transmit_phase_ptr = transmit_phase;
    required_api->notify_phase_ptr = notify_phase;
    required_api->notify_runtime_phase_ptr = notify_runtime_phase;
    required_api->phase_srv_notify_phase_done_ptr = phase_srv_notify_phase_done;
    required_api->phase_srv_start_ptr = phase_srv_start;

    required_api->set_trace_mode_ptr = set_trace_mode;
    required_api->find_connector_id_by_name_ptr = find_connector_id_by_name;
    required_api->get_connector_intf_name_ptr = get_connector_intf_name;
    required_api->get_connector_type_ptr = get_connector_type;
    required_api->try_put_uvm_ml_stream_ptr = tlm1_try_put;
    required_api->can_put_ptr = tlm1_can_put;

    /* the put family */

    required_api->put_uvm_ml_stream_ptr = 0;
    required_api->put_uvm_ml_stream_request_ptr = tlm1_put_request;

    /* the get family */

    required_api->get_uvm_ml_stream_ptr = 0;
    required_api->get_uvm_ml_stream_request_ptr = tlm1_get_request;
    required_api->try_get_uvm_ml_stream_ptr = tlm1_try_get;
    required_api->can_get_ptr = tlm1_can_get;
    required_api->get_requested_uvm_ml_stream_ptr = tlm1_get_requested;

    /* peek family */

    required_api->peek_uvm_ml_stream_ptr =  0;
    required_api->peek_uvm_ml_stream_request_ptr = tlm1_peek_request;
    required_api->try_peek_uvm_ml_stream_ptr = tlm1_try_peek;
    required_api->can_peek_ptr = tlm1_can_peek;
    required_api->peek_requested_uvm_ml_stream_ptr = tlm1_peek_requested;

    required_api->transport_uvm_ml_stream_ptr =  0;
    required_api->transport_uvm_ml_stream_request_ptr = tlm1_transport_request;
    required_api->transport_response_uvm_ml_stream_ptr = tlm1_transport_response;
    required_api->nb_transport_uvm_ml_stream_ptr = tlm1_nb_transport;

    /* analysis if */

    required_api->write_uvm_ml_stream_ptr = tlm_write;

    required_api->notify_end_blocking_ptr = tlm_notify_end_blocking;
    required_api->tlm2_b_transport_ptr = 0; // True blocking is currently not supported 
    required_api->tlm2_b_transport_request_ptr = tlm2_b_transport_request;
    required_api->tlm2_b_transport_response_ptr = tlm2_b_transport_response;
    required_api->tlm2_nb_transport_fw_ptr = tlm2_nb_transport_incoming;
    required_api->tlm2_nb_transport_bw_ptr = tlm2_nb_transport_incoming;
    required_api->tlm2_transport_dbg_ptr = 0;
    required_api->tlm2_turn_off_transaction_mapping_ptr = tlm2_turn_off_transaction_mapping;
    required_api->external_connect_ptr = external_connect;

    required_api->synchronize_ptr = 0;

    required_api->create_child_junction_node_ptr = create_child_junction_node;

    required_api->notify_resource_ptr = notify_resource;

    required_api->notify_config_ptr = notify_config;

    required_api->notify_event_ptr = notify_event;

    required_api->notify_barrier_ptr = notify_barrier;

    // Time-wakeup service

    required_api->wakeup_srv_register_time_cb_ptr = register_time_cb;
    required_api->wakeup_srv_remove_time_cb_ptr = remove_time_cb;
    required_api->notify_time_cb_ptr = NULL;

    // Stopper service
    required_api->stopper_srv_stop_request_ptr = stopper_srv_stop_request;
    required_api->phase_srv_check_phase_ptr = phase_srv_check_phase;
    required_api->phase_srv_check_future_phase_ptr = phase_srv_check_future_phase;

    // Debug print tree
    required_api->get_num_children_ptr = get_num_children;
    required_api->get_child_name_ptr = get_child_name;
    required_api->get_component_type_name_ptr = get_component_type_name;
    required_api->is_port_ptr = is_port;

    // Print service
    required_api->print_srv_print_request_ptr = print_srv_print_request;

    // Notify command
    required_api->notify_command_ptr = receive_command;

    return required_api;
}

}  // extern C

// ================================================
//
//  U V M - M L  S V   P R O V I D E D   A P I 
//
// ================================================

uvm_ml_master_get_current_simulation_time_type * master_get_current_simulation_time_ptr;

// TBD: Use the tray - meantime the file is linked together with the
// backplane and hence we can call the backplane functions directly

static bool uvm_sv_framework_registered = false;
static int  uvm_sv_framework_id = -1;

static bp_api_struct * bpProvidedAPI = NULL;

#define BP(f) (*bpProvidedAPI->f##_ptr)

bp_frmw_c_api_struct *  sv_get_required_api ();

extern "C" {

///////////////////////////////////////////////////////////////////////
//
// 

void uvm_ml_uvmsv_set_trace_mode(int mode)
{
    BP(set_trace_mode)(mode);
}

static PLI_INT32 uvm_ml_uvmsv_handle_reset(p_cb_data cb_data);
static vpiHandle start_of_reset_handle = NULL;



// return -1 for failure
int uvm_ml_uvmsv_register_framework()
{

    if (uvm_sv_framework_registered == true) {
      //assert (uvm_sv_framework_id != (-1));
      if(uvm_sv_framework_id == -1) {
	cout << "*ERROR: UVM-SV framework was not registered properly" << endl;
	return -1;
      };
        return uvm_sv_framework_id;
    }

    
    bp_api_struct* (*bp_get_provided_tray_ptr)();

    bp_get_provided_tray_ptr = (bp_api_struct* (*)())GlobalDlsym("bp_get_provided_tray");

    if ( bp_get_provided_tray_ptr ) { 
        bpProvidedAPI = bp_get_provided_tray_ptr();
    }

    //assert (bpProvidedAPI != NULL);
    if(bpProvidedAPI == NULL) {
      cout << "*ERROR: UVM-SV framework cannot get the provided API from the backplane" << endl;
      return -1;
    };

    master_get_current_simulation_time_ptr = uvm_ml_uvmsv_get_current_simulation_time;

    char * frmw_ids[3] = {(char *)"UVMSV", (char *)"SV", (char *)""};
  
    uvm_sv_framework_id = BP(register_framework)((char *) "UVM SV", frmw_ids, sv_get_required_api());

    uvm_sv_framework_registered = true;

    if ( in_ncsim() ) {  // Reset support is Incisive-only for the time being
     /* Register the reset callback */
        
        if ( start_of_reset_handle == NULL ) { // avoid re-registering if already registered
            s_cb_data start_of_reset_data;
            start_of_reset_data.cb_rtn = uvm_ml_uvmsv_handle_reset;
            start_of_reset_data.reason = cbStartOfReset;
            start_of_reset_data.obj = NULL;
            start_of_reset_data.user_data = NULL;
            
            start_of_reset_handle = vpi_register_cb(&start_of_reset_data);
            if ( start_of_reset_handle == NULL ) { 
                fprintf(stderr, "ERROR: Failed to register start of reset handler\n");
            }
        }
    }

    return uvm_sv_framework_id;
}

int uvm_ml_uvmsv_notify_phase(const char *      phase_group,
                           const char *      phase_name,
                           unsigned int      phase_action)
{
    return BP(notify_phase)(uvm_sv_framework_id, phase_group, phase_name, uvm_ml_phase_action(phase_action));
}

int uvm_ml_uvmsv_transmit_phase(const char * target_frmw_ind,
                             int          target_id,
                             const char * phase_group,
                             const char * phase_name,
                             unsigned int phase_action)
{
    return BP(transmit_phase)(uvm_sv_framework_id, target_frmw_ind, target_id, phase_group, phase_name, uvm_ml_phase_action(phase_action));
}


void uvm_ml_uvmsv_notify_runtime_phase(const char *    phase_group,
                                    const char *   phase_name,
                                    unsigned int   phase_action,
                                    unsigned int * participate)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    BP(notify_runtime_phase)(uvm_sv_framework_id, phase_group, phase_name, uvm_ml_phase_action(phase_action), time_unit, time_value, participate);
}

int uvm_ml_uvmsv_notify_phase_done(char * phase_group,
                                char * phase_name)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);
    BP(notify_phase_done)(uvm_sv_framework_id, phase_group, phase_name, time_unit, time_value);
    return 0;
}

void uvm_ml_uvmsv_register_srv_providers(bp_srv_provider_struct * provider)
{
    BP(register_srv_providers)(uvm_sv_framework_id, provider);
}

unsigned uvm_ml_uvmsv_connect_ports(const char *   path1,
                                 const char *   path2)
{
    return BP(connect)(uvm_sv_framework_id, path1, path2);
}

int uvm_ml_uvmsv_get_connector_size(int connector_id)
{
    return BP(get_connector_size)(uvm_sv_framework_id, connector_id);
}

uvm_ml_type_id_t uvm_ml_uvmsv_get_type_id_from_name(const char * type_name)
{
    return BP(get_type_id_from_name)(uvm_sv_framework_id, type_name);
}

const char* uvm_ml_uvmsv_get_type_name_from_id(uvm_ml_type_id_t type_id)
{
    return BP(get_type_name_from_id)(uvm_sv_framework_id, type_id);
}

int uvm_ml_uvmsv_nb_put(unsigned int    producer_id,
                     unsigned int    stream_size,
                     uvm_ml_stream_t stream)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(nb_put)(uvm_sv_framework_id, producer_id, stream_size, stream, time_unit, time_value);
}

int uvm_ml_uvmsv_can_put(unsigned int port_id)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(can_put)(uvm_sv_framework_id, port_id, time_unit, time_value);
}

int uvm_ml_uvmsv_nb_get(int             producer_id,
                     unsigned int *  stream_size_ptr,
                     uvm_ml_stream_t stream)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(nb_get)(uvm_sv_framework_id, producer_id, stream_size_ptr, stream, time_unit, time_value);
}

int uvm_ml_uvmsv_nb_peek(unsigned int    producer_id,
                      unsigned int *  stream_size_ptr,
                      uvm_ml_stream_t stream)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(nb_peek)(uvm_sv_framework_id, producer_id, stream_size_ptr, stream, time_unit, time_value);
}

int uvm_ml_uvmsv_can_get(unsigned int port_id)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(can_get)(uvm_sv_framework_id, port_id, time_unit, time_value);
}

int uvm_ml_uvmsv_can_peek(unsigned int port_id)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(can_peek)(uvm_sv_framework_id, port_id, time_unit, time_value);
}

unsigned int uvm_ml_uvmsv_get_requested(int             producer_id,
                                     int             call_id,
                                     uvm_ml_stream_t stream)
{
    return BP(get_requested)(uvm_sv_framework_id, producer_id, call_id, stream);
}

int uvm_ml_uvmsv_nb_transport(unsigned        connector_id, 
                           unsigned        req_stream_size, 
                           uvm_ml_stream_t req_stream,
                           unsigned int  * rsp_stream_size_ptr, 
                           uvm_ml_stream_t rsp_stream
)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(nb_transport)(uvm_sv_framework_id, connector_id, req_stream_size, req_stream, rsp_stream_size_ptr, rsp_stream, time_unit, time_value);
}

unsigned int uvm_ml_uvmsv_transport_response(int              producer_id,
                                          int              call_id,
                                          uvm_ml_stream_t  rsp_stream) 
{ 
    return BP(transport_response)(uvm_sv_framework_id, producer_id, call_id, rsp_stream);
}

void uvm_ml_uvmsv_write(unsigned        connector_id, 
                     unsigned        stream_size, 
                     uvm_ml_stream_t stream)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    BP(write)(uvm_sv_framework_id, connector_id, stream_size, stream, time_unit, time_value);
}

void uvm_ml_uvmsv_notify_end_blocking(unsigned callback_adapter_id, 
                                   unsigned call_id)
{ 
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    BP(notify_end_blocking)(uvm_sv_framework_id, callback_adapter_id, call_id, time_unit, time_value);
}

int uvm_ml_uvmsv_put_request(unsigned        port_connector_id, 
                          unsigned        call_id, 
                          unsigned        stream_size, 
                          uvm_ml_stream_t stream,
                          unsigned *      done
) 
{ 
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    void(BP(request_put)(uvm_sv_framework_id, port_connector_id, call_id, stream_size, stream, done, &time_unit, &time_value));

    // FIXME - implement good handling of disabled tasks later
    // svIsDisabledState is not implemented in VCS
    //int result = svIsDisabledState();
    //return result;
    return 0;
}

int uvm_ml_uvmsv_get_request(unsigned            port_connector_id, 
                          unsigned            call_id,
                          unsigned *          stream_size_ptr,
                          uvm_ml_stream_t     stream,
                          unsigned *          done) 
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    void(BP(request_get)(uvm_sv_framework_id, port_connector_id, call_id, stream_size_ptr, stream, done, &time_unit, &time_value));

    // FIXME - implement good handling of disabled tasks later
    // svIsDisabledState is not implemented in VCS
    //int result = svIsDisabledState();
    //return result;
    return 0;
}

int uvm_ml_uvmsv_peek_request(unsigned            port_connector_id, 
                           unsigned            call_id,
                           unsigned *          stream_size_ptr,
                           uvm_ml_stream_t     stream,
                           unsigned *          done) 
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    void(BP(request_peek)(uvm_sv_framework_id, port_connector_id, call_id, stream_size_ptr, stream, done, &time_unit, &time_value));

    // FIXME - implement good handling of disabled tasks later
    // svIsDisabledState is not implemented in VCS
    //int result = svIsDisabledState();
    //return result;
    return 0;
}

int uvm_ml_uvmsv_transport_request(unsigned            connector_id, 
                                unsigned            call_id, 
                                unsigned            req_stream_size, 
                                uvm_ml_stream_t     req_stream,
                                unsigned*           rsp_stream_size, 
                                uvm_ml_stream_t     rsp_stream,
                                /* Argument done is needed because a calling adapter does not know whether 
                                   the called adapter supports blocking or fake blocking protocol */
                                unsigned *          done)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    void(BP(request_transport)(uvm_sv_framework_id,  connector_id, call_id, req_stream_size, req_stream, rsp_stream_size, rsp_stream, done, &time_unit, &time_value));

    // FIXME - implement good handling of disabled tasks later
    // svIsDisabledState is not implemented in VCS
    //int result = svIsDisabledState();
    //return result;
    return 0;
}

uvm_ml_tlm_sync_enum uvm_ml_uvmsv_tlm2_nb_transport_fw(unsigned           initiator_connector_id,
                                                    unsigned *         stream_size,
                                                    uvm_ml_stream_t    stream,
                                                    uvm_ml_tlm_phase * phase,
                                                    unsigned int       transaction_id,
                                                    uvm_ml_time_unit * delay_unit,
                                                    double *           delay_value)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);


    uvm_ml_stream_t stream_ptr = stream;

    uvm_ml_tlm_sync_enum result = BP(nb_transport_fw)(uvm_sv_framework_id, initiator_connector_id, stream_size, &stream_ptr, 
                                                   phase, transaction_id, delay_unit, delay_value, time_unit, time_value);

    if (stream_ptr != stream)
        memcpy (stream, stream_ptr, (*stream_size)*sizeof(int));

    return result;
}

uvm_ml_tlm_sync_enum uvm_ml_uvmsv_tlm2_nb_transport_bw(unsigned           target_connector_id,
                                                    unsigned *         stream_size,
                                                    uvm_ml_stream_t    stream,
                                                    uvm_ml_tlm_phase * phase,
                                                    unsigned int       transaction_id,
                                                    uvm_ml_time_unit * delay_unit,
                                                    double *           delay_value)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    uvm_ml_stream_t stream_ptr = stream;

    uvm_ml_tlm_sync_enum result = BP(nb_transport_bw)(uvm_sv_framework_id, target_connector_id, stream_size, &stream_ptr, 
                                                     phase, transaction_id, delay_unit, delay_value, time_unit, time_value);

    if (stream_ptr != stream)
        memcpy (stream, stream_ptr, (*stream_size)*sizeof(int));

    return result;
}

int uvm_ml_uvmsv_tlm2_b_transport_request(unsigned         initiator_connector_id, 
                                       unsigned         call_id,
                                       unsigned *       stream_size,
                                       uvm_ml_stream_t  stream,
                                       uvm_ml_time_unit delay_unit,
                                       double           delay_value,
                                       unsigned *       done)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    uvm_ml_stream_t stream_ptr = stream;
    int res = BP(request_b_transport_tlm2)(uvm_sv_framework_id, initiator_connector_id, call_id, stream_size, &stream_ptr, 
                                          delay_unit, delay_value, done, time_unit, time_value);

    if (*done && (stream_ptr != stream)) {
        memcpy (stream, stream_ptr, (*stream_size)*sizeof(int));
    }
    return res;  
}

int uvm_ml_uvmsv_tlm2_b_transport_response(unsigned        initiator_id, 
                                        unsigned        call_id,
                                        unsigned *      stream_size,
                                        uvm_ml_stream_t stream)
{
    return BP(b_transport_tlm2_response)(uvm_sv_framework_id, initiator_id, call_id, stream_size, stream);
}

void uvm_ml_uvmsv_turn_off_transaction_mapping(const char* socket_name)
{
    BP(turn_off_transaction_mapping)(uvm_sv_framework_id, socket_name);
}

unsigned int uvm_ml_uvmsv_assign_transaction_id()
{
    return BP(assign_transaction_id)(uvm_sv_framework_id);
}

static int array_converter (const svOpenArrayHandle sv_array, char ***c_array_ptr) {
        
    int number_of_elements = 0;

    if (sv_array != NULL) { 
        int low = svLow(sv_array,1);
        int high = svHigh(sv_array,1);
        number_of_elements = high-low+1;

        if (number_of_elements > 0) {

	    *c_array_ptr = (char **) malloc (number_of_elements * sizeof (char *));

            char ** str;
            for (int i = 0; i < number_of_elements ; ++i) {
                str = (char**)svGetArrElemPtr1(sv_array,low+i);
                if (str != NULL)
                    (*c_array_ptr)[i] = *str;
                if (str  == NULL || (*str)[0] == '\000') {
                    break;
                }
            }
        }
    }
    return number_of_elements;
}

int uvm_ml_uvmsv_run_test(int framework_id, const svOpenArrayHandle svTops, char * svTest, int * result) 
{
    int     top_n = 0;
    char ** c_tops = NULL;

    top_n = array_converter(svTops, &c_tops);
    
    *result = BP(run_test)(framework_id, top_n, c_tops, svTest);

    if (c_tops)
        free (c_tops);

    return 0; // SV import task must return value
}

int uvm_ml_uvmsv_add_root_node
(
    unsigned         top_level_id,
    const char *     component_name,
    const char *     instance_name
)
{
    return BP(add_root_node)(uvm_sv_framework_id, top_level_id, component_name, instance_name);
}

int uvm_ml_uvmsv_synchronize()
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    BP(synchronize)(uvm_sv_framework_id, time_unit, time_value);
    return 0;
}

int uvm_ml_uvmsv_set_match_types(const char* type1, 
                              const char* type2)
{
    return BP(set_match_types)(uvm_sv_framework_id, type1, type2);
}

int uvm_ml_uvmsv_create_child_junction_node(const char *   target_framework_indicator,
                                         const char *   component_type_name,
                                         const char *   instance_name,
                                         const char *   parent_full_name,
                                         int            parent_junction_node_id)
{
    return BP(create_child_junction_node)(uvm_sv_framework_id, target_framework_indicator, component_type_name,
                                          instance_name, parent_full_name, parent_junction_node_id);
}

int uvm_ml_uvmsv_notify_resource (uvm_ml_resource_notify_action action,
                               const char *                  item_scope,
                               const char *                  item_name,
                               const char *                  accessor_name,
                               unsigned int                  stream_size,
                               uvm_ml_stream_t               stream)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    return BP(notify_resource)(uvm_sv_framework_id, action, item_scope, item_name, accessor_name, stream_size, stream, time_unit, time_value);
}

void uvm_ml_uvmsv_notify_config (const char *    cntxt,
                              const char *    instance_name,
                              const char *    field_name,
                              unsigned int    stream_size,
                              uvm_ml_stream_t stream)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    BP(notify_config)(uvm_sv_framework_id, cntxt, instance_name, field_name, stream_size, stream, time_unit, time_value);
}

void uvm_ml_uvmsv_notify_event ( const char *    scope_name,
                              const char *    event_name,
                              uvm_ml_event_notify_action    action,
                              unsigned int    stream_size,
                              uvm_ml_stream_t stream)

{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    BP(notify_event)(uvm_sv_framework_id, scope_name, event_name, action, stream_size, stream, time_unit, time_value);

}

void uvm_ml_uvmsv_notify_barrier ( const char *    scope_name,
                                const char *    barrier_name,
                                uvm_ml_barrier_notify_action    action,
                                int             data)
{
    uvm_ml_time_unit time_unit;
    double           time_value;

    uvm_ml_uvmsv_get_current_simulation_time(&time_unit, &time_value);

    BP(notify_barrier)(uvm_sv_framework_id, scope_name, barrier_name, action, data, time_unit, time_value);

}

void uvm_ml_uvmsv_set_pack_max_size(int size)
{
    BP(set_pack_max_size)(uvm_sv_framework_id, size);
}

int uvm_ml_uvmsv_get_pack_max_size()
{
    return BP(get_pack_max_size)(uvm_sv_framework_id);
}

/* ======================================
   TIME CALLBACK NOTIFICATION FUNCTIONS
   ===================================== */

void t_callback_data::do_notify_time_cb(uvm_ml_time * time)
{
    FIND_GLOBAL_API(bp_utils_convert_uvm_ml_time_unit_to_c_str);
    if ( bp_utils_convert_uvm_ml_time_unit_to_c_str_ptr) { 
        internal_printf(true, UVM_ML_DEBUG, "SV C", 
                        "Notifying framework %d of time callback at %f %s\n",
                        adapter_id,
                        time->value,
                        bp_utils_convert_uvm_ml_time_unit_to_c_str_ptr(time->units));
    }

    BP(notify_time_cb)(uvm_sv_framework_id, adapter_id, time->units, time->value, user_data);
    free (this);
}

void uvm_ml_uvmsv_dpi_context_notify_time_cb()
{
    uvm_ml_time time;
    uvm_ml_uvmsv_get_current_simulation_time(&time.units, &time.value);

    std::for_each(current_time_cb_data_v.begin(), current_time_cb_data_v.end(), std::bind2nd(std::mem_fun(&t_callback_data::do_notify_time_cb), &time));
    current_time_cb_data_v.resize(0);
}

static PLI_INT32 uvm_ml_uvmsv_handle_reset(p_cb_data cb_data) {
    internal_printf(true, UVM_ML_DEBUG,"SV C","UVM-ML-OA INFO: Resetting the System Verilog adapter\n");

    current_time_cb_data_v.resize(0); // FIXME - memory leak?
    uvm_sv_framework_registered = false;
    uvm_sv_framework_id = -1;
    bpProvidedAPI = NULL;
    sv_blocking_calls_helper_ptr = 0;

    std::for_each(list_of_dpi_exports.begin(), list_of_dpi_exports.end(), set_to_zero);
    list_of_dpi_exports.resize(0);

    std::for_each(list_of_backplane_exports.begin(), list_of_backplane_exports.end(), set_to_zero);
    list_of_backplane_exports.resize(0);
    return 0;
}

}  // extern C


static void __attribute__((destructor)) release_vpi_callbacks() {
    if ( start_of_reset_handle )
        vpi_remove_cb(start_of_reset_handle);
}
