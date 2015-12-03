`ifndef __APB_BACKDOOR_SV__
`define __APB_BACKDOOR_SV__

class apb_mem_backdoor #(
  ADDR_WIDTH = 14,
  DATA_WIDTH = 32
) extends
  smtdv_backdoor;

  `uvm_component_param_utils_begin(`APB_MEM_BACKDOOR)
  `uvm_component_utils_end

  function new(string name = "apb_mem_backdoor", uvm_component parent=null);
    super.new(name, parent);
  endfunction

  virtual function string gen_query_cmd(string table_nm, string map, ref `APB_ITEM item);
    string cmd;
    case(map)
      "LAST_WR_TRX": begin
          cmd = {$psprintf("SELECT * FROM %s WHERE dec_rw=%d AND dec_addr>=%d AND dec_addr<%d ORDER BY dec_ed_cyc DESC limit %d;",
            table_nm, WR, item.addrs[item.addr_idx], item.addrs[item.addr_idx]+item.offset, item.offset)};
        end
      "FRIST_WR_TRX": begin
          cmd = {$psprintf("SELECT * FROM %s WHERE dec_rw=%d AND dec_addr>=%d AND dec_addr<%d ORDER BY dec_ed_cyc ASC limit %d;",
            table_nm, WR, item.addrs[item.addr_idx], item.addrs[item.addr_idx]+item.offset, item.offset)};
      end
      // extend your query ...
      default: begin
          cmd = {$psprintf("SELECT * FROM %s ORDER BY dec_ed_cyc ASC;", table_nm)};
      end
    endcase
     `uvm_info(get_full_name(), {$psprintf("get query backdoor cmd\n%s", cmd)}, UVM_LOW)
    return cmd;
  endfunction

  virtual function void populate_item(string header, int r, int c, string data, ref `APB_ITEM item);
    if (!cb_map.exists(header)) begin
      `uvm_fatal("NOREGISTER",{"callback header must be register to cb_map: %s", header});
    end

    case(cb_map[header])
      // extend your cb event
      SMTDV_CB_DATA:  begin
        item.data_beat[item.data_idx] |= data.atoi() << (r*8);
      end
      default:  begin
      end
    endcase
  endfunction

  virtual function void prepare_item(ref `APB_ITEM item);
    item.addrs.delete();
    item.data_beat.delete();
    item.addr_idx = 0;
    item.data_idx = 0;

    item.addrs.push_back(item.addr);
    item.data_beat.push_back(0);
  endfunction

  virtual function void post_item(string table_nm, `APB_ITEM item);
    for(item.data_idx =0; item.data_idx < 1; item.data_idx++) begin
      convert_2_item(
        table_nm,
        gen_query_cmd(table_nm, "LAST_WR_TRX", item),
        item);
    end
     `uvm_info(get_full_name(), {$psprintf("get after backdoor item\n%s", item.sprint())}, UVM_LOW)
  endfunction

  virtual function void convert_2_item(string table_nm, string query, `APB_ITEM item);
    chandle m_pl;  // iter pool
    chandle m_row; // iter row
    chandle m_col; // iter col
    int m_row_size =0;
    int m_col_size =0;
    int len =0;
    string header, data;

    smtdv_sqlite3::create_pl(table_nm);
    m_pl = smtdv_sqlite3::exec_query(table_nm, query);
    if (!m_pl) begin
      `uvm_warning("NOBACKDATA",{"backdoor %s not found at query: %s,", table_nm, query});
      return;
    end

    // iter row and col
    m_row_size = smtdv_sqlite3::exec_row_size(m_pl);
    for (int r=0; r<m_row_size; r++) begin
      m_row = smtdv_sqlite3::exec_row_step(m_pl, r);
      m_col_size = smtdv_sqlite3::exec_column_size(m_row);
      for (int c=0; c<m_col_size; c++) begin
        m_col = smtdv_sqlite3::exec_column_step(m_row, c);
        if (smtdv_sqlite3::is_longint_data(m_col)) begin
          header = smtdv_sqlite3::exec_header_data(m_col);
          data = smtdv_sqlite3::exec_string_data(m_col);
//          `uvm_info(get_full_name(), {$psprintf("dec mem backdoor iter headr:%s r:%3d c:%3d, data:%d\n", header, r, c, data)}, UVM_LOW)
          populate_item(header, r, c, data, item);
        end
      end
    end
    smtdv_sqlite3::delete_pl(table_nm);
  endfunction

  virtual function bit compare(string table_nm, `APB_ITEM item, ref `APB_ITEM ritem);
    prepare_item(item);
    $cast(ritem, item.clone());
    post_item(table_nm, ritem);
    `uvm_info(get_full_name(), {$psprintf("backdoor compare \n%s, %s", item.sprint(), ritem.sprint())}, UVM_LOW)
    return item.compare(ritem);
  endfunction

endclass


`endif // end of __APB_BACKDOOR_SV__
