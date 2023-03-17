#pragma once
#include "enemy_class.h"
#include "map_class.h"
#include "player_class.h"

class GameMaster {
public:
	bool ChceckCollisionP_E(std::pair<int, int>& player_pos, std::vector<Enemy>& enemies) {
		for (Enemy enem : enemies) {
			std::pair<int, int> enemy_pos = enem.GetPos();
			if (player_pos == enemy_pos)
				return true;
		}
		return false;
	}
};
