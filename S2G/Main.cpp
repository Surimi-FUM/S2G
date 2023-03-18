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

	map.ShowConsole(player_pos, enemy_poses);

	bool flag_show = false;

	while (System::Update())
	{
		if (map.CheckClear(player_pos))
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
			player_pos = player.GetPos();
			enemy_pos = enemy_1.GetPos();

			if (game_master.ChceckCollisionP_E(player_pos, enemy_poses))
				break;
			enemy_1.Move();
			enemy_2.Move();
			enemy_pos = enemy_1.GetPos();
			enemy_poses["A_1"] = enemy_pos;
			enemy_pos = enemy_2.GetPos();
			enemy_poses["A_2"] = enemy_pos;
			map.ShowConsole(player_pos, enemy_poses);
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
