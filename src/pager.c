#include "defs.h"

void *get_page(Pager *pager, uint32_t page_num)
{
    if (page_num > TABLE_MAX_PAGES)
    {
        printf("Tried to fetch page number out of bounds. %d > %d\n", page_num,
               TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }
    if (pager->pages[page_num] == NULL)
    {
        // Cache miss, allocate memory and load from file
        void *page = malloc(PAGE_SIZE);
        memset(page, 0, PAGE_SIZE);
        uint32_t num_pages = pager->file_length / PAGE_SIZE;
        if (pager->file_length % PAGE_SIZE)
            num_pages += 1;

        if (page_num <= num_pages)
        {
            lseek(pager->file_discriptor, PAGE_SIZE * page_num, SEEK_SET);
            ssize_t bytes_read = read(pager->file_discriptor, page, PAGE_SIZE);
            if (bytes_read == -1)
            {
                printf("Error reading file: %d.\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_num] = page;
        if (page_num >= pager->num_pages)
        {
            pager->num_pages = page_num + 1;
        }
    }
    return pager->pages[page_num];
}


Pager *pager_open(const char *file_name)
{
    Pager *pager = (Pager *)malloc(sizeof(Pager));
    int fd = open(file_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0)
    {
        printf("Error opening file : %d.\n", errno);
        exit(EXIT_FAILURE);
    }
    off_t ost = lseek(fd, 0, SEEK_END);

    pager->file_discriptor = fd;
    pager->file_length = ost;
    pager->num_pages = (ost / PAGE_SIZE);

    if (ost % PAGE_SIZE != 0)
    {
        printf("Db file is not a whole number of pages.Corrupt file.\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
        pager->pages[i] = NULL;
    }

    return pager;
}

void pager_flush(Pager *pager, uint32_t page_num)
{
    if (pager->pages[page_num] == NULL)
    {
        printf("Tried to a NULL page.\n");
        exit(EXIT_FAILURE);
    }
    off_t oft = lseek(pager->file_discriptor, page_num * PAGE_SIZE, SEEK_SET);

    if (oft == -1)
    {
        printf("Error seeking: %d.\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_write = write(pager->file_discriptor,
                                pager->pages[page_num], PAGE_SIZE);

    if (bytes_write == -1)
    {
        printf("Error writing file: %d.\n", errno);
        exit(EXIT_FAILURE);
    }
}

/*
Util we start recycling free pages, new page will always
go onto the end of the database file
*/
uint32_t get_unused_page_num(Pager *pager)
{
    return pager->num_pages;
}