/*
	Author: Blake Johnson
	COSC 439
	Client side, reades a message from the user and sents it to the sever.
	Receives severs translation to or from Pit Latin and displays is. 
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <string.h>

typedef struct {

	enum { toPigLatin, toEnglish }  requestType;     /* same size as an unsigned int */

	unsigned int requestID;                          /* unique identifier for each request */

	char fromPhrase[256];                           /* phrase to be translated */

} translateRequest;

typedef struct {

	unsigned int requestID;                                    /* unique identifier for each request */

	char toPhrase[256];                                        /* translated phrase */

} translateResponse;

void DieWithError(char* errorMessage);  /* External error handling function */

int main(int argc, char* argv[]) {

	int sock;                        /* Socket descriptor */
	struct sockaddr_in echoServAddr; /* Echo server address */
	struct sockaddr_in fromAddr;     /* Source address of echo */
	unsigned short echoServPort;     /* Echo server port */
	char* servIP;                    /* IP address of server */
	unsigned int fromSize;           /* In-out of address size for recvfrom() */
	translateRequest package;		 /* package that will be sent*/
	translateResponse tranPackage;	 /* package that was received and translated by server*/

	memset(&package, 0, sizeof(package));    /* Zero out structure */
	memset(&tranPackage, 0, sizeof(tranPackage));    /* Zero out structure */
	package.requestID = 0;

	if ((argc < 2) || (argc > 3))    /* Test for correct number of arguments */
	{
		fprintf(stderr, "Usage: %s <Server IP>  [<Echo Port>]\n", argv[0]);
		exit(1);
	}

	//Infine loop untile the user nolonger wishes to make translations
	while (1) {
		char translationType[10]; //stores read in value of translation type
		printf("Translation type [toPigLatin/toEnglish] or Quit: ");
		scanf("%s", translationType);

		//Checks user in put to set translation type or quit
		if (strcmp(translationType, "toPigLatin") == 0) {
			package.requestType = toPigLatin;
		}
		else if (strcmp(translationType, "toEnglish") == 0) {
			package.requestType = toEnglish;
		}
		else if (strcmp(translationType, "Quit") == 0 || strcmp(translationType, "quit") == 0) {
			exit(0); //ends program
		}
		else {
			printf("Please enter [toPigLatin/toEnglish] or Quit:\n");
			continue;
		}

		//Reads in the phrase the user wants translated 
		printf("Word or phrase to translate: ");
		getchar();
		scanf("%[^\n]s", &package.fromPhrase);

		servIP = argv[1];           /* First arg: server IP address (dotted quad) */

		//Checks to see if a port was specified else make it 7
		if (argc == 3)
			echoServPort = atoi(argv[2]);  /* Use given port, if any */
		else
			echoServPort = 7;  /* 7 is the well-known port for the echo service */

			/* Create a datagram/UDP socket */
		if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
			DieWithError("socket() failed Client");

		/* Construct the server address structure */
		memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
		echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
		echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
		echoServAddr.sin_port = htons(echoServPort);     /* Server port */

		package.requestID += 1;
		printf("Request ID: %d\n", package.requestID);

		/* Send the Struct to the server  that holds message for encrpying/decrypting request ID and Translation tpye*/
		if (sendto(sock, (void*)&package, sizeof(package), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != sizeof(package))
			DieWithError("sendto() sent a different number of bytes than expected Client");
		else {
			printf("Sent: %s\n", package.fromPhrase);
		}

		/* Recv a response */
		fromSize = sizeof(fromAddr);
		if (recvfrom(sock, (void*)&tranPackage, sizeof(tranPackage), 0, (struct sockaddr*)&fromAddr, &fromSize) != sizeof(tranPackage))
			DieWithError("recvfrom() failed Client");
		else if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
		{
			fprintf(stderr, "Error: received a packet from unknown source Client.\n");
			exit(1);
		}
		else if (package.requestID != tranPackage.requestID) {
			printf("Request ID does not match, Please close and restart program.\n");
			exit(1);
		} else
			printf("Received: %s\n", tranPackage.toPhrase);    /* Print the echoed arg */

		close(sock);
	}
}