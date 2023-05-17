/*
* タイトル画面クラスのメンバ関数定義
*/
#pragma once
# include "SceneHeaders/TitleScene.h"

// コンストラクタ　※必ず実装しないと動かない
Title::Title(const InitData& init)
	: IScene{ init } {
	// 音楽再生
	audio.play();
}

//更新関数　メイン関数のWhile文内で周期的に呼ばれる
void Title::update()
{
	// Sキーが押されたらゲーム開始
	if (KeyS.pressed()) {
		audio.pause(2s);
		changeScene(State::Game);
	}

	// "Licenses" ボタンが押されたら
	if (KeyL.pressed()) {
		// ライセンス情報を表示
		LicenseManager::ShowInBrowser();
	}
}

//　描画関数　メイン関数のWhile文内で周期的に呼ばれる
//　描画サイズと位置はウィンドウサイズ(640 * 480)に合わせている　(次の改良では、位置を比率で求めるようにする)
void Title::draw() const
{
	// 毎回背景を指定した色でリセットする
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	image.scaled(1.5).draw();

	FontAsset(U"TitleFont")(U"S  2  G").drawAt(320, 100);

	s(7 * 16, 13 * 15.95, 16).scaled(3.0).drawAt(200, 250);
	g(3 * 16, 167 * 16, 16).scaled(3.0).drawAt(440, 250);

	player(1 * 22, 0 * 28 + 7, 20, 28).scaled(3.0).drawAt(320, 190);

	Line{ 250, 250, 390, 250 }.drawArrow(10, Vec2{ 20, 20 }, Palette::White);

	font(U"ゲームを始める：[S] \n 終了：[ESC] \n ライセンス：[L]").drawAt(320, 370);

	// 音量の調整は倍率を指定する：2倍=2.0, 半減＝0.5　
	//　audio.setVolume(0.5);
}
