# include "SceneHeaders/TitleScene.h"

Title::Title(const InitData& init)
	: IScene{ init } {}

void Title::update()
{
	if (KeyS.pressed())
		changeScene(State::Game);
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	FontAsset(U"TitleFont")(U"S2G").drawAt(320, 100);
}
