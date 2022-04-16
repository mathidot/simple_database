#include "defs.h"

/*
    buffer.c is used to read user input and store the input 
    into the struct InputBuffer.
*/


void print_prompt(){
    printf("db > ");
}

/*
    Create a new InputBuffer.
*/

InputBuffer* new_input_buffer(){
    InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

/*
    Close the input_buffer, and free the memory.
*/

void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

/*
    Read the input from the shell.
*/

void read_input(InputBuffer *input_buffer){
    ssize_t bytes_read = getline(&(input_buffer->buffer),&(input_buffer->buffer_length),stdin);
    if(bytes_read <= 0){
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read-1] = 0;
}