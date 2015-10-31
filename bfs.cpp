#include "data_str.h"
#include <queue>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

#define D_LEVEL 70


bool on_board(short int x, short int y, short int m, short int n){
	if(x >= 0 && x < m)
		if(y >= 0 && y  < n)
			return true;

	return false;
}

/*matrice, player1, player2, nr linii, nr coloana*/
void bfs(game_board map[32][32], player p1, player p2, short int m, short int n){
		
	
	/*vectori de vecini*/
	short int vx[] = {0, 1, 0, -1};
	short int vy[] = {-1, 0, 1, 0};

	queue< pair<short int, short int> > qe;
	qe.push(make_pair(p1.pos_x, p1.pos_y)); /*adaug sursa in coada*/
	
	short int dist_map[32][32];/*matrice cu distante*/
	memset(dist_map, 0, sizeof(dist_map[0][0]) * m * n);

	bool dest_reached = false;

	
	dist_map[p1.pos_x][p1.pos_y] = 1;	
	while(!qe.empty()){
		pair<short int, short int> cel = qe.front();
		if(cel.first == p2.pos_x && cel.second == p2.pos_y){
			dest_reached = true; /*am ajuns la destinatie*/
			break; 
		}
		
		short int pas = dist_map[cel.first][cel.second] + 1;		
		for(short int i = 0; i < 4; i++){
			short pos_x = cel.first + vx[i];
			short pos_y = cel.second + vy[i];
			if (on_board(pos_x, pos_y, m, n)) /*verifica daca sunt pe tabla*/
				if(dist_map[pos_x][pos_y] == 0) /*daca pot scrie in casuta respectiva*/
					if(!map[pos_x][pos_y].is_wall && (map[pos_x][pos_y].danger_level <= D_LEVEL)) {
						dist_map[pos_x][pos_y] = pas;
						pair<short int, short int> new_pair = make_pair(pos_x, pos_y);
						qe.push(new_pair);
					}
		}

		
		qe.pop();
	}


	if(dest_reached){
		vector<pair<short int, short int> > path;
		short int pos_x = qe.front().first;
		short int pos_y = qe.front().second;
		short int val = dist_map[pos_x][pos_y];
	
		path.push_back(qe.front());
		while(dist_map[pos_x][pos_y] != 1){
			for(int i = 0; i < 4; i++){
				short int next_x = pos_x + vx[i];
				short int next_y = pos_y + vy[i];
	
				if(on_board(next_x, next_y, m, n))
					if(dist_map[next_x][next_y] == (val-1)){
						path.push_back(make_pair(next_x, next_y));
						val--;
						pos_x = next_x;
						pos_y = next_y;	
					}
			}		
		}

		reverse(path.begin(), path.end());
		for(unsigned int i = 0; i < path.size(); i++){
			printf("(%hd, %hd) ", path[i].first, path[i].second);
		}
		printf("\n");
	}else{
		printf("Nu s-a ajuns la destinatie\n");
	}
	
	/*
	for(short int i = 0; i < m; i++ ){
		for(short int j = 0; j < n; j++){
			printf("%hd ", dist_map[i][j]);
		}

		printf("\n");
		
	}
	*/
}

int main(){

	FILE *fin = fopen("date.in", "rt");
	

	short int m, n;
	fscanf(fin, "%hd%hd", &m, &n);
	game_board map[32][32];

	for(short int i = 0; i < m; i++){
		for(short int j = 0; j < n; j++){
			short int x;			
			fscanf(fin, "%hd", &x);
			if(x == 1)
				map[i][j].is_wall = true;
			map[i][j].danger_level = 0;
		}
	}	


	player p1, p2;
	p1.pos_x = 0, p1.pos_y = 0;
	p2.pos_x = m-1, p2.pos_y = n-1;
	
	bfs(map, p1, p2, m, n);	
	return 0;
}
