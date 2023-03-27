/*
* 場面クラス共有ヘッダー
*/
#pragma once  // ※必要。書かないと動かない
# include <Siv3D.hpp>

// シーンの名前
enum class State
{
	Title,
	Game,
	Clear,
};

// 各場面で使用したいデータを指定する
using App = SceneManager<State>;
