# List Labeling Array (LLA)

A C implementation of a List Labeling Array data structure that maintains a sorted array with O(log n) insertion time and O(log²n) amortized elements moved per insertion.

### Key Features

- O(log n) insertion time
- O(log²n) amortized elements moved per insertion
- Density-based rebalancing strategy
- Memory-efficient implementation
- Configurable parameters for balancing

## Implementation Details

### Data Structure

The LLA consists of two main components:
1. A sorted array storing the elements
2. A balancing tree that manages the density of array segments

### Key Parameters

- `N`: Size of the array
- `C`: Capacity factor
- `TAU_0`: Initial density threshold
- `TAU_D`: Maximum density threshold

### Core Operations

- `create_lla(N, C, TAU_0, TAU_D)`: Creates a new LLA instance
- `insert(lla, x)`: Inserts element x while maintaining sorted order
- `cleanup_lla(lla)`: Frees all allocated memory

## Building and Running

### Prerequisites

- GCC compiler
- Make
- entr (for auto-rebuild feature)

### Building

```bash
make
```

### Running

```bash
./program
```

### Auto-Rebuild and Run

To automatically rebuild and run when source files change:
```bash
./watch.sh
```

### Cleaning

```bash
make clean
```

## Performance Testing

The project includes a performance test suite that:
- Measures insertion time complexity
- Verifies O(log²n) amortized time
- Provides correlation analysis with theoretical complexity
- Reports detailed timing statistics

## Project Structure

```
.
├── lla.h          # Header file with declarations
├── lla.c          # Implementation file
├── main.c         # Test driver and performance measurements
├── Makefile       # Build configuration
└── watch.sh       # Auto-rebuild script
```

## Dependencies

- `entr` for auto-rebuild feature:
  - macOS: `brew install entr`
  - Ubuntu/Debian: `sudo apt-get install entr`
  - Fedora: `sudo dnf install entr`

## Usage Example

```c
#include "lla.h"

int main() {
    // Create LLA with N=1024, C=8, TAU_0=0.5, TAU_D=0.75
    lla *my_lla = create_lla(1024, 8, 0.5, 0.75);
    
    // Insert elements
    insert(my_lla, 10);
    insert(my_lla, 5);
    insert(my_lla, 15);
    
    // Print current state
    print_lla(my_lla);
    
    // Cleanup
    cleanup_lla(&my_lla);
    return 0;
}
```

## Performance Characteristics

- Insertion: O(log n) amortized time
- Elements moved per insertion: O(log²n) Amortized
- Space: O(n) where n is the number of elements
- Memory overhead: O(log n) for the balancing tree

## References

1. McCauley, S., Moseley, B., Niaparast, A., & Singh, S. (2023). Online List Labeling with Predictions. arXiv:2305.10536

## Contributing

Reach out at wafik.wt@gmail.com.
