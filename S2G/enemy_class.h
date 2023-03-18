#pragma once

class Enemy {
	std::pair<int, int> enemy_pos;
	std::queue<int> move_queue;
	std::map<std::string, int> order_map = {
		{"l", 0},
		{"r", 1},
		{"u", 2},
		{"d", 3},
	};

	bool CheckMove(int cell, int wall) {
		if (cell == wall)
			return true;
		return false;
	}

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
				if (flag_select) {
					move_queue.push(order_map.at(order));
				}

				if (order == select)
					flag_select = true;
			}
		}
	}

public:
	Enemy(std::pair<int, int> &e_pos) {
		enemy_pos = e_pos;
	}

	std::pair<int, int> GetPos() {
		return enemy_pos;
	}

	void SetMoveQueue(std::string& path, std::string select) {
		LoadMoveQueueCsv(path, select);
	}

	void Move() {
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
};
