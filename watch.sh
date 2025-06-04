#!/bin/bash

# Kill any existing instances of the program
pkill -f "./program" 2>/dev/null

# Watch for changes in .c files and rebuild/run
find . -name "*.c" | entr -r sh -c 'make && ./program' 