#include "data_str.h"
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#define INF 100


const short int mov[10][5] = { 
	{0, 0, 0, 0, 0},	//stanga, dreapta, sus, jos, bomba
	{1, 0, 0, 0, 0},
	{0, 1, 0, 0, 0},	
	{0, 0, 1, 0, 0},
	{0, 0, 0, 1, 0},
	{0, 0, 0, 0, 1}
	//{1, 0, 0, 0, 1},
	//{0, 1, 0, 0, 1},
	//{0, 0, 1, 0, 1},
	//{0, 0, 0, 1, 1}
};

/*veridica daca punctul e pe harta*/
bool on_board(int x, int y, int M, int N){
	if(x >= 0 && x < N)
		if(y >= 0 && y  < M)
			return true;

	return false;
}

/*verifica daca mutarea e posibila*/
bool is_possible(game_board** board, int x, int y, int N, int M)
{

	if(!on_board(x, y, N, M))
		return false;
	if(board[x][y].is_wall == false &&
	   board[x][y].on_fire == 0 &&
	   board[x][y].time_left_bomb == 0) {
	   	return true;
	}
	return false;
}

/*updateaza casutele dupa explozie*/
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

/*creeaza o harta care e un update al hartii primite */
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

/*calculeaza arborele*/
void calc_arbore(nod* n_crt, game_board** board, int N, int M, int pas)
{
	game_board** new_board = update_board(board, N, M);
	if(pas >= 1) {
		//eff = 0 cuz ded
		if( board[n_crt->p1.pos_x][n_crt->p1.pos_y].on_fire > 0 ) {
			n_crt->eff1 = 0;
		} else {
			//eff = rounds till ded
			n_crt->eff1 = board[n_crt->p1.pos_x][n_crt->p1.pos_y].time_left_bomb;
			
			//maximum eff
			if(n_crt->eff1 == 0) {
				n_crt->eff1 = INF;
			}
		}

		if( board[n_crt->p2.pos_x][n_crt->p2.pos_y].on_fire > 0 ) {
			n_crt->eff2 = 0;
		} else {
			//eff = rounds till ded
			n_crt->eff2 = board[n_crt->p2.pos_x][n_crt->p2.pos_y].time_left_bomb;
			
			//maximum eff
			if(n_crt->eff2 == 0) {
				n_crt->eff2 = INF;
			}
		}

	} else {
		
		//iteram prin matricea movement
		for(int i = 0; i < 10; i++) {
			for(int j = 0; j < 6; j++) {
				nod *n = new nod;
				n->p1.pos_x = n_crt->p1.pos_x - mov[i][0] + mov[i][1];
				n->p1.pos_y = n_crt->p1.pos_y - mov[i][2] + mov[i][3];

				n->p2.pos_x = n_crt->p2.pos_x - mov[j][0] + mov[j][1];
				n->p2.pos_y = n_crt->p2.pos_y - mov[j][2] + mov[j][3];

		
				//fprintf(stderr, "%d %d %d %d\n",n->p1.pos_x, n->p1.pos_y, n->p2.pos_x, n->p2.pos_y);

				if(is_possible(new_board, n->p1.pos_x, n->p1.pos_y, N, M) &&
				   is_possible(new_board, n->p2.pos_x, n->p2.pos_y, N, M)) {

					n_crt->v_nod.push_back(n);
					//plants bomb
					if(mov[i][4] == 1){
						new_board[n_crt->p1.pos_x][n_crt->p1.pos_y].time_left_bomb = 10;
						n->plant_bomb = true;
					}
					if(mov[j][4] == 1){
						new_board[n_crt->p2.pos_x][n_crt->p2.pos_y].time_left_bomb = 10;
					}

				}
				//fprintf(stderr,"%d %d\n", i, j);
			
				
				//delete[] n;
				//fprintf(stderr, "%lu\n", n_crt->v_nod.size());
			}
		}
 
		//solve nodes
		for(size_t i = 0; i < n_crt->v_nod.size(); i++) {
			//fprintf(stderr, "%lu %lu \n", i, n_crt->v_nod.size());
			calc_arbore(n_crt->v_nod[i], new_board, N, M, pas + 1);
		
		}

		//calc best solution
		//free(new_board);
		
		if (n_crt->v_nod.size()){	
			int min_eff1 = n_crt->v_nod[0]->eff1;
			int max_eff2 = n_crt->v_nod[0]->eff2;
			for (size_t i = 1; i < n_crt->v_nod.size(); i++) {
				if(min_eff1 > n_crt->v_nod[i]->eff1) {
					min_eff1 = n_crt->v_nod[i]->eff1;
				}
				if(max_eff2 < n_crt->v_nod[i]->eff2) {
					max_eff2 = n_crt->v_nod[i]->eff2;
				}
			}
			n_crt->eff1 = min_eff1;
			n_crt->eff2 = max_eff2;
		}else{
			int min_eff1 = 0;
			int max_eff1 = 0;
		}	
		n_crt->v_nod.clear();
	}
}

/*returneaza mutarea cea mai eficienta*/
int get_movement(nod *n)
{
	int index = 0;
	int max_eff1 = n->v_nod[0]->eff1;
	int min_eff2 = n->v_nod[0]->eff2;
	for(size_t i = 1; i < n->v_nod.size(); i++) {
		if(max_eff1 < n->v_nod[i]->eff1) {
			max_eff1 = n->v_nod[i]->eff1;
			index = i;
			min_eff2 = n->v_nod[i]->eff2;
		} else if (max_eff1 == n->v_nod[i]->eff1) {
			if(min_eff2 < n->v_nod[i]->eff2) {
				index = i;
				min_eff2 = n->v_nod[i]->eff2;
			}
		}
	}
	int dif_row = n->v_nod[index]->p1.pos_x - n->p1.pos_x;	
	int dif_col = n->v_nod[index]->p1.pos_y - n->p1.pos_y;

	int res = 0;
	if(dif_row > 0) {
		res += 0x01000000;
		//res += 1;
	} else if(dif_row < 0) {
		res += 0x03000000;
		// res += 3;
	} else if(dif_col > 0) {
		res += 0x02000000;
		// res += 2;
	} else if(dif_col < 0) {
		res += 0x04000000;
		// res += 4;
	}

	if(n->plant_bomb == true) {
		res += 1;
		// res += 0x80000000;
	}	

	return res;
}

