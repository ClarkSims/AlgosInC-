## test1_circular_queue_in_mmap
echo "running test1_circular_queue_in_mmap";
../bin/test1_circular_queue_in_mmap || { echo "test1_circular_queue_in_mmap failed"; exit 1; }

echo "running test2_circular_queue_in_mmap parent";
../bin/test2_circular_queue_in_mmap parent || { echo "../bin/test2_circular_queue_in_mmap failed"; exit 1; }

echo "running test2_circular_queue_in_mmap parent again to test overwrite";
../bin/test2_circular_queue_in_mmap parent || { echo "../bin/test2_circular_queue_in_mmap failed"; exit 1; }

echo "running test2_circular_queue_in_mmap child";
../bin/test2_circular_queue_in_mmap child || { echo "../bin/test2_circular_queue_in_mmap failed"; exit 1; }

echo "running test2_circular_queue_in_mmap step_parent again to repopulate the queue in memory";
../bin/test2_circular_queue_in_mmap step_parent || { echo "../bin/test2_circular_queue_in_mmap failed"; exit 1; }

echo "running test2_circular_queue_in_mmap child again to empty it again";
../bin/test2_circular_queue_in_mmap child || { echo "../bin/test2_circular_queue_in_mmap failed"; exit 1; }



echo ""
echo "All Test Passed in Regression Suite!"
