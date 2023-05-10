/*
* プレイヤクラス
*/
#pragma once

class Player {
	std::pair<int32, int32> player_pos;
	std::pair<int32, int32> player_next_pos;
	int32 temp = 5;
	int32 temp_distance = 5;
	double walk_speed = 2.5;
	// 方向: (下: 0, 左: 1, 右: 2, 上: 3) 
	int32 direction = 0;

	// 移動可能かの判定を行う
	bool CheckMove(int32 cell, int32 wall) {
		if (cell == wall)
			return true;
		return false;
	}

public:
	// コンストラクタ
	Player(std::pair<int32, int32> &p_pos) {
		player_pos = p_pos;
		player_next_pos = p_pos;
	}

	// #----- パラメータ(メンバ変数)取得 -----# 
	int32 GetTemp() {
		return temp;
	}

	int32 GetTempDistance() {
		return temp_distance;
	}

	std::pair<int32, int32> GetPos() {
		return player_pos;
	}

	std::pair<int32, int32> GetNextPos() {
		return player_next_pos;
	}

	bool CanWalk() {
		if (player_next_pos == player_pos)
			return false;
		return true;
	}

	double GetWalkSpeed() {
		return walk_speed;
	}

	int32 GetDirection() {
		return direction;
	}

	// #----- パラメータ変更 -----#
	void SetPos(std::pair<int32, int32>& pos) {
		player_pos = pos;
		player_next_pos = pos;
	}

	void SetDirection(int32 dir) {
		direction = dir;
	}

	// #----- アクション処理 -----# 
	//　入力を受け取り、座標を変更する。移動先が壁だった場合は変更を取り消す
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

	void Move2(int step, std::string dir, auto& map) {
		if (dir == "Left") {
			player_next_pos.second -= 1;
			direction = 1;
			if (CheckMove(map.GetMapCell(player_next_pos), map.GetMapVal("#")))
				player_next_pos.second += 1;
		}

		if (dir == "Right") {
			player_next_pos.second += 1;
			direction = 2;
			if (CheckMove(map.GetMapCell(player_next_pos), map.GetMapVal("#")))
				player_next_pos.second -= 1;
		}

		if (dir == "Up") {
			player_next_pos.first -= 1;
			direction = 3;
			if (CheckMove(map.GetMapCell(player_next_pos), map.GetMapVal("#")))
				player_next_pos.first += 1;
		}

		if (dir == "Down") {
			player_next_pos.first += 1;
			direction = 0;
			if (CheckMove(map.GetMapCell(player_next_pos), map.GetMapVal("#")))
				player_next_pos.first -= 1;
		}
	}

	void ArrivePos() {
		player_pos = player_next_pos;
	}
};
