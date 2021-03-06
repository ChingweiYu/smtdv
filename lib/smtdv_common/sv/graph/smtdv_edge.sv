
`ifndef __SMTDV_EDGE_SV__
`define __SMTDV_EDGE_SV__

typedef class smtdv_node;
typedef class smtdv_graph;
//typedef struct smtdv_attr;

/*
* smtdv_edge a basic edge type to handle source node and sink node
*
* @class smtdv_edge#(SOURCE, SINK)
*/
class smtdv_edge#(
  type SOURCE = smtdv_node,
  type SINK = SOURCE
  ) extends
  uvm_object;

  typedef smtdv_attr attr_t;
  typedef smtdv_edge#(SOURCE, SINK) edge_t;
  typedef smtdv_node node_t;
  typedef smtdv_graph graph_t;
  typedef uvm_object bgraph_t;

  SOURCE source; // map source node ptr
  SINK sink;     // map sink node ptr

  int sourceid;  // map source id
  int sinkid;    // map sink id

  attr_t attr;
  graph_t graph;

  bit has_finalize = FALSE;

  `uvm_object_param_utils_begin(edge_t)
    `uvm_field_int(sourceid, UVM_ALL_ON)
    `uvm_field_int(sinkid, UVM_ALL_ON)
    `uvm_field_object(source ,UVM_ALL_ON)
    `uvm_field_object(sink, UVM_ALL_ON)
    `uvm_field_object(graph, UVM_ALL_ON)
  `uvm_object_utils_end

  function new(string name = "smtdv_edge");
    super.new(name);
  endfunction : new

  extern virtual function void register(uvm_object parent=null);
  extern virtual function void finalize();
  extern virtual function void add_source(int isourceid, SOURCE node); // add source id and ptr
  extern virtual function void add_sink(int isinkid, SINK node); // add sink id and ptr
  extern virtual function void del_source();
  extern virtual function void del_sink();
  extern virtual function void add_attr(attr_t iattr);
  extern virtual function attr_t get_attr();
  extern virtual function void update_attr(attr_t iattr);
  extern virtual function void dump();

endclass : smtdv_edge

/*
* cast from object to smtdv_graph
*/
function void smtdv_edge::register(uvm_object parent=null);
  if (!$cast(graph, parent))
    `uvm_error("SMTDV_UCAST_GRAPH",
        {$psprintf("UP CAST TO SMTDV GRAPH FAIL")})

endfunction : register

/*
* finalize while all settings are finished
*/
function void smtdv_edge::finalize();
  has_finalize = TRUE;
endfunction : finalize

/*
* add source node ptr and id
*/
function void smtdv_edge::add_source(int isourceid, SOURCE node);
  if(has_finalize) return;
  sourceid = isourceid;

  if (!$cast(source, node))
    `uvm_error("SMTDV_DCAST_NODE",
        {$psprintf("DOWN CAST TO SMTDV NODE FAIL")})

endfunction : add_source

/*
* add sink node ptr and id
*/
function void smtdv_edge::add_sink(int isinkid, SINK node);
  if(has_finalize) return;
  sinkid = isinkid;

  if (!$cast(sink, node))
    `uvm_error("SMTDV_DCAST_NODE",
        {$psprintf("DOWN CAST TO SMTDV NODE FAIL")})

endfunction : add_sink

/*
* del_source ptr as null and reset id=-1
*/
function void smtdv_edge::del_source();
  if(has_finalize) return;
  sourceid = -1;
  if (source)
    source = null;
endfunction : del_source

/*
* del_sink ptr as null and reset id=-1
*/
function void smtdv_edge::del_sink();
  if(has_finalize) return;
  sinkid = -1;
  if (sink)
    sink = null;
endfunction : del_sink

/*
* add default attr
*/
function void smtdv_edge::add_attr(smtdv_edge::attr_t iattr);
  if(has_finalize) return;
  attr = iattr;
endfunction : add_attr

/*
* update attr
*/
function void smtdv_edge::update_attr(smtdv_edge::attr_t iattr);
  attr = iattr;
endfunction : update_attr

/*
* get attr
*/
function smtdv_edge::attr_t smtdv_edge::get_attr();
  return attr;
endfunction : get_attr

function void smtdv_edge::dump();
endfunction : dump

`endif // __SMTDV_EDGE_SV__
