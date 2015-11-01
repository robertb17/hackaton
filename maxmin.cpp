#include "data_str.h"
#include <stdlib.h>
#include <cstring>
#define INF 100


const short int mov[10][5] = { 
	{0, 0, 0, 0, 0},	//stanga, dreapta, sus, jos, bomba
	{1, 0, 0, 0, 0},
	{0, 1, 0, 0, 0},	
	{0, 0, 1, 0, 0},
	{0, 0, 0, 1, 0},
	{0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1},
	{0, 1, 0, 0, 1},
	{0, 0, 1, 0, 1},
	{0, 0, 0, 1, 1}
};

bool is_possible(game_board** board, int x, int y)
{
	if(board[x][y].is_wall == false &&
	   board[x][y].on_fire == false &&
	   board[x][y].time_left_bomb == 0) {
	   	return true;
	}
	return false;
}

void update_flames(game_board** board, int x, int y) {
	for(int i = 0; i < 3; i++) {

		//left
		if(board[x-i][y].is_wall == true) {
			break;
		} else {
			board[x-i][y].on_fire = 3;
			if(board[x-i][y].time_left_bomb > 1) {
				board[x-i][y].time_left_bomb = 1;
			}
		}

		//right
		if(board[x+i][y].is_wall == true) {
			break;
		} else {
			board[x+i][y].on_fire = 3;
			if(board[x+i][y].time_left_bomb > 1) {
				board[x+i][y].time_left_bomb = 1;
			}
		}

		//up
		if(board[x][y-i].is_wall == true) {
			break;
		} else {
			board[x][y-i].on_fire = 3;
			if(board[x][y-i].time_left_bomb > 1) {
				board[x][y-i].time_left_bomb = 1;
			}
		}

		//down
		if(board[x][y+i].is_wall == true) {
			break;
		} else {
			board[x][y+i].on_fire = 3;
			if(board[x][y+i].time_left_bomb > 1) {
				board[x][y+i].time_left_bomb = 1;
			}
		}
	}
}

game_board** update_board(game_board** board, int N, int M)
{
	game_board** new_board = (game_board**) malloc(sizeof(game_board*) * N);
	for (int i = 0; i < N ; i++) {
		board[i] = (game_board*) malloc(sizeof(game_board) * M);
	}
	memcpy(new_board, board, sizeof(game_board) * N * M);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {

			//handle flames
			if (new_board[i][j].on_fire > 1) {
				new_board[i][j].on_fire--;
			}

			//handle bombs			
			if (new_board[i][j].time_left_bomb > 1) {
				new_board[i][j].time_left_bomb--;
				//explode bombs
				if(new_board[i][j].time_left_bomb == 0) {
					update_flames(new_board, i, j);
				}
			}
		}
	}
	return new_board;
}


void calc_arbore(nod& n_crt, game_board** board, int N, int M, int pas)
{
	game_board** new_board = update_board(board, N, M);
	if(pas == 5) {
		//eff = 0 cuz ded
		if( board[n_crt.p1.pos_x][n_crt.p1.pos_y].on_fire > 0 ) {
			n_crt.eff1 = 0;
		} else {
			//eff = rounds till ded
			n_crt.eff1 = board[n_crt.p1.pos_x][n_crt.p1.pos_y].time_left_bomb;
			
			//maximum eff
			if(n_crt.eff1 == 0) {
				n_crt.eff1 = INF;
			}
		}

		if( board[n_crt.p2.pos_x][n_crt.p2.pos_y].on_fire > 0 ) {
			n_crt.eff2 = 0;
		} else {
			//eff = rounds till ded
			n_crt.eff2 = board[n_crt.p2.pos_x][n_crt.p2.pos_y].time_left_bomb;
			
			//maximum eff
			if(n_crt.eff2 == 0) {
				n_crt.eff2 = INF;
			}
		}

	} else {
		
		//iteram prin matricea movement
		for(int i = 0; i < 10; i++) {
			for(int j = 0; j < 10; j++) {
				nod* n = new nod[5];
				for(int k = 0; k < 5; k++) {
					n[k].p1.pos_x += n_crt.p1.pos_x + mov[i][0] * (-1) + mov[i][1];
					n[k].p1.pos_y += n_crt.p1.pos_y + mov[i][2] * (-1) + mov[i][3];

					n[k].p2.pos_x += n_crt.p2.pos_x + mov[j][0] * (-1) + mov[j][1];
					n[k].p2.pos_y += n_crt.p2.pos_y + mov[j][2] * (-1) + mov[j][3];

					if(is_possible(new_board, n[k].p1.pos_x, n[k].p1.pos_y)) {
						n_crt.v_nod.push_back(n[k]);
					}
				}
				delete n;
			}
		}

		//solve nodes
		for(size_t i = 0; i < n_crt.v_nod.size(); i++) {
			calc_arbore(n_crt.v_nod[i], new_board, N, M, ++pas);
		}

		//calc best solution
		free(new_board);
		int min_eff1 = n_crt.v_nod[0].eff1;
		int max_eff2 = n_crt.v_nod[0].eff2;
		for (size_t i = 1; i < n_crt.v_nod.size(); i++) {
			if(min_eff1 > n_crt.v_nod[i].eff1) {
				min_eff1 = n_crt.v_nod[i].eff1;
			}
			if(max_eff2 < n_crt.v_nod[i].eff2) {
				max_eff2 = n_crt.v_nod[i].eff2;
			}
		}
		n_crt.eff1 = min_eff1;
		n_crt.eff2 = max_eff2;

		n_crt.v_nod.clear();
	}
}

player get_movement(nod n)
{
	int index = 0;
	int max_eff1 = n.v_nod[0].eff1;
	int min_eff2 = n.v_nod[0].eff2;
	for(size_t i = 1; i < n.v_nod.size(); i++) {
		if(max_eff1 < n.v_nod[i].eff1) {
			max_eff1 = n.v_nod[i].eff1;
			index = i;
			min_eff2 = n.v_nod[i].eff2;
		} else if (max_eff1 == n.v_nod[i].eff1) {
			if(min_eff2 < n.v_nod[i].eff2) {
				index = i;
				min_eff2 = n.v_nod[i].eff2;
			}
		}
	}

	return n.v_nod[index].p1;
}

