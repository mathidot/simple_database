#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "types.h"

// buffer.c
InputBuffer* new_input_buffer();
void close_input_buffer(InputBuffer *);
void print_prompt();
void read_input(InputBuffer *);

// table.c
void serialize_row(Row *,void *);
void deserialize_row(void *,Row *);
void *row_slot(Table *,uint32_t);
void print_row(Row *);
Table *db_open(const char*);
void db_close(Table *);
Cursor* table_start(Table *);
Cursor* table_end(Table *);
Cursor* table_find(Table*, uint32_t);

// statement.c
MetaCommandResult do_meta_command(InputBuffer *,Table *);
PrepareResult prepare_statement(InputBuffer *,Statement *);
PrepareResult prepare_insert(InputBuffer*, Statement*);
ExecuteResult execute_insert(Statement *,Table *);
ExecuteResult execute_select(Statement *,Table *);
ExecuteResult execute_delete(Statement *,Table *);
ExecuteResult execute_statement(Statement *,Table *);

// pager.c
Pager *pager_open(const char*);
void pager_flush(Pager *,uint32_t);
void* get_page(Pager *, uint32_t);
uint32_t get_unused_page_num(Pager *);

// cursor.c
void *cursor_value(Cursor *);
void cursor_advance(Cursor *);

// node.c
void print_constants();
uint32_t* leaf_node_num_cells(void *);
void* leaf_node_cell(void *, uint32_t);
uint32_t* leaf_node_key(void *, uint32_t);
void* leaf_node_value(void *, uint32_t);
void initialize_leaf_node(void *);
void leaf_node_insert(Cursor *, uint32_t, Row *);
void print_leaf_node(void *);
void leaf_node_split_and_insert(Cursor *, uint32_t, Row *);
void initialize_leaf_node(void *);
void leaf_node_split_and_insert(Cursor *, uint32_t, Row *);
void leaf_node_insert(Cursor *, uint32_t, Row *);
Cursor *leaf_node_find(Table *, uint32_t, uint32_t);

uint32_t *internal_node_num_keys(void *);
uint32_t *internal_node_right_child(void *);
uint32_t *internal_node_cell(void *, uint32_t);
uint32_t *internal_node_child(void *, uint32_t);
uint32_t *internal_node_key(void *, uint32_t);
uint32_t *leaf_node_next_leaf(void *);
uint32_t internal_node_find_child(void *,uint32_t);
Cursor* internal_node_find(Table *, uint32_t, uint32_t);
void update_internal_node_key(void *, uint32_t,uint32_t);
void internal_node_insert(Table *, uint32_t, uint32_t);
void initialize_internal_node(void *);

void create_new_root(Table*, uint32_t);
bool is_node_root(void *);

NodeType get_node_type(void *);
uint32_t get_node_max_key(void *);
uint32_t* node_parent(void *node);

void set_node_type(void *, NodeType);
void set_node_root(void *, bool);

void print_tree(Pager *,uint32_t, uint32_t);

#endif //DEFS_H