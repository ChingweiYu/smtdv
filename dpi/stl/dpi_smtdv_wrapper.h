

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dpi_smtdv_transfer.h"

using namespace std;
using namespace SMTDV;

#ifdef __cplusplus
extern "C" {
  long int dpi_hexstr_2_longint(char* st);
  char* dpi_longint_2_hexstr(long int i);

  void* dpi_smtdv_parse_file(char* file);
  void* dpi_smtdv_new_smtdv_transfer();
  void* dpi_smtdv_new_smtdv_mailbox(char* inst_name);
  /* begin cycle */
  void dpi_smtdv_set_smtdv_transfer_begin_cycle(void* ip, char* begin_cycle);
  char* dpi_smtdv_get_smtdv_transfer_begin_cycle(void* ip);
  /* end cycle */
  void dpi_smtdv_set_smtdv_transfer_end_cycle(void* ip, char* end_cycle);
  char* dpi_smtdv_get_smtdv_transfer_end_cycle(void* ip);
  /* begin_time */
  void dpi_smtdv_set_smtdv_transfer_begin_time(void* ip, char* begin_time);
  char* dpi_smtdv_get_smtdv_transfer_begin_time(void* ip);
  /* end_time */
  void dpi_smtdv_set_smtdv_transfer_end_time(void* ip, char* end_time);
  char* dpi_smtdv_get_smtdv_transfer_end_time(void* ip);
  /* id */
  void dpi_smtdv_set_smtdv_transfer_id(void* ip, char* id);
  char* dpi_smtdv_get_smtdv_transfer_id(void *ip);
  /* resp */
  void dpi_smtdv_set_smtdv_transfer_resp(void* ip, char* resp);
  char* dpi_smtdv_get_smtdv_transfer_resp(void* ip);
  /* rw */
  void dpi_smtdv_set_smtdv_transfer_rw(void* ip, char* rw);
  char* dpi_smtdv_get_smtdv_transfer_rw(void* ip);
  /* bst_type */
  void dpi_smtdv_set_smtdv_transfer_bst_type(void* ip, char* bst_type);
  char* dpi_smtdv_get_smtdv_transfer_bst_type(void* ip);
  /* trx_size */
  void dpi_smtdv_set_smtdv_transfer_trx_size(void* ip, char* trx_size);
  char* dpi_smtdv_get_smtdv_transfer_trx_size(void* ip);
  /* trx_prt */
  void dpi_smtdv_set_smtdv_transfer_trx_prt(void* ip, char* trx_prt);
  char* dpi_smtdv_get_smtdv_transfer_trx_prt(void* ip);
  /* lock */
  void dpi_smtdv_set_smtdv_transfer_lock(void* ip, char* lock);
  char* dpi_smtdv_get_smtdv_transfer_lock(void* ip);
  /* addr */
  void dpi_smtdv_set_smtdv_transfer_addr(void* ip, char* addr);
  char* dpi_smtdv_get_smtdv_transfer_addr(void* ip);
  /* data */
  void dpi_smtdv_set_smtdv_transfer_data(void* ip, char* data);
  char* dpi_smtdv_get_smtdv_transfer_data(void* ip, int i);
  /* byten */
  void dpi_smtdv_set_smtdv_transfer_byten(void* ip, char* byten);
  char* dpi_smtdv_get_smtdv_transfer_byten(void* ip, int i);
  /* register smtdv trx to smtdv masmtdvox */
  void dpi_smtdv_register_smtdv_transfer(void* it, void* ip);
  /* get nxt smtdv trx */
  void* dpi_smtdv_next_smtdv_transfer(void* it);
}
#endif
