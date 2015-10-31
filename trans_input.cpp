#include "data_str.h"
#include <string.h>
#include <stdlib.h>

void handle_player(player* x, char* p, int i, int j)
{
	if(x[0].player_id == 0) {
		x[0].player_id = *((int*)p);
		x[0].pos_x = j;
		x[0].pos_y = i;
	} else {
		x[1].player_id = *((int*)p);
		x[1].pos_x = j;
		x[1].pos_y = i;
	}
}

int* get_new_int(char* s, int l)
{
	//asumming little endian
	int* p = (int*) (s+ l - 32);
	s[l-32] = '\0';
	return p;

}

char* get_new_char(char* s, int l)
{
	//asumming little endian
	char* p = (char*) (s + l - 8);
	s[l-8] = '\0';
	return p;
}

bool get_info(char* s, int& crt_move, int& max_move, int& aggressive, int& N, int& M)
{
	int* p = get_new_int(s, strlen(s));
	crt_move = *p;
	
	p = get_new_int(s, strlen(s));
	aggressive = *p;

	p = get_new_int(s, strlen(s));
	max_move = *p;

	p = get_new_int(s, strlen(s));
	N = *p;

	p = get_new_int(s, strlen(s));
	M = *p;

	return (crt_move >= aggressive) ? true : false;
}

game_board** trans_input(char* s, player* players, int N, int M)
{
	

	game_board** board = (game_board**) malloc(sizeof(game_board*) * N);
	for(int i = 0; i < N; i++) {
		board[i] = (game_board*) malloc(sizeof(game_board) * M);
		for(int j = 0; j < M; j++) {
			char* pp = get_new_char(s, strlen(s));
			if(*pp != 0) {
				handle_player(players, pp, i, j);
			}
			pp = get_new_char(s, strlen(s)); 
			board[i][j].is_wall = *pp;
			board[i][j].danger_level = 0;

			pp = get_new_char(s, strlen(s)); 
			board[i][j].on_fire = *pp;

			pp = get_new_char(s, strlen(s)); 
			board[i][j].has_bomb =	*pp;	
		}
	}

	return board;
}