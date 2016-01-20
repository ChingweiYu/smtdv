----------------------------------------------------------------------
   (C) Copyright 2013 Cadence Design Systems, Incorporated
   (C) Copyright 2013 Advanced Micro Devices, Inc.
   All Rights Reserved Worldwide

   Licensed under the Apache License, Version 2.0 (the
   "License"); you may not use this file except in
   compliance with the License.  You may obtain a copy of
   the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in
   writing, software distributed under the License is
   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.  See
   the License for the specific language governing
   permissions and limitations under the License.
----------------------------------------------------------------------

test: 
	Construct a UVM-ML environment with e top and a SystemVerilog 
        subtree. Analysis ports are connected, sending packets 
        in both directions.

environment: 
	IES: Native IES SV working with Specman e
	VCS: Native VCS SV working with Specman e
	QUESTA: Native QUESTA SV working with Specman e

to run: Make sure the UVM-ML OA package is installed as described in 
        $UVM_ML_HOME/ml/README_INSTALLATION.txt
	% make ies
        or
        % make vcs
        or
        % make questa