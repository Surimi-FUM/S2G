#pragma once
#include "enemy_class.h"
#include "map_class.h"
#include "player_class.h"

class GameMaster {
public:
	bool ChceckCollisionP_E(std::pair<int, int>& player_pos, std::map<std::string, std::pair<int, int>>& e_pos_map) {
		for (auto e_pos : e_pos_map) {
			std::pair<int, int> pos = e_pos.second;
			if (player_pos == pos)
				return true;
		}
		return false;
	}
};
