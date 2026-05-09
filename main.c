#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <signal.h>

struct vehicle {
    int speed;
    int distance;
};

// Signal handler for system shutdown
void exit_handler(int sig) {
    printf("\nSystem shutting down...\n");
    exit(0);
}

int main() {

    printf("\n========================================\n");
    printf(" SMART INTERSECTION COLLISION AVOIDANCE SYSTEM\n");
    printf("========================================\n");
    printf("Starting simulation...\n\n");
    
    sleep(2);

    signal(SIGINT, exit_handler);

    // Create shared memory for 2 vehicles
    int shmid = shmget(IPC_PRIVATE, sizeof(struct vehicle)*2, 0666 | IPC_CREAT);
    
     // Create semaphore for synchronization
    int semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

    semctl(semid, 0, SETVAL, 1);
    
    // Create Vehicle A process
    pid_t pidA = fork();

    if (pidA == 0) {
        char shm[10], sem[10];
        sprintf(shm, "%d", shmid);
        sprintf(sem, "%d", semid);

        execl("./vehicleA", "vehicleA", shm, sem, NULL);
    }

     // Create Vehicle B process
    pid_t pidB = fork();

    if (pidB == 0) {
        char shm[10], sem[10];
        sprintf(shm, "%d", shmid);
        sprintf(sem, "%d", semid);

        execl("./vehicleB", "vehicleB", shm, sem, NULL);
    }
    
    // Create Controller process
    pid_t pidC = fork();

    if (pidC == 0) {
        char shm[10], sem[10], a[10], b[10];
        sprintf(shm, "%d", shmid);
        sprintf(sem, "%d", semid);
        sprintf(a, "%d", pidA);
        sprintf(b, "%d", pidB);

        execl("./controller", "controller", shm, sem, a, b, NULL);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
