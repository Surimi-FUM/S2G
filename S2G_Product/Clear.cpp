# include "SceneHeaders/ClearScene.h"

Clear::Clear(const InitData& init)
	: IScene{ init } {}

void Clear::update()
{
	audio.play();

	fontSDF.setBufferThickness(3);

	if (KeyEnter.pressed())
		changeScene(State::Title);
}

void Clear::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	texture.scaled(1.25).draw(-50, 0);

	FontAsset(U"TitleFont")(U"Game Clear!").drawAt(200, 250);

	const double outlineScale = 0.2;
	const ColorF outlineColor{ 0.0, 0.0, 0.0 };
	fontSDF(U"タイトル画面へ：[Enter] \n 終了：[ESC]").drawAt(TextStyle::Outline(outlineScale, outlineColor), 200, 450);

	audio.setVolume(0.7);
}
