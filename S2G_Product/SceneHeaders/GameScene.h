﻿# pragma once
# include "Common.h"
#include "Objects.h"

// ゲームシーン
class Game : public App::Scene{
	// GameMaster
	GameMaster game_master;

	// Map
	std::string csv_path = "maps/csv/map1.csv";
	Map map;

	// Player
	std::pair<int, int> player_pos;
	Player player;

	// Enemy
	std::map<std::string, std::pair<int, int>> enemy_poses;
	std::string enemy_move_path = "enemy_move1.csv";
	std::pair<int, int> enemy_pos;
	Enemy enemy_1;
	Enemy enemy_2;

	// マップチップ用テクスチャ
	std::string chip_path = "maps/mapchip/base.png";
	MapChip mapchip;

	// マップ描画用のレンダーテクスチャ
	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	RenderTexture renderTexture{ 320, 240 };

	bool flag_eMove = false;


public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;
};