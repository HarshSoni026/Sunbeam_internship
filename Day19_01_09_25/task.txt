Task 1: Integration
Prepare:
Skeleton of full server code.
Sample test script.

Task 2: Multi-client Handling
Prepare:
Fork-based or pthread-based template.(Check which one would be better to implement).

Fork-based Tasks

1. Modify the handle_client() function to send back the client PID along with the echoed message.

2. Write a script to connect multiple clients (e.g., using telnet or nc) and observe how each child process handles them.

Pthread-based Tasks

1. Modify handle_client() to count total messages per client and print them before closing the connection.

2. Add synchronization using a pthread_mutex to maintain a global counter of total messages handled across all clients.