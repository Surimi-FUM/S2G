#pragma once
#include "Common.h"

// タイトルシーン
class Clear : public App::Scene
{
	const Texture texture{ U"pictures/clear_image.png" };
	// 基本サイズ 25 のフォントを作成
	Font fontSDF{ FontMethod::SDF, 25, Typeface::Bold };

	const Audio audio{ U"music/MusMus-BGM-104.mp3", Loop::Yes };

public:

	Clear(const InitData& init);

	void update() override;

	void draw() const override;
};
