/*
* マップチップクラス
*/
#pragma once
# include <Siv3D.hpp>

class MapChip {
	Texture m_base;
	Texture player;
	Texture enemy;
	Texture enemy_q;

	// マップチップのサイズ（ピクセル）
	static constexpr int32 MapChipSize = 16;

public:
	// インスタンス生成用コンストラクタ
	MapChip(std::string &map_image_path) {
		const String image_path = Unicode::Widen(map_image_path);
		m_base = Texture {image_path};
	}

	// 他クラスで変数宣言する用のコンストラクタ
	MapChip() {
	}

	// #----- パラメータ(メンバ変数)取得 -----#
	int32 GetMapChipSize() const {
		return MapChipSize;
	}

	// 指定したインデックスのマップチップを返す
	TextureRegion GetChip_Map(int32 x, int32 y) const
	{
		return m_base(x * MapChipSize, y * MapChipSize, MapChipSize);
	}

	TextureRegion GetChip_Player(int32 x, int32 y) const
	{
		return player(22 * x, 28 * y + 8, 20, 28);
	}

	TextureRegion GetChip_Enemy(int32 x, int32 y) const
	{
		return enemy(22 * x, 28 * y + 8, 20, 28);
	}

	TextureRegion GetChip_Enemy_Q(int32 x, int32 y) const
	{
		return enemy_q(22 * x, 28 * y, 34, 34);
	}

	// #----- パラメータ変更 -----#
	void SetChip(std::string& map_image_path, std::string str) {
		const String image_path = Unicode::Widen(map_image_path);
		if(str == "p")
			player = Texture{ image_path };

		if (str == "e") {
			enemy = Texture{ image_path };
		}

		if (str == "e_q") {
			enemy_q = Texture{ image_path };
		}
	}
};
