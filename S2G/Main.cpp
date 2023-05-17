/*
* S2G_Productのメイン関数
*/
#pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include "Scenes.h"

void Main()
{
	// ウィンドウを 640x480 にリサイズする
	Window::Resize(640, 480);

	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 50, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<Clear>(State::Clear);

	// デバック用　開始したいシーンを指定できる
	// manager.init(State::Clear);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要です。
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/tutorial/
//
// Tutorial
// https://siv3d.github.io/tutorial/tutorial/
//
// Siv3D コミュニティへの参加（Discord などで気軽に質問や交流, 最新情報の入手ができます）
// https://siv3d.github.io/ja-jp/community/community/
//
// Siv3D User Community
// https://siv3d.github.io/community/community/
//
// 新機能の提案やバグの報告 | Feedback
// https://siv3d.github.io/ja-jp/develop/report/
//
// Sponsoring Siv3D
// https://github.com/sponsors/Reputeless
//
