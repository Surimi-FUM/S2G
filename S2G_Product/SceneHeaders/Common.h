# pragma once
# include <Siv3D.hpp>

// シーンの名前
enum class State
{
	Title,
	Game,
	Clear,
};

using App = SceneManager<State>;
