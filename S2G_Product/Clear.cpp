/*
* クリア画面クラスのメンバ関数定義
*/
# include "SceneHeaders/ClearScene.h"

// コンストラクタ ※必ず実装しないと動かない
Clear::Clear(const InitData& init)
	: IScene{ init } {
	// 音楽再生
	audio.play();
}

//　更新関数
void Clear::update()
{
	fontSDF.setBufferThickness(3);

	// Enterキーでタイトル画面へ戻る
	if (KeyEnter.pressed())
		changeScene(State::Title);
}

// 描画関数
void Clear::draw() const
{
	// 毎回背景を指定した色でリセットする
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	texture.scaled(1.0).draw(0, 0);

	FontAsset(U"TitleFont")(U"Game Clear!").drawAt(200, 180);

	const double outlineScale = 0.2;
	const ColorF outlineColor{ 0.0, 0.0, 0.0 };
	fontSDF(U"タイトル画面へ：[Enter] \n 終了：[ESC]").drawAt(TextStyle::Outline(outlineScale, outlineColor), 200, 350);

	// 音量の調整は倍率を指定する：2倍 = 2.0, 半減＝0.5
	audio.setVolume(0.7);
}
