/* allocator.c
   Simple Dynamic Memory Allocator Simulator
   - Strategies: First-Fit, Best-Fit, Worst-Fit
   - Menu: allocate, deallocate, show map, compact, stats, exit
   - Designed for clarity (interview-friendly)
*/

#include <stdio.h>
#include <string.h>

#define MAX_BLOCKS 50

typedef struct {
    int id;         // process id (0 = free)
    int start;      // start address (for display)
    int size;       // block size
} Block;

Block blocks[MAX_BLOCKS];
int blockCount = 0;
int totalMemory = 0;

// Initialize allocator with single free block of total size
void init_allocator(int memSize) {
    blockCount = 1;
    totalMemory = memSize;
    blocks[0].id = 0;
    blocks[0].start = 0;
    blocks[0].size = memSize;
}

// Merge adjacent free blocks (helper after deallocation)
void merge_free_blocks() {
    for (int i = 0; i < blockCount - 1; ++i) {
        if (blocks[i].id == 0 && blocks[i + 1].id == 0) {
            blocks[i].size += blocks[i + 1].size;
            // shift left
            for (int j = i + 1; j < blockCount - 1; ++j) blocks[j] = blocks[j + 1];
            blockCount--;
            i--; // re-check this index
        }
    }
}

// First-Fit allocation
int allocate_first_fit(int pid, int req) {
    for (int i = 0; i < blockCount; ++i) {
        if (blocks[i].id == 0 && blocks[i].size >= req) {
            if (blocks[i].size == req) {
                blocks[i].id = pid;
            } else {
                // split block
                for (int j = blockCount; j > i + 1; --j) blocks[j] = blocks[j - 1];
                blocks[i + 1].size = blocks[i].size - req;
                blocks[i + 1].id = 0;
                blocks[i + 1].start = blocks[i].start + req;
                blocks[i].size = req;
                blocks[i].id = pid;
                blockCount++;
            }
            return 1; // success
        }
    }
    return 0; // fail
}

// Best-Fit allocation
int allocate_best_fit(int pid, int req) {
    int bestIdx = -1, bestSize = 1<<30;
    for (int i = 0; i < blockCount; ++i) {
        if (blocks[i].id == 0 && blocks[i].size >= req && blocks[i].size < bestSize) {
            bestSize = blocks[i].size;
            bestIdx = i;
        }
    }
    if (bestIdx == -1) return 0;
    if (blocks[bestIdx].size == req) {
        blocks[bestIdx].id = pid;
    } else {
        for (int j = blockCount; j > bestIdx + 1; --j) blocks[j] = blocks[j - 1];
        blocks[bestIdx + 1].size = blocks[bestIdx].size - req;
        blocks[bestIdx + 1].id = 0;
        blocks[bestIdx + 1].start = blocks[bestIdx].start + req;
        blocks[bestIdx].size = req;
        blocks[bestIdx].id = pid;
        blockCount++;
    }
    return 1;
}

// Worst-Fit allocation
int allocate_worst_fit(int pid, int req) {
    int worstIdx = -1, worstSize = -1;
    for (int i = 0; i < blockCount; ++i) {
        if (blocks[i].id == 0 && blocks[i].size >= req && blocks[i].size > worstSize) {
            worstSize = blocks[i].size;
            worstIdx = i;
        }
    }
    if (worstIdx == -1) return 0;
    if (blocks[worstIdx].size == req) {
        blocks[worstIdx].id = pid;
    } else {
        for (int j = blockCount; j > worstIdx + 1; --j) blocks[j] = blocks[j - 1];
        blocks[worstIdx + 1].size = blocks[worstIdx].size - req;
        blocks[worstIdx + 1].id = 0;
        blocks[worstIdx + 1].start = blocks[worstIdx].start + req;
        blocks[worstIdx].size = req;
        blocks[worstIdx].id = pid;
        blockCount++;
    }
    return 1;
}

// Deallocate by process id
int deallocate(int pid) {
    int found = 0;
    for (int i = 0; i < blockCount; ++i) {
        if (blocks[i].id == pid) {
            blocks[i].id = 0;
            found = 1;
        }
    }
    if (found) merge_free_blocks();
    return found;
}

