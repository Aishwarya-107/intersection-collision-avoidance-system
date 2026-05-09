//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>

struct vehicle {
    int speed;
    int distance;
};

void exit_handler(int sig) {
    printf("\nController shutting down...\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    signal(SIGINT, exit_handler);
    
    // Get IDs passed from main process
    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);
    int pidA  = atoi(argv[3]);
    int pidB  = atoi(argv[4]);

    // Attach shared memory to controller
    struct vehicle *data = (struct vehicle*) shmat(shmid, NULL, 0);

     // Open log file in append mode
    int logfd = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    
    // Get current system time
    time_t now = time(NULL);
    char *time_str = ctime(&now);

    // Write header
    dprintf(logfd, "\n========================================\n");
    dprintf(logfd, "SMART INTERSECTION COLLISION AVOIDANCE SYSTEM\n");
    dprintf(logfd, "System Started at: %s", time_str);
    dprintf(logfd, "========================================\n"); 
    
    
    while(1) {

        semop(semid, &(struct sembuf){0, -1, 0}, 1);

        int speedA = data[0].speed;
        int distA  = data[0].distance;
        int speedB = data[1].speed;
        int distB  = data[1].distance;

        semop(semid, &(struct sembuf){0, 1, 0}, 1);

        printf("\n[Controller]\n");
        printf("A: S=%d D=%d | B: S=%d D=%d\n", speedA, distA, speedB, distB);

        float timeA = (float)distA / speedA;
        float timeB = (float)distB / speedB;

        time_t now = time(NULL);

        if (abs(timeA - timeB) < 2) {
            printf("⚠️ Collision Detected!\n");

            dprintf(logfd, "[%ld] COLLISION\n", now);

            kill(pidA, SIGUSR1);
            kill(pidB, SIGUSR1);
        } else {
            dprintf(logfd, "[%ld] SAFE\n", now);
        }

        sleep(5);
    }

    return 0;
}
