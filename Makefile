# Compiler
CC = gcc

# Flags
CFLAGS = -Wall

# Targets
all: main vehicleA vehicleB controller

# Compile main
main: main.c
	$(CC) $(CFLAGS) main.c -o main

# Compile vehicle A
vehicleA: vehicleA.c
	$(CC) $(CFLAGS) vehicleA.c -o vehicleA

# Compile vehicle B
vehicleB: vehicleB.c
	$(CC) $(CFLAGS) vehicleB.c -o vehicleB

# Compile controller
controller: controller.c
	$(CC) $(CFLAGS) controller.c -o controller

# Run program
run: all
	./main

# Clean build files
clean:
	rm -f main vehicleA vehicleB controller log.txt
