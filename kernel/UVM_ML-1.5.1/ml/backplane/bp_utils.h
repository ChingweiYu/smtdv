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

// Backplane internal utilities header File
// bp_utils.h

#ifndef BP_UTILS_H
#define BP_UTILS_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#include "bp_common_c.h"

#define MLUVM01   7118
#define MLUVM02   7119
#define MLUVM03   7120
#define MLUVM04   7121
#define MLUVM05   7122
#define MLUVM06   7123
#define MLUVM07   7124
#define MLUVM08   7125
#define MLUVM09   7126
#define MLUVM10   7127
#define MLUVM11   7128
#define MLUVM12   7129
#define MLUVM13   7130
#define MLUVM14   7131
#define MLUVM15   7132
#define MLUVM16   7133
#define MLUVM17   7134
#define MLUVM18   7135

/* Temporarily cannot add new error codes: #define MLUVM17   7132 */

using namespace std;

namespace Bp 
{

class BpStringComparer 
{
private:
    string x;

public:
    BpStringComparer (const string & s) { x = s; }
    ~BpStringComparer() {}
    string X() { return x; }
    bool operator ()(const string & s) { return (s == x); }
};

template <typename T1,class T2> T2 * BpGetMapSecondObject (map <T1,T2 *> & mapInst, const T1 & key)
{
    typename map <T1, T2 *>::iterator it = mapInst.find(key);
    return ((it == mapInst.end()) ? NULL : it->second);
}

template <typename TargetType, class ComparerType, typename KeyType> TargetType * BpVectorObjectFinder(vector<TargetType *> & vectorInst, KeyType wanted)
{
    typename vector<TargetType *>::iterator it = std::find_if (vectorInst.begin(), vectorInst.end(), ComparerType(wanted));
    if (it != vectorInst.end())
        return (*it);
    else
        return NULL;
}

bool BpPrefixCompare(const string & prefixStr, const string & x, const string separators = "./");

const string & BpExtractBaseName(const string & typeName);

string GetToken(const string &x, int nToken=0);

class BpUtils {
 public:
    static const char * convert_uvm_ml_time_unit_to_c_str(uvm_ml_time_unit unit) {
        switch ( unit) {
	    case TIME_UNIT_FS:
	        return "fs";
            case TIME_UNIT_PS:
                return "ps";
	    case TIME_UNIT_NS:
                return "ns";
            case TIME_UNIT_US:
                return "us";
            case TIME_UNIT_MS:
                return "ms";
            case TIME_UNIT_SEC:
                return "sec";
            case TIME_UNIT_MIN:
                return "min";
            case TIME_UNIT_HR:
                return "hr";
            default:
                return "unknown"; /* or throw an exception here */
        }
    }

    static void print_format_va_list(int level, const char * caller_name, const char *string, va_list & args) {

        fflush (stderr);
        switch(level)
        {
            case UVM_ML_ERROR:
                fprintf (stderr, "UVM-ML %s ERROR >> ", caller_name);
                break;
            case UVM_ML_WARNING:
                 fprintf (stderr, "UVM-ML %s WARNING >> ", caller_name);
                 break;
            case UVM_ML_DEBUG:
                 fprintf (stderr, "UVM-ML %s DEBUG >> ", caller_name);
                 break;
        };
        vfprintf (stderr, string, args);
        fprintf (stderr, "\n");
    } 

    static void uvm_ml_printf(int level, const char * caller_name, const char *string,...) {
        va_list args;

        va_start (args, string);
        print_format_va_list(level, caller_name, string, args);
        va_end(args);
    }
};

}   // namespace Bp


typedef void (uvm_ml_master_get_current_simulation_time_type)
(
    uvm_ml_time_unit * units, 
    double *           value
);

#ifdef __GNUC__
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

#endif /* BP_UTILS_H */
