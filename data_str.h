#include <vector>

struct player {
	short player_id;
	short pos_x;
	short pos_y;
};

struct nod {
	short eff1;
	short eff2;
	std::vector<nod> v_nod;
	player p1;
	player p2;
};

struct game_board {
	bool is_wall;
	char on_fire;
	char time_left_bomb;
};

void update_state(game_board );
bool get_info(char*, int&, int&, int&, int&, int&);
game_board** trans_input(char*, player*, int, int);
player get_movement(nod n);
void calc_arbore(nod&, game_board**, int, int, int);