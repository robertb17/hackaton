#include "data_str.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void handle_player(player* x, char* p, int i, int j)
{
	if(x[0].player_id == 0) {
		x[0].player_id = *((int*)p);
		x[0].pos_x = i;
		x[0].pos_y = j;
	} else {
		x[1].player_id = *((int*)p);
		x[1].pos_x = i;
		x[1].pos_y = j;
	}
}

int get_new_int(char* s, int l)
{
	//asumming little endian
	char* p = s + l - 1;
	int val = 0;
	for(int i = 0; i <= 3; i++) {
		val += (int)(*((p - i))) * pow(2,8 * (3 - i));
	}
	return val;

}

char* get_new_char(char* s)
{
	//asumming little endian
	char* p = s;
	return p;
}

bool get_info(char* s, int& crt_move, int& max_move, int& aggressive, int& N, int& M)
{
	printf("%s\n", s);
	int p = get_new_int(s, 20);
	N = p;

	printf("%s\n", s);
	
	p = get_new_int(s, 16);
	M = p;

	p = get_new_int(s, 12);
	max_move = p;

	p = get_new_int(s, 8);
	aggressive = p;

	p = get_new_int(s, 4);
	crt_move = p;

	return (crt_move >= aggressive) ? true : false;
}

game_board** trans_input(char* s, player* players, int N, int M)
{
	game_board** board = (game_board**) malloc(sizeof(game_board*) * N);
	for(int i = 0; i < N; i++) {
		board[i] = (game_board*) malloc(sizeof(game_board) * M);
		for(int j = 0; j < M; j++) {
			char* pp = s;
			pp++;

			if(*pp != 0) {
				printf("%d %d\n", i, j);
				handle_player(players, pp, i, j);
			}
			pp++;
			if(*pp == '\0') {
				board[i][j].is_wall = false;
			} else {
				board[i][j].is_wall = true;
			}

			pp++;
			memcpy(&board[i][j].on_fire, pp, sizeof(char));

			pp++;
			memcpy(&board[i][j].time_left_bomb, pp, sizeof(char));

			//printf("%d %d: %d %d %d\n", i, j, (int)board[i][j].is_wall, board[i][j].on_fire, board[i][j].has_bomb);	
		}
	}

	return board;
}
