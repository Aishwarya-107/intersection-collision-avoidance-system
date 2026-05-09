//header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>

// Structure to store vehicle data in shared memory
struct vehicle {
    int speed;
    int distance;
};

// Signal handler for collision warning (SIGUSR1)
void collision_handler(int sig) {
    printf("🚨 Vehicle A: Collision Warning!\n");
}

// Signal handler for graceful shutdown (SIGINT)
void exit_handler(int sig) {
    printf("Vehicle A shutting down...\n");
    exit(0);
}

int main(int argc, char *argv[]) {

     // Register signal handlers
    signal(SIGUSR1, collision_handler);
    signal(SIGINT, exit_handler);

    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);

    struct vehicle *data = (struct vehicle*) shmat(shmid, NULL, 0);

    srand(time(NULL) + getpid());

    while(1) {
        semop(semid, &(struct sembuf){0, -1, 0}, 1);

        data[0].speed = rand() % 100 + 1;
        data[0].distance = rand() % 200 + 1;

        printf("Vehicle A → Speed=%d Distance=%d\n", data[0].speed, data[0].distance);

        semop(semid, &(struct sembuf){0, 1, 0}, 1);

        sleep(5);
    }

    return 0;
}
