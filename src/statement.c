#include "defs.h"

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table)
{
    if (strcmp(input_buffer->buffer, ".exit") == 0)
    {
        close_input_buffer(input_buffer);
        db_close(table);
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(input_buffer->buffer, ".table") == 0)
    {
        printf("The table command.\n");
        return META_COMMAND_SUCCESS;
    }
    else if (strcmp(input_buffer->buffer, ".constants") == 0)
    {
        printf("Constants:\n");
        print_constants();
        return META_COMMAND_SUCCESS;
    }
    else if (strcmp(input_buffer->buffer, ".btree") == 0)
    {
        printf("Tree:\n");
        print_tree(table->pager,0,0);
        return META_COMMAND_SUCCESS;
    }
    else
    {
        return META_COMMAND_UNRECONGNIZED_COMMAND;
    }
}

PrepareResult prepare_insert(InputBuffer *input_buffer, Statement *statement)
{
    statement->type = STATEMENT_INSERT;

    char *keyword = strtok(input_buffer->buffer, " ");
    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *useremail = strtok(NULL, " ");

    if (keyword == NULL || id_string == NULL || username == NULL || useremail == NULL)
        return PREPARE_SYNTAX_ERROR;

    int id = atoi(id_string);
    if (id < 0)
        return PREPARE_NEGATIVE_ID;

    statement->row_to_insert.id = id;

    if (strlen(username) > COLUMN_USERNAMEA_SIZE ||
        strlen(useremail) > COLUMN_EMAIL_SIZE)
    {
        return PREPARE_STRING_TOO_LONG;
    }
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, useremail);
    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
    if (strncmp(input_buffer->buffer, "insert", 6) == 0)
    {
        return prepare_insert(input_buffer, statement);
    }
    else if (strncmp(input_buffer->buffer, "delete", 6) == 0)
    {
        statement->type = STATEMENT_DELETE;
        return PREPARE_SUCCESS;
    }
    else if (strcmp(input_buffer->buffer, "select") == 0)
    {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECONGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement *statement, Table *table)
{
    void *node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);

    Row *row_to_insert = &(statement->row_to_insert);
    uint32_t key_to_insert = row_to_insert->id;
    Cursor *cursor = table_find(table, key_to_insert);
    if (cursor->cell_num < num_cells)
    {
        uint32_t key_at_index = *leaf_node_key(node, cursor->cell_num);
        if (key_at_index == key_to_insert)
        {
            return EXECUTE_DUPLICATE_KEY;
        }
    }

    leaf_node_insert(cursor, row_to_insert->id, row_to_insert);
    free(cursor);

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement *statement, Table *table)
{
    Row row;
    Cursor *cursor = table_start(table);
    while (!(cursor->end_of_table))
    {
        deserialize_row(cursor_value(cursor), &row);
        print_row(&row);
        cursor_advance(cursor);
    }
    free(cursor);

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_delete(Statement *statement, Table *table)
{
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement *statement, Table *table)
{
    switch (statement->type)
    {
    default:
        return EXECUTE_FAILED;
    case STATEMENT_INSERT:
        return execute_insert(statement, table);
    case STATEMENT_DELETE:
        return execute_delete(statement, table);
    case STATEMENT_SELECT:
        return execute_select(statement, table);
    }
}