// Compact memory: move all allocated blocks to start and create one free block at end
void compact_memory() {
    int write = 0;
    for (int i = 0; i < blockCount; ++i) {
        if (blocks[i].id != 0) {
            if (i != write) {
                blocks[write].id = blocks[i].id;
                blocks[write].size = blocks[i].size;
                blocks[write].start = (write == 0) ? 0 : blocks[write - 1].start + blocks[write - 1].size;
            }
            write++;
        }
    }
    int usedSize = 0;
    for (int i = 0; i < write; ++i) usedSize += blocks[i].size;
    // one free block
    blocks[write].id = 0;
    blocks[write].start = usedSize;
    blocks[write].size = totalMemory - usedSize;
    blockCount = write + 1;
    if (blocks[write].size == 0) blockCount--; // no free block left
}

// Display memory map
void display_map() {
    printf("\nMemory Map (start:size:id)\n");
    for (int i = 0; i < blockCount; ++i) {
        printf("[%d:%d:%d] ", blocks[i].start, blocks[i].size, blocks[i].id);
    }
    printf("\n(id = 0 => free block)\n");
}

// Statistics: fragmentation and free space
void stats() {
    int freeTotal = 0, largest = 0, freeBlocks = 0;
    for (int i = 0; i < blockCount; ++i) {
        if (blocks[i].id == 0) {
            freeTotal += blocks[i].size;
            freeBlocks++;
            if (blocks[i].size > largest) largest = blocks[i].size;
        }
    }
    printf("\nTotal Memory: %d  Free: %d  Free Blocks: %d  Largest Free Block: %d\n",
           totalMemory, freeTotal, freeBlocks, largest);
}

// Check if PID already exists (prevent double allocate)
int pid_exists(int pid) {
    for (int i = 0; i < blockCount; ++i) if (blocks[i].id == pid) return 1;
    return 0;
}

// Menu
int main() {
    int memSize;
    printf("Dynamic Memory Allocator Simulator\n");
    printf("Enter total memory size (e.g. 100): ");
    if (scanf("%d", &memSize) != 1 || memSize <= 0) {
        printf("Invalid memory size.\n");
        return 0;
    }
    init_allocator(memSize);

    while (1) {
        printf("\n--- Menu ---\n");
        printf("1 Allocate  2 Deallocate  3 Display Map  4 Compact  5 Stats  6 Exit\n");
        printf("Choose: ");
        int opt; if (scanf("%d", &opt) != 1) break;

        if (opt == 1) {
            int pid, req, strat;
            printf("Enter process id (integer): "); scanf("%d", &pid);
            if (pid <= 0) { printf("pid must be > 0\n"); continue; }
            if (pid_exists(pid)) { printf("pid already allocated\n"); continue; }
            printf("Enter requested size: "); scanf("%d", &req);
            if (req <= 0) { printf("size must be > 0\n"); continue; }
            printf("Strategy: 1=FirstFit 2=BestFit 3=WorstFit : "); scanf("%d", &strat);

            int ok = 0;
            if (strat == 1) ok = allocate_first_fit(pid, req);
            else if (strat == 2) ok = allocate_best_fit(pid, req);
            else if (strat == 3) ok = allocate_worst_fit(pid, req);
            else { printf("Invalid strategy\n"); continue; }

            if (ok) printf("Allocated pid %d size %d\n", pid, req);
            else printf("Allocation failed: insufficient suitable free block\n");

        } else if (opt == 2) {
            int pid; printf("Enter pid to deallocate: "); scanf("%d", &pid);
            if (deallocate(pid)) printf("Deallocated pid %d\n", pid);
            else printf("PID not found\n");

        } else if (opt == 3) {
            display_map();

        } else if (opt == 4) {
            compact_memory();
            printf("Memory compacted.\n");

        } else if (opt == 5) {
            stats();

        } else if (opt == 6) {
            printf("Exit.\n"); break;

        } else {
            printf("Invalid option\n");
        }
    }

    return 0;
}
