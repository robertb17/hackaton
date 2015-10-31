

struct player {
	short player_id;
	short pos_x;
	short pos_y;
};

struct game_board {
	bool is_wall;
	short danger_level; // 0 -> 100
	char on_fire;
	char has_bomb;
};

void update_state(game_board );
bool get_info(char*, int&, int&, int, int&, int&);
game_board** trans_input(char*, player*, int, int);
