# Partition List Viewer

## Overview

The **Partition List Viewer** is a command-line utility designed to display the partition tables of a disk device. It supports both primary and extended partitions, including logical partitions within extended partitions. The program reads the disk's partition information from the Master Boot Record (MBR) and uses a linked list to manage and display the partition data dynamically.

## Features

- **Supports MBR Partition Tables**: Handles up to 4 primary partitions or 3 primary plus one extended partition.
- **Extended Partitions**: Recursively processes extended partitions to display logical partitions.
- **Linked List Management**: Uses linked lists to manage and dynamically store partition entries.
- **Detailed Output**: Provides comprehensive details about each partition, including start sector, end sector, total sectors, and size in megabytes.

## Requirements

- C compiler (e.g., GCC)
- Standard C library functions

## Installation

To build the program from source, follow these steps:

1. **Clone the Repository**:

   ```sh
   git clone https://github.com/yourusername/partition-list-viewer.git
   cd partition-list-viewer
   ```

2. **Compile the Program**:

   ```sh
   make
   ```

   Alternatively, you can compile directly using `gcc`:

   ```sh
   gcc -o partition-list-viewer partition-list-viewer.c
   ```

## Usage

After building the program, you can use it to list partitions on a disk device. Replace `<device>` with the path to your disk device (e.g., `/dev/sda`).

```sh
./partition-list-viewer <device>
```

### Example

```sh
./partition-list-viewer /dev/sda
```

### Output Format

The output provides the following details for each partition:

- **Device**: The path to the disk device.
- **Start**: The starting sector of the partition.
- **End**: The ending sector of the partition.
- **Sectors**: Total number of sectors in the partition.
- **Size**: Size of the partition in megabytes.
- **Id**: Partition type identifier.
- **Type**: Type of partition (e.g., Primary, Extended, Logical).

## Code Explanation

1. **Data Structures**:
   - **`PartitionEntry`**: Represents a partition entry from the MBR.
   - **`PartitionNode`**: Represents a node in the linked list containing a partition entry.
   - **`PartitionList`**: Manages the linked list of partition nodes.

2. **Functions**:
   - **`init_partition_list`**: Initializes an empty linked list.
   - **`append_partition`**: Appends a new partition entry to the linked list.
   - **`free_partition_list`**: Frees all nodes in the linked list to prevent memory leaks.
   - **`print_partition`**: Prints partition details.
   - **`read_and_print_partition_table`**: Reads the partition table from the disk and updates the linked list.

3. **`main` Function**:
   - **Opens the disk device**.
   - **Initializes the linked list**.
   - **Reads and prints partitions**.
   - **Cleans up** resources and closes the file descriptor.

## Example of the output

![image](https://github.com/user-attachments/assets/33b80486-45b3-4224-95ce-3890ab9ef3b0)
   
