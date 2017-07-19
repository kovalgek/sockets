#include <stdio.h>
#include <unistd.h>
#include "../common/TCPServerUtility.h"
#include "../common/DieWithMessage.h"

int main(int argc, char *argv[]) {

	if (argc != 2) // Test for correct number of arguments
		DieWithUserMessage("Parameter(s)", "<Server Port/Service>");

	char *service = argv[1]; // First arg: local port

	// Create socket for incoming connections

	int servSock = SetupTCPServerSocket(service);
	if (servSock < 0)
		DieWithUserMessage("SetupTCPServerSocket() failed", service);

	for (;;) { // Run forever
		// New connection creates a connected client socket
		int clntSock = AcceptTCPConnection(servSock);

		HandleTCPClient(clntSock); // Process client
		close(clntSock);
	}
	// NOT REACHED
}
