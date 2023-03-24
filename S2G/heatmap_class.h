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

	void SetHeatMap(auto & map) {
		map_size = map.GetMapInfo();
		for (int i = 0; i < map.GetMapInfo().first; i++) {
			std::vector<int32> h_vec;
			for (int j = 0; j < map.GetMapInfo().second; j++) {
				std::pair<int, int> cell = std::make_pair(i, j);

				if(map.GetMapCell(cell) == map.GetMapVal("."))
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

	void UpdateHeatMap(int32 x, int32 y, int32 temp, std::queue<std::pair<int32, int32>> &h_queue, std::vector<bool> &v_cell) {
		std::pair<int32, int32> next_pos;

		if (temp == 0)
			temp = 1;

		if (heatmap.at(y).at(x) != temp_map.at("#")) {
			next_pos = std::make_pair(y, x);
			h_queue.push(next_pos);
			v_cell.at(x * y) = true;
			heatmap.at(y).at(x) = temp;
		}
	}

public:
	HeatMap(auto & map) {
		SetHeatMap(map);
	}

	void ChangeHeatMap(auto &map) {
		SetHeatMap(map);
	}

	int32 GetCellTemp(std::pair<int, int>& pair) {
		return heatmap.at(pair.first).at(pair.second);
	}

	int32 GetTempVal(std::string str) {
		return temp_map.at(str);
	}

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

	void CalcHeatMap(std::pair<int, int> pos, int32 temp, int32 distance) {
		std::queue<std::pair<int32, int32>> heat_queue;
		std::pair<int32, int32> heat_pos;
		std::vector<bool> visitted_cell(map_size.first * map_size.second, false);
		int32 x = 0, y = 0, next_x=0, next_y=0, search_dis=0;
		double num = 0.0;

		// 幅優先探索
		//　初期条件
		if (distance > 5)
			distance = 5;

		std::tie(y, x) = pos;
		heatmap.at(y).at(x) = temp;
		heat_pos = std::make_pair(y, x);
		heat_queue.push(heat_pos);
		visitted_cell.at(x * y) = true;

		// ヒートマップ更新処理
		while (!heat_queue.empty()) {
			// キューからマスを取り出す
			heat_pos = heat_queue.front();
			heat_queue.pop();
			std::tie(y, x) = heat_pos;

			num = std::pow((y - pos.first), 2) + std::pow((x - pos.second), 2);
			search_dis = std::sqrt(num);

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

	void DebugConsole() {
		for (auto h_vec: heatmap) {
			std::string str = "";
			for (auto ele : h_vec) {
				str += std::to_string(ele);
				str += " ";
			}
			const String text = Unicode::Widen(str);
			Console << text;
		}
	}
};
