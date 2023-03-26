/*
* プレイヤクラス
*/
#pragma once

class Player {
	std::pair<int, int> player_pos;
	int32 temp = 5;
	int32 temp_distance = 5;

	// 移動可能かの判定を行う
	bool CheckMove(int cell, int wall) {
		if (cell == wall)
			return true;
		return false;
	}

public:
	// インスタンス生成用コンストラクタ
	Player(std::pair<int, int> &p_pos) {
		player_pos = p_pos;
	}

	// 他クラスで変数宣言する用のコンストラクタ
	Player() {
		player_pos = { 0, 0 };
	}

	// #----- パラメータ(メンバ変数)取得 -----# 
	int32 GetTemp() {
		return temp;
	}

	int32 GetTempDistance() {
		return temp_distance;
	}

	std::pair<int, int> GetPos() {
		return player_pos;
	}

	// #----- パラメータ変更 -----#
	void SetPos(std::pair<int, int>& pos) {
		player_pos = pos;
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
};
