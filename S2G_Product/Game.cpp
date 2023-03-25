# include "SceneHeaders/GameScene.h"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// GameMaster
	game_master = GameMaster();

	// Map
	map = Map(csv_path);

	// Player
	player_pos = map.GetPos("start");
	player = Player(player_pos);

	// Enemy
	enemy_pos = map.GetPos("enemy", 0);
	enemy_poses["A_1"] = enemy_pos;
	enemy_1 = Enemy(enemy_pos);
	enemy_1.SetMoveQueue(enemy_move_path, "1");

	enemy_pos = map.GetPos("enemy", 1);
	enemy_poses["A_2"] = enemy_pos;
	enemy_2 = Enemy(enemy_pos);
	enemy_2.SetMoveQueue(enemy_move_path, "2");

	// マップチップ用テクスチャ
	mapchip = MapChip(chip_path);

	chip_path = "example/spritesheet/siv3d-kun-16.png";
	mapchip.SetChip(chip_path, "p");

	chip_path = "pictures/dark_siv3dkun.png";
	mapchip.SetChip(chip_path, "e");

	// ヒートマップ
	heatmap = HeatMap(map);
	heatmap.CoolHeatMap();
	heatmap.CalcHeatMap(player_pos, player.GetTemp(), player.GetTempDistance());
	heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
	heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
}

void Game::update()
{
	audio.play();

	if (map.CheckClear(player_pos)) {
		audio.pause(2s);
		changeScene(State::Clear);
	}

	if (KeyLeft.down()) {
		player.Move(1, "Left", map);
		flag_eMove = true;
	}

	if (KeyRight.down()) {
		player.Move(1, "Right", map);
		flag_eMove = true;
	}

	if (KeyUp.down()) {
		player.Move(1, "Up", map);
		flag_eMove = true;
	}

	if (KeyDown.down()) {
		player.Move(1, "Down", map);
		flag_eMove = true;
	}

	// 蓄積時間が出現間隔を超えたら敵が行動する
	enemy_1.AddAccumulator(Scene::DeltaTime());
	enemy_2.AddAccumulator(Scene::DeltaTime());

	if (enemy_1.CanMove())
	{
		enemy_pos = enemy_1.GetPos();

		if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
			heatmap.RestHeatMap();
			heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
			heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
			player_pos = map.GetPos("start");
			player.SetPos(player_pos);
		}
		enemy_1.MoveQueue();
		enemy_pos = enemy_1.GetPos();
		enemy_poses["A_1"] = enemy_pos;

		if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
			heatmap.RestHeatMap();
			heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
			heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
			player_pos = map.GetPos("start");
			player.SetPos(player_pos);
		}
	}

	if (enemy_2.CanMove())
	{
		enemy_pos = enemy_2.GetPos();

		if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
			heatmap.RestHeatMap();
			heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
			heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
			player_pos = map.GetPos("start");
			player.SetPos(player_pos);
		}

		enemy_2.MoveHeatMap(heatmap, map);
		enemy_pos = enemy_2.GetPos();
		enemy_poses["A_2"] = enemy_pos;

		if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
			heatmap.RestHeatMap();
			heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
			heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
			player_pos = map.GetPos("start");
			player.SetPos(player_pos);
		}
	}

	if (flag_eMove) {
		player_pos = player.GetPos();

		heatmap.CoolHeatMap();
		heatmap.CalcHeatMap(player_pos, player.GetTemp(), player.GetTempDistance());
		heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
		heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);

		flag_eMove = false;
	}
}

void Game::draw() const
{
	audio.setVolume(0.5);

	// レンダーテクスチャを黒でクリア
	renderTexture.clear(ColorF{ 0.0,1.0 });

	{
		// renderTexture を描画先として設定
		const ScopedRenderTarget2D rt{ renderTexture };

		// マップ
		for (int32 y = 0; y < map.GetMapInfo().first; ++y)
		{
			for (int32 x = 0; x < map.GetMapInfo().second; ++x)
			{
				// マップチップの描画座標
				const Point pos{ (x * mapchip.GetMapChipSize()), (y * mapchip.GetMapChipSize()) };
				std::pair<int, int> cell = std::make_pair(y, x);

				if (map.GetMapCell(cell) == map.GetMapVal("."))
					mapchip.GetChip_Map(4, 4).draw(pos);

				if (map.GetMapCell(cell) == map.GetMapVal("#"))
					mapchip.GetChip_Map(0, 169).draw(pos);

				if (map.GetMapCell(cell) == map.GetMapVal("g"))
					mapchip.GetChip_Map(3, 167).draw(pos);

				if (map.GetMapCell(cell) == map.GetMapVal("s"))
					mapchip.GetChip_Map(7, 13).draw(pos);
			}
		}

		const Point pos{ (player_pos.second * mapchip.GetMapChipSize()), (player_pos.first * mapchip.GetMapChipSize()) };
		mapchip.GetChip_Player(1, 0).draw(pos);

		int32 i = 0;
		for (auto enemy_pos : enemy_poses) {
			std::pair<int, int> e_pos = enemy_pos.second;
			const Point pos{ (e_pos.second * mapchip.GetMapChipSize()), (e_pos.first * mapchip.GetMapChipSize()) };
			if (i == 0) {
				if (enemy_1.GetChaseFlag()) {
					mapchip.GetChip_Enemy(0, 0).draw(pos, ColorF{ 1.0, 0.0, 0.0 });
				}
				else
					mapchip.GetChip_Enemy(0, 0).draw(pos);
			}
			else {
				if (enemy_2.GetChaseFlag()) {
					mapchip.GetChip_Enemy(0, 0).draw(pos, ColorF{ 1.0, 0.0, 0.0 });
				}
				else
					mapchip.GetChip_Enemy(0, 0).draw(pos);
			}
			i++;
		}
	}


	{
		// テクスチャ拡大描画時にフィルタリング（なめらかなな拡大処理）をしないサンプラーステートを適用
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		// renderTexture を 2 倍のサイズでシーンに描画
		renderTexture.scaled(2).draw();
	}
}
