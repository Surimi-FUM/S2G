# include "SceneHeaders/ClearScene.h"

Clear::Clear(const InitData& init)
	: IScene{ init } {}

void Clear::update()
{
	if (KeyEnter.pressed())
		changeScene(State::Title);
}

void Clear::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	FontAsset(U"TitleFont")(U"Game Clear!").drawAt(320, 100);
}
