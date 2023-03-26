/*
* 敵クラスの実装
*/
#pragma once
#include <fstream>

class Enemy {
	std::pair<int, int> enemy_pos;
	std::queue<int> move_queue;
	std::map<std::string, int> order_map = {
		{"l", 0},
		{"r", 1},
		{"u", 2},
		{"d", 3},
	};
	// 行動間隔（秒）
	double spawn_time = 1.0;
	// 蓄積された時間（秒）
	double accumulator = 0.0;
	// プレイヤ追跡判定
	bool flag_chase = false;

	// 移動可能判定
	bool CheckMove(int cell, int wall) {
		if (cell == wall)
			return true;
		return false;
	}

	//　行動ルールがcsvファイルで与えられるとき、読み込んでQueueを生成する
	void LoadMoveQueueCsv(std::string& path, std::string select) {
		std::ifstream csv(path);
		if (not csv)
			throw Error{ U"Failed to load Map csv" };

		std::string line;
		std::string order;

		while (std::getline(csv, line)) {
			std::istringstream i_stream(line);
			bool flag_select = false;

			while (std::getline(i_stream, order, ',')) {
				// 先頭の文字は数字なので、一度無視する
				if (flag_select) {
					move_queue.push(order_map.at(order));
				}

				// 先頭の数字が指定されたものであるなら、その行の行動ルールでQueueを生成する
				if (order == select)
					flag_select = true;
				else
					flag_select = false;
			}
		}
	}

public:
	// インスタンス生成用コンストラクタ
	Enemy(std::pair<int, int> &e_pos) {
		enemy_pos = e_pos;
	}

	// 他クラスで変数宣言する用のコンストラクタ
	Enemy() {
	}

	// #----- パラメータ(メンバ変数)取得 -----# 
	std::pair<int, int> GetPos() {
		return enemy_pos;
	}

	double GetSpawnTime() {
		return spawn_time;
	}

	bool GetChaseFlag() const {
		return flag_chase;
	}

	// #----- パラメータ変更 -----#
	void AddAccumulator(double time) {
		accumulator += time;
	}

	void SetMoveQueue(std::string& path, std::string select) {
		LoadMoveQueueCsv(path, select);
	}

	// #----- アクション処理 -----#
	bool CanMove() {
		if (accumulator >= spawn_time) {
			accumulator -= spawn_time;
			return true;
		}
		return false;
	}

	// Queueで行動する
	void MoveQueue() {
		int order = move_queue.front();

		if (order == order_map.at("l")){
			enemy_pos.second -= 1;
		}

		if (order == order_map.at("r")){
			enemy_pos.second += 1;
		}

		if (order == order_map.at("u")){
			enemy_pos.first -= 1;
		}

		if (order == order_map.at("d")){
			enemy_pos.first += 1;
		}

		move_queue.pop();
		move_queue.push(order);
	}

	// ヒートマップで行動する
	void MoveHeatMap(auto& heatmap, auto& map) {
		int x = 0, y = 0;
		std::tie(y, x) = enemy_pos;
		int32 next_y = 0, next_x = 0, max_temp = 1;

		// 周囲4マスの中で最も温度が高い方向へ移動する
		if (heatmap.GetCellTemp(y, x + 1) > max_temp) {
			next_y = y;
			next_x = x + 1;
			max_temp = heatmap.GetCellTemp(y, x + 1);
		}

		if (heatmap.GetCellTemp(y, x - 1) > max_temp) {
			next_y = y;
			next_x = x - 1;
			max_temp = heatmap.GetCellTemp(y, x - 1);
		}

		if (heatmap.GetCellTemp(y + 1, x) > max_temp) {
			next_y = y + 1;
			next_x = x;
			max_temp = heatmap.GetCellTemp(y + 1, x);
		}

		if (heatmap.GetCellTemp(y - 1, x) > max_temp) {
			next_y = y - 1;
			next_x = x;
			max_temp = heatmap.GetCellTemp(y - 1, x);
		}

		// 温度が均一の場合はランダムで移動する
		if (max_temp == 1) {
			spawn_time = 1.0;
			flag_chase = false;
			while (true) {
				int32 add_y = Random(-1, 1);
				int32 add_x = 0;

				if (add_y == 0)
					add_x = Random(-1, 1);

				next_y = y + add_y;
				next_x = x + add_x;

				std::pair<int32, int32> next_pos = std::make_pair(next_y, next_x);
				if (!CheckMove(map.GetMapCell(next_pos), map.GetMapVal("#")))
					break;
			}
		}
		else {
			// Player追跡時は移動速度を速くする
			spawn_time = 0.5;
			flag_chase = true;
		}
		// 座標を移動先の座標に更新する
		enemy_pos = std::make_pair(next_y, next_x);
	}
};
