#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

extern const uint32_t ID_SIZE;
extern const uint32_t USERNAME_SIZE;
extern const uint32_t EMAIL_SIZE;
extern const uint32_t ROW_SIZE;
extern const uint32_t ID_OFFSET;
extern const uint32_t USERNAME_OFFSET;
extern const uint32_t EMAIL_OFFSET;

extern const uint32_t PAGE_SIZE;
// extern const uint32_t TABLE_MAX_PAGES;
#define TABLE_MAX_PAGES 100

// Commond Node Header Layout
extern const uint32_t NODE_TYPE_SIZE;
extern const uint32_t NODE_TYPE_OFFSET;
extern const uint32_t IS_ROOT_SIZE;
extern const uint32_t IS_ROOT_OFFSET;
extern const uint32_t PARENT_POINTER_SIZE;
extern const uint32_t PARENT_POINTER_OFFSET;
extern const uint8_t COMMON_NODE_HEADER_SIZE;

// Leaf Node Header Layout
extern const uint32_t LEAF_NODE_NUM_CELLS_SIZE;
extern const uint32_t LEAF_NODE_NUM_CELLS_OFFSET;
extern const uint32_t LEAF_NODE_NEXT_LEAF_SIZE;
extern const uint32_t LEAF_NODE_NEXT_LEAF_OFFSET;
extern const uint32_t LEAF_NODE_HEADER_SIZE;

// Leaf Node Body Layout
extern const uint32_t LEAF_NODE_KEY_SIZE;
extern const uint32_t LEAF_NODE_KEY_OFFSET;
extern const uint32_t LEAF_NODE_VALUE_SIZE;
extern const uint32_t LEAF_NODE_VALUE_OFFSET;
extern const uint32_t LEAF_NODE_CELL_SIZE;
extern const uint32_t LEAF_NODE_SPACE_FOR_CELLS;
extern const uint32_t LEAF_NODE_MAX_CELLS;

// Leaf Node sizes
extern const uint32_t LEAF_NODE_RIGHT_SPLIT_COUNT;
extern const uint32_t LEAF_NODE_LEFT_SPLIT_COUNT;

// Internal Node Header Layout
extern const uint32_t INTERNAL_NODE_NUM_KEYS_SIZE;
extern const uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET;
extern const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE;
extern const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET;
extern const uint32_t INTERNAL_NODE_HEADER_SIZE;

// Internal Node Body Layout
extern const uint32_t INTERNAL_NODE_KEY_SIZE;
extern const uint32_t INTERNAL_NODE_CHILD_SIZE;
extern const uint32_t INTERNAL_NODE_CELL_SIZE;
extern const uint32_t INTERNAL_NODE_MAX_CELLS;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECONGNIZED_COMMAND
}MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
    PREPARE_STRING_TOO_LONG,
    PREPARE_NEGATIVE_ID,
    PREPARE_UNRECONGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
}PrepareResult;

typedef enum{
    STATEMENT_INSERT,
    STATEMENT_DELETE,
    STATEMENT_SELECT
}StatementType;

typedef enum{
    EXECUTE_SUCCESS,
    EXECUTE_FAILED,
    EXECUTE_TABLE_FULL,
    EXECUTE_DUPLICATE_KEY,
}ExecuteResult;

typedef enum {NODE_INTERNAL, NODE_LEAF} NodeType;

typedef struct
{
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
}InputBuffer;

#define COLUMN_USERNAMEA_SIZE 32
#define COLUMN_EMAIL_SIZE 255
typedef struct 
{
    uint32_t id;
    char username[COLUMN_USERNAMEA_SIZE+1];
    char email[COLUMN_EMAIL_SIZE+1];
}Row;

typedef struct 
{
    StatementType type;
    Row row_to_insert;
}Statement;

typedef struct 
{   
    int file_discriptor;
    uint32_t file_length;
    uint32_t num_pages;
    void *pages[TABLE_MAX_PAGES];
}Pager;

typedef struct
{   
    uint32_t root_page_num;
    Pager *pager;
}Table;

typedef struct 
{
    Table *table;
    uint32_t page_num;
    uint32_t cell_num;
    bool end_of_table;
}Cursor;


#endif //TYPES_H