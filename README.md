
# Dynamic Memory Allocator Simulator (C Program)

This project simulates a simple dynamic memory allocation system in C.
It demonstrates **First-Fit, Best-Fit, and Worst-Fit** memory allocation strategies, including deallocation and compaction.
Useful for OS labs, memory management assignments, and interview preparation.

---

## Features

* User-defined memory size
* Allocation strategies:

  * First-Fit
  * Best-Fit
  * Worst-Fit
* Deallocate memory by process ID
* Memory compaction (removes fragmentation)
* Display current memory map
* Show memory statistics:

  * Total memory
  * Free memory
  * Free block count
  * Largest free block
* Prevents duplicate process IDs

---

## Data Structure Used

* **Array of blocks** (simulates memory)
* Each block contains:

  * `id` : process ID (0 = free)
  * `start` : starting address
  * `size` : block size

---

## How to Compile & Run

### Windows (MinGW/GCC)

```
gcc allocator.c -o allocator
./allocator
```

### Linux / macOS

```
gcc allocator.c -o allocator
./allocator
```

---

## Menu Options

```
1. Allocate Memory
2. Deallocate Memory
3. Display Memory Map
4. Compact Memory
5. Show Memory Stats
6. Exit
```

---

## Sample Commands (Example Run)

```
Enter total memory size: 100

1 Allocate  2 Deallocate  3 Display Map  4 Compact  5 Stats  6 Exit
Choose: 1
Enter process id: 10
Enter requested size: 20
Strategy: 1=FirstFit 2=BestFit 3=WorstFit: 1
Allocated pid 10 size 20
```

---

## Output Example (Display Map)

```
Memory Map (start:size:id)
[0:20:10] [20:80:0]
(id = 0 => free block)
```

---

## Concepts Demonstrated

* Memory Management
* Fragmentation
* Compaction
* Allocation Strategies
* Simulation of OS Memory Allocation

---

## Future Enhancements

* File-based logging
* GUI version using C/Java/Python
* Real memory visualization bars
* Add "Next-Fit" strategy
* Auto-test mode for demonstration


