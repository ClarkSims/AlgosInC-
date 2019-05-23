#!/bin/bash

### Testing read write between processes using a circular queue in shared memory

echo "running test1_circular_queue_in_shared_memory parent";
../bin/test1_circular_queue_in_shared_memory parent &
sleep 1

echo "running test1_circular_queue_in_shared_memory child";
../bin/test1_circular_queue_in_shared_memory child || { echo "../bin/test1_circular_queue_in_shared_memory failed"; exit 1; }

echo "running test1_circular_queue_in_shared_memory cleanup";
../bin/test1_circular_queue_in_shared_memory cleanup || { echo "../bin/test1_circular_queue_in_shared_memory failed"; exit 1; }

