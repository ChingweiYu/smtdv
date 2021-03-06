
`ifndef __SMTDV_CFG_LABEL_SV__
`define __SMTDV_CFG_LABEL_SV__

typedef class smtdv_sequence_item;
typedef class smtdv_cfg;
typedef class smtdv_component;
typedef class smtdv_run_label;
typedef class smtdv_system_table;

/*
* smtdv register updated cfg label while after setting reg cfg
*
* @class smtdv_cfg_label#
*/
class smtdv_cfg_label#(
  ADDR_WIDTH = 14,
  DATA_WIDTH = 32,
  type CFG = smtdv_cfg,          // need to update cfg
  type CMP = smtdv_component,     // belong to which cmp
  type T1 = smtdv_sequence_item#(ADDR_WIDTH, DATA_WIDTH)
  ) extends
  smtdv_run_label;

  typedef smtdv_cfg_label#(ADDR_WIDTH, DATA_WIDTH, CFG, CMP, T1) label_t;

  typedef struct {
    int ucid;
    int left;   // left Boundary
    int right;  // right Boundary
    int def;    // default value
    int val;    // value
    string desc;
  } col_t;

  typedef struct {
    bit match;
    bit require;
    int depends[$]; // depend on row
    bit visit;
    bit clear;
   } attr_t;

  typedef struct {
    int urid;
    attr_t attr;
    col_t cols[$];
    bit [DATA_WIDTH-1:0] data; // unpackage data
    bit [ADDR_WIDTH-1:0] addr; // map addr
    trs_type_t trs;
  } row_t;

  typedef struct {
    int ufid;
    row_t rows[$];
    CMP cmp;
    CFG cfg;
    string desc; // frame description
  } cfgtb_t;

  typedef struct {
    int ufid;
  } test_t;

  test_t test;

  typedef smtdv_base_item bitem_t;

  T1 item;

  cfgtb_t cfgtb;
  row_t row;
  col_t col;

  bit has_pass;

  `uvm_object_param_utils_begin(label_t)
  `uvm_object_utils_end

  function new(string name = "smtdv_cfg_label", uvm_component parent=null);
    super.new(name, parent);
  endfunction : new

  extern virtual function void pre_do();
  extern virtual function void mid_do();
  extern virtual function void post_do();
  extern virtual function void flush();
  extern virtual function void update_item(bitem_t bitem);
  extern virtual function void populate();

endclass : smtdv_cfg_label

/*
*  that should be called at monitor thread when latest even trigger
*/
function void smtdv_cfg_label::update_item(bitem_t bitem);
  super.update_item(bitem);

  has_ready = FALSE;
  if ($cast(item, bitem))
    has_ready = TRUE;

endfunction : update_item

/*
* update lookup table while item has updated
*/
function void smtdv_cfg_label::pre_do();
  int dep;
  bit go = TRUE;
  string cmp_name;
  super.pre_do();

  cmp_name = this.cmp.get_full_name();
  if (!cmp_name.compare(item.cmp.get_full_name()))
    return;

  foreach(cfgtb.rows[i]) begin
    row = cfgtb.rows[i];

    if (row.trs != item.trs_t)
      continue;

    foreach(item.addrs[k]) begin

      if (item.addrs[k] == row.addr) begin
        go = TRUE;

        foreach(row.attr.depends[d]) begin
          dep = row.attr.depends[d];
          if (dep inside {[0:cfgtb.rows.size()]})
            if (cfgtb.rows[dep].attr.visit == FALSE)
              go = FALSE;
        end

        if (go) begin
          if (row.attr.match) begin
            if (item.data_beat[k] == row.data)
              row.attr.visit = TRUE;
          end
          else begin
            row.data = item.unpack_data(k);
            row.attr.visit = TRUE;
          end
        end

      end
    end

   cfgtb.rows[i] = row;
  end
endfunction : pre_do


function void smtdv_cfg_label::mid_do();
  super.mid_do();
  has_pass = TRUE;

  foreach(cfgtb.rows[i]) begin
    row = cfgtb.rows[i];
    if (row.attr.visit == FALSE)
      has_pass = FALSE;

  end
endfunction : mid_do


function void smtdv_cfg_label::post_do();
  super.post_do();

  if (has_pass) begin
    callback();
    populate();
    flush();
  end
endfunction : post_do


function void smtdv_cfg_label::populate();
  super.populate();
  smtdv_system_table::populate_label('{
      stime: $time,
      abbr_name: this.cmp.get_full_name(),
      desc: cfgtb.desc
  });
endfunction : populate


function void smtdv_cfg_label::flush();
  super.flush();

  foreach(cfgtb.rows[i]) begin
    row = cfgtb.rows[i];
    if (row.attr.clear)
      row.attr.visit = FALSE;

    cfgtb.rows[i] = row;
  end
endfunction : flush

`endif // __SMTDV_CFG_LABEL_SV__
