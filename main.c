#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECONGNIZED_COMMAND
}MetaCommandResult;

typedef enum{
    PREPARE_SUCCESS,
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
    EXECUTE_TABLE_FULL,
    EXECUTE_FAILED,
}ExecuteResult;

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
    char username[COLUMN_USERNAMEA_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}Row;

typedef struct 
{
    StatementType type;
    Row row_to_insert;
}Statement;

#define size_of_attribute(Struct,Attribute) sizeof(((Struct *)0)->Attribute)

const uint32_t ID_SIZE = size_of_attribute(Row,id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row,username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row,email);
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;

void serialize_row(Row *source,void *destination)
{
    memcpy(destination + ID_OFFSET,&(source->id),ID_SIZE);
    memcpy(destination + USERNAME_OFFSET,&(source->username),USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET,&(source->email),EMAIL_SIZE);
}

void deserialize_row(void *source,Row *destination)
{
    memcpy(&(destination->id),source + ID_OFFSET,ID_SIZE);
    memcpy(&(destination->username),source + USERNAME_OFFSET,USERNAME_SIZE);
    memcpy(&(destination->email),source + EMAIL_OFFSET,EMAIL_SIZE);
}

const uint32_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
const uint32_t ROWS_PER_PARE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PARE * TABLE_MAX_PAGES;

typedef struct
{   
    uint32_t num_rows;
    void *pages[TABLE_MAX_PAGES];
}Table;

void *row_slot(Table *table,uint32_t num_rows)
{
    uint32_t page_num = num_rows / ROWS_PER_PARE;
    void *page = table->pages[page_num];
    if(page == NULL){
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    uint32_t row_offset = num_rows % ROWS_PER_PARE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    
    return page + byte_offset;
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer)
{
    if(strcmp(input_buffer->buffer,".exit") == 0){
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }else if(strcmp(input_buffer->buffer,".table") == 0){
        printf("The table command.\n");
        return META_COMMAND_SUCCESS;
    }  
    else{
        return META_COMMAND_UNRECONGNIZED_COMMAND;
    }
}

InputBuffer* new_input_buffer(){
    InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

void print_prompt(){
    printf("db>");
}

void read_input(InputBuffer *input_buffer){
    ssize_t bytes_read = getline(&(input_buffer->buffer),&(input_buffer->buffer_length),stdin);
    if(bytes_read <= 0){
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read-1] = 0;
}


PrepareResult prepare_statement(InputBuffer *input_buffer,Statement *statement)
{
    if(strncmp(input_buffer->buffer,"insert",6) == 0){
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer,"insert %d %s %s",&(statement->row_to_insert.id),
            statement->row_to_insert.username,statement->row_to_insert.email);
        if(args_assigned < 3){
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }else if(strncmp(input_buffer->buffer,"delete",6) == 0){
        statement->type = STATEMENT_DELETE;
        return PREPARE_SUCCESS;
    }else if(strcmp(input_buffer->buffer,"select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECONGNIZED_STATEMENT;
}

void print_row(Row *row)
{
    printf("(%d %s %s)\n",row->id,row->username,row->email);
}

ExecuteResult execute_insert(Statement *statement,Table *table)
{
    if(table->num_rows >= TABLE_MAX_ROWS){
        return EXECUTE_TABLE_FULL;
    }

    Row *row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert,row_slot(table,table->num_rows));   
    table->num_rows += 1;
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement,Table *table)
{
    Row row;
    for(uint32_t i = 0; i < table->num_rows;i++)
    {
        deserialize_row(row_slot(table,i),&row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_delete(Statement *statement,Table *table)
{
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement,Table *table){
    switch(statement->type){
        default:
            return EXECUTE_FAILED;
        case STATEMENT_INSERT:
            return execute_insert(statement,table);
        case STATEMENT_DELETE:
            return execute_delete(statement,table);
        case STATEMENT_SELECT:
            return execute_select(statement,table);
    }
}

Table* new_table()
{
    Table *table = (Table *)malloc(sizeof(Table));
    table->num_rows = 0;
    for(uint32_t i = 0;i < TABLE_MAX_PAGES;i++){
        table->pages[i] = NULL;
    }
    return table;
}

void free_table(Table *table)
{
    for(uint32_t i = 0;table->pages[i];i++){
        free(table->pages[i]);
    }
    free(table);
}


int main(int argc,char *argv[])
{
    Table* table = new_table();
    InputBuffer *input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if(input_buffer->buffer[0] == '.'){
            switch (do_meta_command(input_buffer))
            {
            case META_COMMAND_SUCCESS:
                continue;
            case META_COMMAND_UNRECONGNIZED_COMMAND:
                printf("Unrecongnized command:%s\n",input_buffer->buffer);
                continue;
            }
        }

        Statement statement;
        switch(prepare_statement(input_buffer,&statement)){
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECONGNIZED_STATEMENT:
                printf("Unrecongnized statement:%s\n",input_buffer->buffer);
                continue;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error. Could not parse statement.\n");
                continue;
        }
        switch(execute_statement(&statement,table)){
            default:
                break;
            case EXECUTE_SUCCESS:
                printf("Executed.\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table full.\n");
                break;
            case EXECUTE_FAILED:
                printf("Executed Failed.\n");
                break;
        }
    }
    return 0;
}