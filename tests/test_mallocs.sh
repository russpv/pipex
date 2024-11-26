#!/bin/bash

# Set the path to your executable (adjust accordingly)
PROGRAM="./pipex"  # Replace with the path to your compiled program

# Example arguments to pass to your program (adjust as needed)
ARGS="here_doc EOF cat cat wc outfile.txt"

# Number of runs to perform with different call_count values (1, 2, 3,...)
NUM_RUNS=6  # Adjust this depending on how many times you want to run

# Loop over different call_count values
for i in $(seq 1 $NUM_RUNS); do
  echo "Running Valgrind with simulated malloc failure on call #$i..."

  # Set the environment variable for the call_count you want to test
  export CALL_COUNT=$i
  
  # Run the program under Valgrind, using the custom malloc via LD_PRELOAD
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $PROGRAM $ARGS
  
  # Optionally, unset the CALL_COUNT variable after each test run
  unset CALL_COUNT
  
  # Add a separator for readability
  echo "----------------------------------------"
done

