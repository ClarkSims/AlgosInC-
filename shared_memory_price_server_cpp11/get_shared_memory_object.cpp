#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

#include "shared_memory_price_server.h"

void *get_shared_memory_object( const char *fname, int uid, size_t size, bool huge_pages) {
    key_t key;
    int shmid;
    void *data;

    /* make the key: */
    if ((key = ftok( fname, uid)) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    int flags;
    if (huge_pages) {
        flags = 0644 | IPC_CREAT | SHM_HUGETLB;
    } else {
        flags = 0644 | IPC_CREAT;
    }
    
    if ((shmid = shmget(key, size, flags)) == -1) {
        // for machines not configured for huge pages
        if ((shmid = shmget(key, size, 0644 | IPC_CREAT)) == -1) {
            /* 0644 =>    drw-r--r-- */
            perror("shmget");
            exit(1);
        }
    }

    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    } 

    return data;
}
