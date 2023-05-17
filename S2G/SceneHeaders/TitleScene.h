/*
* タイトル画面のクラス実装
*/
#pragma once
#include "Common.h"

// タイトルシーン
class Title : public App::Scene
{
	Texture player{ U"resource/pictures/siv3d-kun-16.png" };
	Texture s{ U"resource/maps/mapchip/base.png" };
	Texture g{ U"resource/maps/mapchip/base.png" };
	const Texture image{ U"resource/pictures/pipo-bg005a.jpg" };

	// タイトルフォントの作成
	const Font tittle_font{ 60, Typeface::Heavy };
	// 基本サイズ 25 のフォントを作成
	const Font font{ 25 };

	const Audio audio{ U"resource/music/MusMus-BGM-157.mp3", Loop::Yes };

	// タイトル画面でプレイヤ操作
	int p_pos_x = 320;
	int p_pos_y = 190;

public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;
};
