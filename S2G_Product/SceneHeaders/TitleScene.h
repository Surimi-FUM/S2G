#pragma once
#include "Common.h"

// タイトルシーン
class Title : public App::Scene
{
	Texture player{ U"example/spritesheet/siv3d-kun-16.png" };
	Texture s{ U"maps/mapchip/base.png" };
	Texture g{ U"maps/mapchip/base.png" };
	const Texture image{ U"pictures/pipo-bg005a.jpg" };

	// 基本サイズ 25 のフォントを作成
	const Font font{ 25 };

	const Audio audio{ U"music/MusMus-BGM-157.mp3", Loop::Yes };

public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;
};
