/*
* 最初のゲーム開発
* 基本プログラムはC++で記述
* OpenSiv3Dはグラフィック処理のみとする
*/
# include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include "map_class.h"
#include "player_class.h"
#include "enemy_class.h"
#include "GameMaster.h"
#include "mapchip_class.h"

void Main()
{
	// ウィンドウを 640x480 にリサイズする
	Window::Resize(640, 480);

	// GameMaster
	GameMaster game_master = GameMaster();

	// Map
	std::string csv_path = "maps/csv/map1.csv";
	Map map = Map(csv_path);

	// Player
	std::pair<int, int> player_pos = map.GetPos("start");
	Player player = Player(player_pos);

	// Enemy
	std::map<std::string, std::pair<int, int>> enemy_poses;
	std::string enemy_move_path = "enemy_move1.csv";
	std::pair<int, int> enemy_pos = map.GetPos("enemy", 0);
	enemy_poses["A_1"] = enemy_pos;
	Enemy enemy_1 = Enemy(enemy_pos);
	enemy_1.SetMoveQueue(enemy_move_path, "1");

	enemy_pos = map.GetPos("enemy", 1);
	enemy_poses["A_2"] = enemy_pos;
	Enemy enemy_2 = Enemy(enemy_pos);
	enemy_2.SetMoveQueue(enemy_move_path, "2");

	// マップチップ用テクスチャ
	std::string chip_path = "maps/mapchip/base.png";
	MapChip mapchip = MapChip(chip_path);

	chip_path = "example/spritesheet/siv3d-kun-16.png";
	mapchip.SetChip(chip_path, "p");

	chip_path = "pictures/dark_siv3dkun.png";
	mapchip.SetChip(chip_path, "e");

	// マップ描画用のレンダーテクスチャ
	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	RenderTexture renderTexture{ 320, 240 };

	bool flag_eMove = false;

	while (System::Update())
	{
		if (map.CheckClear(player_pos))
			break;

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

		if (flag_eMove) {
			player_pos = player.GetPos();
			enemy_pos = enemy_1.GetPos();

			if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
				player_pos = map.GetPos("start");
				player.SetPos(player_pos);
			}
			enemy_1.Move();
			enemy_2.Move();
			enemy_pos = enemy_1.GetPos();
			enemy_poses["A_1"] = enemy_pos;
			enemy_pos = enemy_2.GetPos();
			enemy_poses["A_2"] = enemy_pos;

			if (game_master.ChceckCollisionP_E(player_pos, enemy_poses)) {
				player_pos = map.GetPos("start");
				player.SetPos(player_pos);
			}

			flag_eMove = false;
		}

		/*
		描画処理
		*/
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

			for (auto enemy_pos : enemy_poses) {
				std::pair<int, int> e_pos = enemy_pos.second;
				const Point pos{ (e_pos.second * mapchip.GetMapChipSize()), (e_pos.first * mapchip.GetMapChipSize()) };
				mapchip.GetChip_Enemy(0, 0).draw(pos);
			}
		}


		{
			// テクスチャ拡大描画時にフィルタリング（なめらかなな拡大処理）をしないサンプラーステートを適用
			const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

			// renderTexture を 2 倍のサイズでシーンに描画
			renderTexture.scaled(2).draw();
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要です。
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/tutorial/
//
// Tutorial
// https://siv3d.github.io/tutorial/tutorial/
//
// Siv3D コミュニティへの参加（Discord などで気軽に質問や交流, 最新情報の入手ができます）
// https://siv3d.github.io/ja-jp/community/community/
//
// Siv3D User Community
// https://siv3d.github.io/community/community/
//
// 新機能の提案やバグの報告 | Feedback
// https://siv3d.github.io/ja-jp/develop/report/
//
// Sponsoring Siv3D
// https://github.com/sponsors/Reputeless
//
