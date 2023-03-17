#pragma once
#include <fstream>
#include <sstream>

class Map {
	std::vector<std::vector<int>> map;
	std::pair<int, int> player_pos = { 0, 0 };
	std::pair<int, int> goal_pos = { 0, 0 };
	std::vector<std::pair<int, int>> enemys_pos;
	std::map<std::string, int> map_val = {
		{".", 0},
		{"#", 1},
		{"s", 2},
		{"g", 3},
		{"e", 5},
	};

	void LoadMapCsv(std::string &path) {
		std::ifstream csv(path);
		if (not csv)
			throw Error{ U"Failed to load Map csv" };

		std::string line;
		std::string map_data;
		int i = 0;

		while (std::getline(csv, line)) {
			std::istringstream i_stream(line);
			std::vector<int> m;
			int j = 0;

			while (std::getline(i_stream, map_data, ',')) {
				if (map_data == "s") {
					player_pos.first = i;
					player_pos.second = j;
				}

				if (map_data == "g") {
					goal_pos.first = i;
					goal_pos.second = j;
				}

				if (map_data == "e") {
					enemys_pos.push_back({ i, j });
				}

				m.push_back(map_val.at(map_data));
				j++;
			}
			i++;
			map.push_back(m);
		}
	}

	bool CheckMove() {
		int cell = map.at(player_pos.first).at(player_pos.second);
		if (cell == map_val.at("#"))
			return true;
		return false;
	}

public:

	Map(std::string& path){
		LoadMapCsv(path);
	}

	bool CheckClear() {
		if (map.at(player_pos.first).at(player_pos.second) == map_val.at("#"))
			return true;
		return false;
	}

	bool CheckOver() {
		if (map.at(player_pos.first).at(player_pos.second) == map_val.at("e"))
			return true;
		return false;
	}

	void MovePlayer(int step, std::string dir) {
		if (dir == "Left") {
			player_pos.second -= 1;
			if(CheckMove())
				player_pos.second += 1;
			DebugConsole();
		}

		if (dir == "Right") {
			player_pos.second += 1;
			if (CheckMove())
				player_pos.second -= 1;
			DebugConsole();
		}

		if (dir == "Up") {
			player_pos.first -= 1;
			if (CheckMove())
				player_pos.first += 1;
			DebugConsole();
		}

		if (dir == "Down") {
			player_pos.first += 1;
			if (CheckMove())
				player_pos.first -= 1;
			DebugConsole();
		}
	}

	void DebugConsole() {
		Print << player_pos.first;
		Print << player_pos.second;
		int i = 0;

		for (auto vec : map) {
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

				if (s == map_val.at("e"))
					str += "e";

				if (i == player_pos.first && j == player_pos.second) {
					str.pop_back();
					str += "p";
				}

				j++;
			}
			const String text = Unicode::Widen(str);
			Console << text;

			i++;
		}
	}
};
