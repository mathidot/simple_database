#include "src/defs.h"

int main(int argc,char *argv[])
{
    if(argc < 2){
        printf("Must supply a file name.\n");
        exit(EXIT_FAILURE);
    }

    const char* file_name = argv[1];
    Table* table = db_open(file_name);
    InputBuffer *input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if(input_buffer->buffer[0] == '.'){
            switch (do_meta_command(input_buffer,table))
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
            default:
                break;
            case PREPARE_SUCCESS:
                break;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                continue;
            case PREPARE_NEGATIVE_ID:
                printf("ID must be positive.\n");
                continue;
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