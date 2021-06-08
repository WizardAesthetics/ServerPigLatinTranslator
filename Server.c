/*
	Author: Blake Johnson
	COSC 439
	Servers side, receives message from the client translaes it to or from pig latin 
	and sends it back to the client.
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

typedef struct {

	unsigned int requestID;                                    /* unique identifier for each request */

	char toPhrase[256];                                        /* translated phrase */

} translateResponse;

typedef struct {

	enum { toPigLatin, toEnglish }  requestType;     /* same size as an unsigned int */

	unsigned int requestID;                          /* unique identifier for each request */

	char fromPhrase[256];                           /* phrase to be translated */

} translateRequest;

int main(int argc, char* argv[])
{
	int sock;                        /* Socket */
	struct sockaddr_in echoServAddr; /* Local address */
	struct sockaddr_in echoClntAddr; /* Client address */
	unsigned short echoServPort;     /* Server port */
	unsigned int cliAddrLen;         /* Length of incoming message */
	translateResponse tranPackage;	/* Message to send to the client  */
	translateRequest package;		/* Message received from the client to translate */


	if (argc != 2)         /* Test for correct number of parameters */
	{
		fprintf(stderr, "Usage:  %s <UDP SERVER PORT>\n", argv[0]);
		exit(1);
	}

	echoServPort = atoi(argv[1]);  /* First arg:  local port */

	/* Create socket for sending/receiving datagrams */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed Server");

	/* Construct local address structure */
	memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
	echoServAddr.sin_family = AF_INET;                /* Internet address family */
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	echoServAddr.sin_port = htons(echoServPort);      /* Local port */

	/* Bind to the local address */
	if (bind(sock, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("bind() failed Server");

	for (;;) /* Run forever */
	{
		memset(&package, 0, sizeof(package));    /* Zero out structure */
		memset(&tranPackage, 0, sizeof(tranPackage));    /* Zero out structure */

		cliAddrLen = sizeof(echoClntAddr);

		/* Block until receive message from a client */
		if (recvfrom(sock, (void*)&package, sizeof(package), 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen) < 0)
			DieWithError("recvfrom() failed Server");


			printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
			printf("Request ID: %d\n", package.requestID);
			printf("Received: %s\n", package.fromPhrase);
			printf("Translating...\n");
	

		char pigLatTail[256] = "";
		int i = 0;
		char vEnding[4] = "";
		char cEnding[3] = "";
		char token[256];
		char* ptr = strtok(package.fromPhrase, " ");

		if (package.requestType == 1) {
			while (ptr != NULL) {
				strcpy(token, ptr);
				memcpy(&vEnding, &token[strlen(token) - 3], strlen(token));
				memcpy(&cEnding, &token[strlen(token) - 2], strlen(token));
				if (strcmp(vEnding, "way") == 0) {
					token[strlen(token) - 3] = '\0';
				}
				else if (strcmp(cEnding, "ay") == 0 || strcmp(vEnding, "Ay") == 0 || strcmp(vEnding, "AY") == 0) {
					vEnding[1] = '\0';
					strcat(vEnding, token);
					strcpy(token, vEnding);
					token[strlen(token) - 3] = '\0';
				}
				strcat(token, " ");
				strcat(tranPackage.toPhrase, token);
				ptr = strtok(NULL, " ");
			}
		}
		else {
			while (ptr != NULL) {
				strcpy(token, ptr);
				memset(pigLatTail, 0, sizeof(pigLatTail));    /* Zero out structure */
				if (token[0] == 'w') { token[0] = 'W'; }
				for (i = 0; i < strlen(token); i++) {
					if (token[0] == 'A' || token[0] == 'a'
						|| token[0] == 'E' || token[0] == 'e'
						|| token[0] == 'I' || token[0] == 'i'
						|| token[0] == 'O' || token[0] == 'o'
						|| token[0] == 'U' || token[0] == 'u'
						|| token[i] == 'Y' || token[i] == 'y') {
						if (token[i] == 'Y' || token[i] == 'y') {
							pigLatTail[0] = 'Y';
							continue;
						}
						strcat(token, "way");
						break;
					} else if (token[i] == 'A' || token[i] == 'a'
						|| token[i] == 'E' || token[i] == 'e'
						|| token[i] == 'I' || token[i] == 'i'
						|| token[i] == 'O' || token[i] == 'o'
						|| token[i] == 'U' || token[i] == 'u'
						|| token[i] == 'Y' || token[i] == 'y') {

						memcpy(&token, &token[i], strlen(token));
						strcat(pigLatTail, "ay");
						strcat(token, pigLatTail);
						break;
					} else {
						pigLatTail[i] = token[i];
					}
				}
				strcat(token, " ");
				strcat(tranPackage.toPhrase, token);
				ptr = strtok(NULL, " ");
			}
		}
		// strcpy(tranPackage.toPhrase, package.fromPhrase);
		tranPackage.requestID = package.requestID;
		if (sendto(sock, (void*)&tranPackage, sizeof(tranPackage), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != sizeof(tranPackage))
			DieWithError("sendto() sent a different number of bytes than expected Server");
		else {
			printf("Sent: %s\n\n", tranPackage.toPhrase);
			printf("---------------------------------------------------------------\n");
			printf("---------------------------------------------------------------\n\n");
		}
	}
	/* NOT REACHED */
}