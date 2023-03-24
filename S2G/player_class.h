#pragma once

class Player {
	std::pair<int, int> player_pos;
	int32 temp = 5;
	int32 temp_distance = 5;

	bool CheckMove(int cell, int wall) {
		if (cell == wall)
			return true;
		return false;
	}

public:
	Player(std::pair<int, int> &p_pos) {
		player_pos = p_pos;
	}

	int32 GetTemp() {
		return temp;
	}

	int32 GetTempDistance() {
		return temp_distance;
	}

	void Move(int step, std::string dir, auto &map) {
		if (dir == "Left") {
			player_pos.second -= 1;
			if (CheckMove(map.GetMapCell(player_pos), map.GetMapVal("#")))
				player_pos.second += 1;
		}

		if (dir == "Right") {
			player_pos.second += 1;
			if (CheckMove(map.GetMapCell(player_pos), map.GetMapVal("#")))
				player_pos.second -= 1;
		}

		if (dir == "Up") {
			player_pos.first -= 1;
			if (CheckMove(map.GetMapCell(player_pos), map.GetMapVal("#")))
				player_pos.first += 1;
		}

		if (dir == "Down") {
			player_pos.first += 1;
			if (CheckMove(map.GetMapCell(player_pos), map.GetMapVal("#")))
				player_pos.first -= 1;
		}
	}

	void SetPos(std::pair<int, int> &pos) {
		player_pos = pos;
	}

	std::pair<int, int> GetPos() {
		return player_pos;
	}
};
