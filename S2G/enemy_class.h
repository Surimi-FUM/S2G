#pragma once

class Enemy {
	std::pair<int, int> enemy_pos;

public:
	Enemy(std::pair<int, int> &pos) {
		enemy_pos = pos;
	}

	std::pair<int, int> GetPos() {
		return enemy_pos;
	}

	void Move() {

	}
};
