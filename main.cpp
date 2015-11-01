#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>
#include "data_str.h"

int main(int argc, char *argv[]) {

	int sockfd, portno, n;
	char buffer[4300];
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

	n = read(sockfd, buffer, 4);
	int id_player = *((unsigned int*)buffer); // id-ul jucatorului

	n = read(sockfd, buffer, 20);

	bool is_aggressive;
	int crt_move, max_move, aggressive, N, M;
	is_aggressive = get_info(buffer, crt_move, max_move, aggressive, N, M);

//	fprintf(stdout, "%d %d %d %d %d\n", crt_move, max_move, aggressive, N, M);

	n = read(sockfd, buffer, N * M * 4);
	player players[2];
	game_board **board = trans_input(buffer, players, N, M, id_player);

/*	FILE * fout = fopen("matr.out", "w");
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++)
			fprintf(fout, "%d ", board[i][j].is_wall);
		fprintf(fout, "\n");
	}
	fclose(fout);*/

	nod rad;
	rad.p1.pos_x = players[0].pos_x;
	rad.p1.pos_y = players[0].pos_y;

	rad.p2.pos_x = players[1].pos_x;
	rad.p2.pos_y = players[1].pos_y;

	for(int i = 0; i < max_move; i++) {

//		fprintf(stderr, "%d\n", i);
		if(crt_move != i) {
			i = crt_move;
		}
		calc_arbore(&rad, board, M, N, 0);
		int next_move = get_movement(&rad);
		sprintf(buffer, "%d%d", crt_move, next_move);

		write(sockfd, buffer, 8);
	}

	

	return 0;
}

