/*
* ヒートマップクラス
*/
#pragma once
#include <Siv3D.hpp>
#include<math.h>

class HeatMap {
	std::vector<std::vector<int32>> heatmap;
	std::map<std::string, int32> temp_map = {
		{".", 1},
		{"#", 0},
		{"g", -5},
		{"s", -5},
	};
	std::pair<int32, int32> map_size;

	//　ステージ全体の温度情報を取得
	void SetHeatMap(auto& map) {
		map_size = map.GetMapInfo();
		for (int i = 0; i < map.GetMapInfo().first; i++) {
			std::vector<int32> h_vec;
			for (int j = 0; j < map.GetMapInfo().second; j++) {
				std::pair<int, int> cell = std::make_pair(i, j);

				if (map.GetMapCell(cell) == map.GetMapVal("."))
					h_vec.push_back(temp_map.at("."));

				if (map.GetMapCell(cell) == map.GetMapVal("#"))
					h_vec.push_back(temp_map.at("#"));

				if (map.GetMapCell(cell) == map.GetMapVal("g"))
					h_vec.push_back(temp_map.at("g"));

				if (map.GetMapCell(cell) == map.GetMapVal("s"))
					h_vec.push_back(temp_map.at("s"));
			}
			heatmap.push_back(h_vec);
		}
	}

	//　近くにある最も低い温度の影響を受ける
	void ColdHeatMap(int32 x, int32 y) {
		int32 cold_temp = -1, max_cold = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				if (i == 0 && j == 0)
					continue;

				cold_temp = heatmap.at(y + i).at(x + j);
				if (cold_temp < max_cold)
					max_cold = cold_temp;

				cold_temp = heatmap.at(y - i).at(x - j);
				if (cold_temp < max_cold)
					max_cold = cold_temp;
			}
		}
		heatmap.at(y).at(x) += max_cold;
	}

	// あるマスの温度情報を更新する
	void UpdateHeatMap(int32 x, int32 y, int32 temp, std::queue<std::pair<int32, int32>>& h_queue, std::vector<bool>& v_cell) {
		std::pair<int32, int32> next_pos;
		int32 cold_temp = -1, max_cold = 0;

		if (temp == 0)
			temp = 1;

		if (heatmap.at(y).at(x) != temp_map.at("#")) {
			next_pos = std::make_pair(y, x);
			h_queue.push(next_pos);
			v_cell.at(x * y) = true;
			heatmap.at(y).at(x) = temp;

			ColdHeatMap(x, y);

			if (heatmap.at(y).at(x) < -5)
				heatmap.at(y).at(x) = -5;

			if (heatmap.at(y).at(x) > 5)
				heatmap.at(y).at(x) = 5;

			if (heatmap.at(y).at(x) == 0)
				heatmap.at(y).at(x) = -1;
		}
	}

public:
	// インスタンス生成用コンストラクタ
	HeatMap(auto& map) {
		SetHeatMap(map);
	}

	// 他クラスで変数宣言する用のコンストラクタ
	HeatMap() {
	}

	// #----- パラメータ(メンバ変数)取得 -----#
	int32 GetCellTemp(std::pair<int, int>& pair) {
		return heatmap.at(pair.first).at(pair.second);
	}

	int32 GetCellTemp(int32 y, int32 x) {
		return heatmap.at(y).at(x);
	}

	int32 GetTempVal(std::string str) {
		return temp_map.at(str);
	}

	// #----- パラメータ変更 -----#
	void ChangeHeatMap(auto& map) {
		SetHeatMap(map);
	}

	// #----- アクション処理 -----#
	//　ステージの温度は空間(".")なら1に、安全地帯なら-1に収束する
	void CoolHeatMap() {
		for (int32 i = 0; i < map_size.first; i++) {
			for (int32 j = 0; j < map_size.second; j++) {
				if (heatmap.at(i).at(j) == 0 || heatmap.at(i).at(j) == 1)
					continue;

				if (heatmap.at(i).at(j) > 1)
					heatmap.at(i).at(j)--;

				if (heatmap.at(i).at(j) < 0)
					heatmap.at(i).at(j)++;
			}
		}
	}

	//　ステージの空間(".")の温度を1にリセットする
	void RestHeatMap() {
		for (int32 i = 0; i < map_size.first; i++) {
			for (int32 j = 0; j < map_size.second; j++) {
				if (heatmap.at(i).at(j) == 0 || heatmap.at(i).at(j) == 1)
					continue;

				if (heatmap.at(i).at(j) != 0)
					heatmap.at(i).at(j) = 1;
			}
		}
	}

	// 熱源によるヒートマップを計算する
	void CalcHeatMap(std::pair<int, int> pos, int32 temp, int32 distance) {
		std::queue<std::pair<int32, int32>> heat_queue;
		std::pair<int32, int32> heat_pos;
		std::vector<bool> visitted_cell(map_size.first * map_size.second, false);
		int32 x = 0, y = 0, next_x = 0, next_y = 0, search_dis = 0;
		double num = 0.0;

		// 幅優先探索
		//　初期条件
		if (distance > 5)  // 熱源が影響を与えるユークリッド距離は5を最大値とする
			distance = 5;
		std::tie(y, x) = pos;
		heatmap.at(y).at(x) = temp;
		heat_pos = std::make_pair(y, x);
		heat_queue.push(heat_pos);
		visitted_cell.at(x * y) = true;   // 2進数で処理すれば軽量化できる

		// ヒートマップ更新処理
		while (!heat_queue.empty()) {
			// キューからマスを取り出す
			heat_pos = heat_queue.front();
			heat_queue.pop();
			std::tie(y, x) = heat_pos;

			num = std::pow((y - pos.first), 2) + std::pow((x - pos.second), 2);
			search_dis = std::sqrt(num);

			//　探索限界距離まで来たら更新を終える
			if (search_dis >= distance)
				break;

			// 取り出したマスから移動可能なマスを探索する
			// 縦・横の周囲4マスを探索する
			next_x = x + 1;
			next_y = y;
			// 探索済みのマスでないなら処理を行う
			if (!visitted_cell.at(next_x * next_y)) {
				UpdateHeatMap(next_x, next_y, temp - search_dis, heat_queue, visitted_cell);
			}

			next_x = x - 1;
			next_y = y;
			if (!visitted_cell.at(next_x * next_y)) {
				UpdateHeatMap(next_x, next_y, temp - search_dis, heat_queue, visitted_cell);
			}

			next_x = x;
			next_y = y + 1;
			if (!visitted_cell.at(next_x * next_y)) {
				UpdateHeatMap(next_x, next_y, temp - search_dis, heat_queue, visitted_cell);
			}

			next_x = x;
			next_y = y - 1;
			if (!visitted_cell.at(next_x * next_y)) {
				UpdateHeatMap(next_x, next_y, temp - search_dis, heat_queue, visitted_cell);
			}
		}
	}
};
