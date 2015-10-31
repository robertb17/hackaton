#include "data_str.h"
#include <queue>
#include <algorithm>

using namespace std;

#define D_LEVEL 70
/*matrice, player1, player2, nr linii, nr coloana*/
void bfs(game_board map[32][32], player p1, player p2, int m, int n){
		
	
	/*vectori de vecini*/
	short int vx[] = {0, 1, 0, -1};
	short int vy[] = {-1, 0, 1, 0};

	queue< pair<short int, short int> > qe;
	qe.push(make_pair(p1.pos_x, p1.pos_y)); /*adaug sursa in coada*/
	
	short int dist_map[32][32];/*matrice cu distante*/
	memset(&dist_map[0][0], 0, sizeof(&dist_map[0][0]));

	int pas = 0; // distanta de la destinatie la celule
	while(!qe.empty()){
		pair<short int, short int> cel = qe.front();
		pas++;		
		for(short int i = 0; i < 4; i++){
			if(!map[cel.x + i][cel.y + i].is_wall && map[cel.x+i][cel.y + i].danger_level <= 70){
				dist_map[cel.x + i][cel.y + i] = pas;
				new_pair = make_pair(cel.x + i, cel.y + i);
				qe.push(new_pair);
			}
		}
	
		qe.pop();
	}


	for(short int i = 0; i < m; i++ ){
	
		
	}

}

int main(){

	return 0;
}
