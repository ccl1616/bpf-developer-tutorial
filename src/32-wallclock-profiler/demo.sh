#!/bin/bash

# Simple script to demonstrate offcputime profiling
# This script compiles and runs the test target program, then profiles it

echo "=== Off-CPU Profiling Demo ==="
echo

# Compile the test target program
echo "1. Compiling test target program..."
make -f Makefile.test
if [ $? -ne 0 ]; then
    echo "Failed to compile test target program"
    exit 1
fi

echo "2. Starting test target program in background..."
./test_target &
TARGET_PID=$!

echo "   Target program PID: $TARGET_PID"
echo "   Target program will run for about 20 seconds"
echo

# Wait a moment for the program to start
sleep 2

echo "3. Starting off-CPU profiling for 15 seconds..."
echo "   Run this command in another terminal to see real-time output:"
echo "   sudo ./offcputime -p $TARGET_PID -m 1000 15"
echo

# Run the profiler
sudo ./offcputime -p $TARGET_PID -m 1000 15

echo
echo "4. Waiting for target program to finish..."
wait $TARGET_PID

echo "5. Cleaning up..."
make -f Makefile.test clean

echo
echo "=== Demo completed ==="
echo "The profiler output above shows:"
echo "- Off-CPU time spent in sleep operations (I/O simulation)"
echo "- Off-CPU time spent waiting for locks (lock contention)"
echo "- Stack traces showing where blocking occurred"
echo
echo "To see a flame graph, you can:"
echo "1. Save the output to a file: sudo ./offcputime -p \$PID -m 1000 15 > profile.folded"
echo "2. Generate flame graph: flamegraph.pl profile.folded > profile.svg"
echo "3. Open profile.svg in a web browser"
