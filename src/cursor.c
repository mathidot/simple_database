#include "defs.h"

/*
    A cursor object represents a location in the table.
*/


//Access the row the cursor is pointing
void *cursor_value(Cursor *cursor)
{
    void *node = get_page(cursor->table->pager, cursor->page_num);

    return leaf_node_value(node, cursor->cell_num);
}

//Advance the cursor to the next row
void cursor_advance(Cursor *cursor)
{
    uint32_t page_num = cursor->page_num;
    void *node = get_page(cursor->table->pager, page_num);

    cursor->cell_num += 1;
    if (cursor->cell_num >= *(leaf_node_num_cells(node)))
    {
        /* Advance  to next leaf node */
        uint32_t next_page_num = *leaf_node_next_leaf(node);
        if(next_page_num == 0){
            /* This was rightmost leaf*/
            cursor->end_of_table = true;
        }else{
            cursor->page_num = next_page_num;
            cursor->cell_num = 0;
        }
    }
}

/*
Later, we also want to 
    1.delete the row pointed by a cursor
    2. Modify the row pointed to by a cursor
    3. Search a table for a given ID, and create a cursor pointing to the row with that ID
*/