//Header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>

// Structure shared between processes
struct vehicle {
    int speed;
    int distance;
};

// Signal handler for collision alert
void collision_handler(int sig) {
    printf("🚨 Vehicle B: Collision Warning!\n");
}

// Signal handler for graceful termination
void exit_handler(int sig) {
    printf("Vehicle B shutting down...\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    signal(SIGUSR1, collision_handler);
    signal(SIGINT, exit_handler);

     // Get shared memory ID and semaphore ID from main process
    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);

    struct vehicle *data = (struct vehicle*) shmat(shmid, NULL, 0);

    srand(time(NULL) + getpid());

    while(1) {
        semop(semid, &(struct sembuf){0, -1, 0}, 1);

        data[1].speed = rand() % 100 + 1;
        data[1].distance = rand() % 200 + 1;

        printf("Vehicle B → Speed=%d Distance=%d\n", data[1].speed, data[1].distance);

        semop(semid, &(struct sembuf){0, 1, 0}, 1);

        sleep(5);
    }

    return 0;
}
