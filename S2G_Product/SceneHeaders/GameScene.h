/*
* プレイ画面のクラス実装
*/
# include "Common.h"
#include "Objects.h"

// ゲームシーン
class Game : public App::Scene
{
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

	// /相互関与用クラス
	GameMaster game_master;

	// マップチップ用テクスチャ
	std::string chip_path = "maps/mapchip/base.png";
	MapChip mapchip;

	// マップ描画用のレンダーテクスチャ
	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	RenderTexture renderTexture{ 320, 240 };

	//　ヒートマップ
	HeatMap heatmap;
	bool flag_Update_heatmap = false;

	// BGM
	const Audio audio{ Audio::Stream, U"music/MusMus-BGM-112.mp3", Loop::Yes };

public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;
};
