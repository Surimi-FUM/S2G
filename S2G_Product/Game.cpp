/*
* プレイ画面クラスのメンバ関数定義
* プレイヤ、敵、マップなど、ゲーム処理はここに記述する
*/
#pragma once
# include "SceneHeaders/GameScene.h"

// コンストラクタ　※必ず実装
Game::Game(const InitData& init)
	: IScene{ init }
{
	// インスタンス生成
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
	// 相互関与用インスタンス
	game_master = GameMaster();
	// マップチップ用テクスチャ
	mapchip = MapChip(chip_path);

	chip_path = "resource/pictures/siv3d-kun-16.png";
	mapchip.SetChip(chip_path, "p");
	chip_path = "resource/pictures/dark_siv3dkun.png";
	mapchip.SetChip(chip_path, "e");
	chip_path = "resource/pictures/b_enemy.png";
	mapchip.SetChip(chip_path, "e_q");

	// ヒートマップ
	heatmap = HeatMap(map);
	heatmap.CoolHeatMap();
	heatmap.CalcHeatMap(player_pos, player.GetTemp(), player.GetTempDistance());
	heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
	heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);

	//　音楽
	audio.play();
	audio.setVolume(0.5);
}

//　更新関数
void Game::update()
{
	//　クリア判定
	if (map.CheckClear(player_pos)) {
		audio.pause(2s);
		changeScene(State::Clear);
	}

	//　プレイヤ移動処理
	if (KeyLeft.down()) {
		player.Move(1, "Left", map);
		flag_Update_heatmap = true;
	}

	if (KeyRight.down()) {
		player.Move(1, "Right", map);
		flag_Update_heatmap = true;
	}

	if (KeyUp.down()) {
		player.Move(1, "Up", map);
		flag_Update_heatmap = true;
	}

	if (KeyDown.down()) {
		player.Move(1, "Down", map);
		flag_Update_heatmap = true;
	}

	// 敵移動処理
	// 蓄積時間が出現間隔を超えたら敵が行動する
	enemy_1.AddAccumulator(Scene::DeltaTime());
	enemy_2.AddAccumulator(Scene::DeltaTime());

	//　敵1　処理
	if (enemy_1.CanMove())
	{
		enemy_pos = enemy_1.GetPos();

		// プレイヤとの衝突判定
		if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
			heatmap.RestHeatMap();
			heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
			heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
			player_pos = map.GetPos("start");
			player.SetPos(player_pos);
		}

		//　移動処理
		enemy_1.MoveQueue();
		enemy_pos = enemy_1.GetPos();
		enemy_poses["A_1"] = enemy_pos;

		// プレイヤとの衝突判定
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

		//　プレイヤとの衝突判定
		if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
			heatmap.RestHeatMap();
			heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
			heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
			player_pos = map.GetPos("start");
			player.SetPos(player_pos);
		}

		// 移動処理
		enemy_2.MoveHeatMap(heatmap, map);
		enemy_pos = enemy_2.GetPos();
		enemy_poses["A_2"] = enemy_pos;

		//　プレイヤとの衝突判定
		if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
			heatmap.RestHeatMap();
			heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
			heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);
			player_pos = map.GetPos("start");
			player.SetPos(player_pos);
		}
	}

	// ヒートマップ更新
	if (flag_Update_heatmap) {
		player_pos = player.GetPos();

		heatmap.CoolHeatMap();
		heatmap.CalcHeatMap(player_pos, player.GetTemp(), player.GetTempDistance());
		heatmap.CalcHeatMap(map.GetPos("start"), heatmap.GetTempVal("s"), 3);
		heatmap.CalcHeatMap(map.GetPos("goal"), heatmap.GetTempVal("g"), 3);

		flag_Update_heatmap = false;
	}
}

//　描画関数
void Game::draw() const
{
	// レンダーテクスチャを黒でクリア
	renderTexture.clear(ColorF{ 0.0,1.0 });

	// スペースキーを押すとヒートマップを表示する
	const bool showHeatmap = KeySpace.pressed();

	//　レンダーテクスチャで描画する場合は、{}で描画先テクスチャを指定する
	{
		// renderTexture を描画先として設定
		const ScopedRenderTarget2D rt{ renderTexture };

		// マップ（ステージ）描画
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

				// ヒートマップ描画処理
				if (showHeatmap)
				{
					// 温度に合わせた色を透過させて上に描画する
					double temp = heatmap.GetCellTemp(cell);
					if (temp >= 0)
						//　高温は赤
						Rect{ pos, mapchip.GetMapChipSize() }.draw(ColorF{ 1 - (1 / temp), 0, 0, 0.4 });
					else
						//　低温は青
						Rect{ pos, mapchip.GetMapChipSize() }.draw(ColorF{ 0, 0, 1 - (1 / temp), 0.4 });
				}
			}
		}

		//　プレイヤ描画
		const Point pos{ (player_pos.second * mapchip.GetMapChipSize()), (player_pos.first * mapchip.GetMapChipSize()) };
		mapchip.GetChip_Player(1, 0).draw(pos);

		//　敵描画
		int32 i = 0;
		for (auto enemy_pos : enemy_poses) {
			std::pair<int, int> e_pos = enemy_pos.second;
			const Point pos{ (e_pos.second * mapchip.GetMapChipSize()), (e_pos.first * mapchip.GetMapChipSize()) };
			if (i == 0) {
				mapchip.GetChip_Enemy_Q(0, 0).scaled(0.6).draw(pos);
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

	// テクスチャの設定
	{
		// テクスチャ拡大描画時にフィルタリング（なめらかなな拡大処理）をしないサンプラーステートを適用
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		// renderTexture を 2 倍のサイズでシーンに描画
		renderTexture.scaled(2).draw();
	}
}
