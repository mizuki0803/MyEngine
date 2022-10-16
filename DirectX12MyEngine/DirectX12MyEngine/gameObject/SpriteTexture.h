#pragma once

class SpriteTexture
{
public: //テクスチャ名
	enum TextureName {
		DebugFont,			//デバッグ用フォント
		Number,				//数字
		Reticle,			//レティクル
		PlayerHPGaugeIn,	//自機HPバー
		PlayerHPGaugeOut,	//自機HPバーフレーム
		BossHPGaugeIn,		//ボスHPバー
		BossHPGaugeOut,		//ボスHPバーフレーム
	};


public: //静的メンバ関数
	/// <summary>
	/// テクスチャ全読み込み
	/// </summary>
	static void TextureLoad();
};