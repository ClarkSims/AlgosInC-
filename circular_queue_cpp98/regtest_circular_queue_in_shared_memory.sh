#!/bin/bash

### Testing persistance of shared memory for a circular queue

echo "running test2_circular_queue_in_shared_memory parent";
../bin/test2_circular_queue_in_shared_memory parent || { echo "../bin/test2_circular_queue_in_shared_memory failed"; exit 1; }

echo "running test2_circular_queue_in_shared_memory parent again to test overwrite";
../bin/test2_circular_queue_in_shared_memory parent || { echo "../bin/test2_circular_queue_in_shared_memory failed"; exit 1; }

echo "running test2_circular_queue_in_shared_memory child";
../bin/test2_circular_queue_in_shared_memory child || { echo "../bin/test2_circular_queue_in_shared_memory failed"; exit 1; }

echo "running test2_circular_queue_in_shared_memory step_parent again to repopulate the queue in memory";
../bin/test2_circular_queue_in_shared_memory step_parent || { echo "../bin/test2_circular_queue_in_shared_memory failed"; exit 1; }

echo "running test2_circular_queue_in_shared_memory child again to empty it again";
../bin/test2_circular_queue_in_shared_memory child || { echo "../bin/test2_circular_queue_in_shared_memory failed"; exit 1; }

echo "running test2_circular_queue_in_shared_memory cleanup";
../bin/test2_circular_queue_in_shared_memory cleanup || { echo "../bin/test2_circular_queue_in_shared_memory failed"; exit 1; }

echo "running test2_circular_queue_in_shared_memory child to test that it exits correctly with error code 1 and does not hang";
../bin/test2_circular_queue_in_shared_memory child || { echo "../bin/test2_circular_queue_in_shared_memory failed just as it should have, so it actually worked"; }

