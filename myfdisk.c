#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <string.h>

#define SECTOR_SIZE 512
#define MAX_PARTITIONS 4

typedef struct PartitionEntry {
    uint8_t status;
    uint8_t first_chs[3];
    uint8_t partition_type;
    uint8_t last_chs[3];
    uint32_t lba;
    uint32_t sector_count;
} PartitionEntry;

typedef struct PartitionNode {
    PartitionEntry entry;
    struct PartitionNode *next;
} PartitionNode;

typedef struct {
    PartitionNode *head;
    PartitionNode *tail;
} PartitionList;

void init_partition_list(PartitionList *list) {
    list->head = NULL;
    list->tail = NULL;
}

void append_partition(PartitionList *list, PartitionEntry *entry) {
    PartitionNode *new_node = (PartitionNode *)malloc(sizeof(PartitionNode));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->entry = *entry;
    new_node->next = NULL;
    if (list->tail) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
}

void free_partition_list(PartitionList *list) {
    PartitionNode *current = list->head;
    while (current) {
        PartitionNode *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

void print_partition(const char *device, int partition_number, PartitionEntry *entry) {
    if (entry->partition_type == 0) {
        return;
    }

    uint32_t start_sector = entry->lba;
    uint32_t end_sector = start_sector + entry->sector_count - 1;
    uint32_t size_mb = (uint32_t)(((uint64_t)entry->sector_count * SECTOR_SIZE) / (1024 * 1024));

    printf("%-10s%-10u%-10u%-10u%-10u%-2" PRIx8 "  %-10s\n",
           device,
           start_sector,
           end_sector,
           entry->sector_count,
           size_mb,
           entry->partition_type,
           (entry->partition_type == 0x05) ? "Extended" : "Linux");
}

void read_and_print_partition_table(const char *device, int fd, off_t start_offset, PartitionList *list, int *partition_counter) {
    char buf[SECTOR_SIZE];
    if (lseek(fd, start_offset, SEEK_SET) == -1) {
        perror("lseek");
        return;
    }

    if (read(fd, buf, SECTOR_SIZE) != SECTOR_SIZE) {
        perror("read");
        return;
    }

    PartitionEntry *table_entry_ptr = (PartitionEntry *) &buf[446];
    for (int i = 0; i < MAX_PARTITIONS; i++) {
        if (table_entry_ptr[i].partition_type != 0) {
            append_partition(list, &table_entry_ptr[i]);
            print_partition(device, ++(*partition_counter), &table_entry_ptr[i]);
        }

        if (table_entry_ptr[i].partition_type == 0x05) {
            off_t extended_start = (off_t) table_entry_ptr[i].lba * SECTOR_SIZE;
            read_and_print_partition_table(device, fd, extended_start, list, partition_counter);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <device>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    int partition_counter = 0;
    PartitionList list;
    init_partition_list(&list);

    printf("Device    Start        End    Sectors   Size      Id  Type\n");
    read_and_print_partition_table(argv[1], fd, 0, &list, &partition_counter);

    free_partition_list(&list);
    close(fd);
    return 0;
}
