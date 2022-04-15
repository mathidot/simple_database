#include "defs.h"

void *cursor_value(Cursor *cursor)
{
    void *node = get_page(cursor->table->pager, cursor->page_num);

    return leaf_node_value(node, cursor->cell_num);
}

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