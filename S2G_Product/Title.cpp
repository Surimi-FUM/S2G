# include "SceneHeaders/TitleScene.h"

Title::Title(const InitData& init)
	: IScene{ init } {}

void Title::update()
{
	audio.play();

	if (KeyS.pressed()) {
		audio.pause(2s);
		changeScene(State::Game);
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	image.scaled(1.5).draw();

	FontAsset(U"TitleFont")(U"S  2  G").drawAt(320, 100);

	s(7 * 16, 13 * 15.95, 16).scaled(3.0).drawAt(200, 250);
	g(3 * 16, 167 * 16, 16).scaled(3.0).drawAt(440, 250);

	player(1 * 22, 0 * 28 + 7, 20, 28).scaled(3.0).drawAt(320, 190);

	Line{ 250, 250, 390, 250 }.drawArrow(10, Vec2{ 20, 20 }, Palette::White);

	font(U"ゲームを始める：[S] \n 終了：[ESC]").drawAt(320, 370);

	//audio.setVolume(0.5);
}
