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

void Main()
{
	// ウィンドウを 640x480 にリサイズする
	Window::Resize(640, 480);

	GameMaster game_master = GameMaster();

	std::string csv_path = "maps/csv/map1.csv";
	Map map = Map(csv_path);

	std::pair<int, int> player_pos = map.GetPos("start");
	Player player = Player(player_pos);

	std::vector<Enemy> enemies;
	for (int i = 0; i < map.GetNumEnemies(); i++) {
		std::pair<int, int> enemy_pos = map.GetPos("enemy", i);
		Enemy enemy = Enemy(enemy_pos);
		enemies.push_back(enemy);
	}

	map.ShowConsole(player_pos, enemies);

	bool flag_show = false;

	while (System::Update())
	{
		player_pos = player.GetPos();
		if (map.CheckClear(player_pos) || game_master.ChceckCollisionP_E(player_pos, enemies))
			break;

		if (KeyLeft.down()) {
			player.Move(1, "Left", map);
			flag_show = true;
		}

		if (KeyRight.down()) {
			player.Move(1, "Right", map);
			flag_show = true;
		}

		if (KeyUp.down()) {
			player.Move(1, "Up", map);
			flag_show = true;
		}

		if (KeyDown.down()) {
			player.Move(1, "Down", map);
			flag_show = true;
		}

		if (flag_show) {
			std::pair<int, int> player_pos = player.GetPos();
			map.ShowConsole(player_pos, enemies);
			flag_show = false;
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
