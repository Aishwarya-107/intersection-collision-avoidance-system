PROJECT TITLE:
Smart Intersection Collision Avoidance System

SCENARIO DESCRIPTION:

This project simulates a smart intersection where two vehicles approach from different directions. Each vehicle continuously generates its speed and distance from the intersection and stores it in shared memory.
A controller process reads this data, calculates the time required for each vehicle to reach the intersection, and predicts a possible collision. If both vehicles are expected to arrive at nearly the same time, the system detects a collision risk and sends warning signals to the vehicles.
The system runs continuously in real-time and logs all events for analysis.

CALLS/APIs USED:

1. Process Control
fork()
Creates child processes for: 
Vehicle A 
Vehicle B 
Controller 
execl()

Replaces child process with respective program (vehicleA, vehicleB, controller) 
wait()

Parent waits for child processes to finish and prevents zombie processes 

2. Shared Memory
shmget()
Creates shared memory segment to store vehicle data 

shmctl()
Deletes shared memory after execution 

3. Semaphore
semget()
Creates semaphore for synchronization 

semctl()
Initializes semaphore (value = 1) and removes it after execution.

4. Signals
signal()
Handles SIGINT (Ctrl + C) for graceful system shutdown 

5. Other Functions
sleep()
Adds delay for better display 

sprintf()
Converts integers (IDs, PIDs) into strings for passing to exec.

FILES DESCRIPTION:

-main.c → Supervisor process
-vehicleA.c → Simulates vehicle A
-vehicleB.c → Simulates vehicle B
-controller.c → Detects collision and sends alerts
-Makefile → Compilation automation


OUTPUT:

-Displays vehicle data
-Detects collision
-ends warning signals
-Logs output in log.txt



