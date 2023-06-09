﻿/*
* マップ(ステージ)クラス
*/
#pragma once
#include <fstream>
#include <sstream>
#include "enemy_class.h"

class Map {
	std::vector<std::vector<int>> map_vec;
	std::pair<int, int> map_info = { 0, 0 };
	std::pair<int, int> start_pos = { 0, 0 };
	std::pair<int, int> goal_pos = { 0, 0 };
	std::vector<std::pair<int, int>> enemys_init_pos;
	std::map<std::string, int> map_val = {
		{".", 0},
		{"#", 1},
		{"s", 2},
		{"g", 3},
	};

	//　csvファイルからマップを数字で生成する
	void LoadMapCsv(std::string &path) {
		std::ifstream csv(path);
		if (not csv)
			throw Error{ U"Failed to load Map csv" };

		std::string line;
		std::string map_data;
		int i = 0, j = 0;

		while (std::getline(csv, line)) {
			std::istringstream i_stream(line);
			std::vector<int> m;
			j = 0;

			while (std::getline(i_stream, map_data, ',')) {
				if (map_data == "s") {
					start_pos.first = i;
					start_pos.second = j;
				}

				if (map_data == "g") {
					goal_pos.first = i;
					goal_pos.second = j;
				}

				if (map_data == "e") {
					enemys_init_pos.push_back({ i, j });
					map_data = ".";
				}

				m.push_back(map_val.at(map_data));
				j++;
			}
			i++;
			map_vec.push_back(m);
		}
		map_info = std::make_pair(i, j);
	}

public:
	// インスタンス生成用コンストラクタ
	Map(std::string &path) {
		LoadMapCsv(path);
	}

	// 他クラスで変数宣言する用のコンストラクタ
	Map() {
	}

	// #----- パラメータ(メンバ変数)取得 -----#
	int GetMapVal(std::string str) const {
		return map_val.at(str);
	}

	int GetMapCell(std::pair<int, int> &pair) const {
		return map_vec.at(pair.first).at(pair.second);
	}

	int GetNumEnemies() {
		return enemys_init_pos.size();
	}

	std::pair<int, int> GetMapInfo() const {
		return map_info;
	}

	std::pair<int, int> GetPos(std::string name, int i=0) {
		if (name == "start")
			return start_pos;
		else if (name == "goal")
			return goal_pos;
		else if (name == "enemy")
			return enemys_init_pos.at(i);
		else
			return std::make_pair(0, 0);
	}

	// #----- アクション処理 -----#
	// クリア判定処理
	bool CheckClear(std::pair<int, int> &player_pos) {
		if (map_vec.at(player_pos.first).at(player_pos.second) == map_val.at("g"))
			return true;
		return false;
	}

	// デバック用　CLI表示
	void ShowConsole(std::pair<int, int> &p_pos, std::map<std::string, std::pair<int, int>> &e_pos_map) {
		Print << U"P [Show]: " << p_pos.first << U", " << p_pos.second;
		for (auto e_pos : e_pos_map) {
			std::string key = e_pos.first;
			const String key_str = Unicode::Widen(key);
			std::pair<int, int> val = e_pos.second;
			Print << key_str << U" [Show]: " << val.first << U", " << val.second;
		}

		int i = 0;

		for (auto vec : map_vec) {
			std::string str = "";
			int j = 0;

			for (auto s : vec) {
				if (s == map_val.at("."))
					str += " ";

				if (s == map_val.at("#"))
					str += "#";

				if (s == map_val.at("s"))
					str += " ";

				if (s == map_val.at("g"))
					str += "g";

				if (i == p_pos.first && j == p_pos.second) {
					str.pop_back();
					str += "p";
				}

				for (auto e_pos : e_pos_map) {
					std::pair<int, int> val = e_pos.second;
					if (i == val.first && j == val.second) {
						str.pop_back();
						str += "e";
					}
				}
				j++;
			}
			const String text = Unicode::Widen(str);
			Console << text;

			i++;
		}
	}
};
