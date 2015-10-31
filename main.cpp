#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>

int main(int argc, char *argv[]) {

	int sockfd, portno, n;
	char buffer[256];
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if(argc < 3) {
		fprintf(stderr, "usage %s hostname port", argv[0]);
	}

	// port number of the client
	portno = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		fprintf(stderr, "ERROR opening socket");
	
	// name of the host
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERRROR, no such host");
	}

	memset( &serv_addr, 0, sizeof(serv_addr));
	memset( buffer, 0, 256);
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	      (char *)&serv_addr.sin_addr.s_addr,
		        server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd,(struct sockaddr *) &serv_addr,
		  sizeof(serv_addr)) < 0)
		fprintf(stderr, "ERROR, conection failed");

	n = write(sockfd, buffer, 8);

	
	return 0;
}

