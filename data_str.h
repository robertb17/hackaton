

struct player {
	short player_id;
	short pos_x;
	short pos_y;
};

struct game_board {
	bool is_wall;
	short danger_level; // 0 -> 100
	short on_fire;
};

void update_state(game_board );
