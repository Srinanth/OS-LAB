#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s AS_size_MB Page_size_KB Virtual_Address\n", argv[0]);
        return 1;
    }

    long as_size_mb = atol(argv[1]);
    long page_size_kb = atol(argv[2]);
    long virtual_addr = atol(argv[3]);

    long total_as_bytes = as_size_mb * 1024 * 1024;
    long page_size_bytes = page_size_kb * 1024;
    
    long num_pages = total_as_bytes / page_size_bytes;

    int *page_table = (int *)calloc(num_pages, sizeof(int));
    if (page_table == NULL) {
        printf("Could not allocate memory for Page Table.\n");
        return 1;
    }

    for (long i = 0; i < num_pages; i++) {
        page_table[i] = i + 100; 
    }

    
    long page_num = virtual_addr / page_size_bytes;
    long offset = virtual_addr % page_size_bytes;

    if (page_num < 0 || page_num >= num_pages || virtual_addr >= total_as_bytes) {
        printf("Page Table Miss!\n");
        printf("Virtual address %ld is outside the allocated %ld MB space.\n", 
                virtual_addr, as_size_mb);
    } else {
        int frame_number = page_table[page_num];
        
        printf("Virtual Address: %ld\n", virtual_addr);
        printf("Physical Address: <%d, %ld>\n", frame_number, offset);
    }

    free(page_table);
    return 0;
}