#pragma once

/// <summary>
/// スプライトテクスチャ読み込みクラス
/// </summary>
class SpriteTextureLoader
{
public: //テクスチャ名
	enum TextureName {
		DebugFont,			//デバッグ用フォント
		White,				//真っ白
		Number,				//数字
		TitleLogo,			//タイトルロゴ
		BButton,			//Bボタン
		HowToPlayCharge,	//遊び方「チャージ」
		Reticle,			//レティクル
		PlayerHPGaugeIn,	//自機HPバー
		PlayerHPGaugeOut,	//自機HPバーフレーム
		BossHPGaugeIn,		//ボスHPバー
		BossHPGaugeOut,		//ボスHPバーフレーム
		HitPlusText,		//HIT+テキスト
		HitPlusNumber,		//HIT+数字
		StageClearText,		//ステージクリアテキスト(攻略完了)
		DefeatNumText,		//撃破数のテキストと枠
	};


public: //静的メンバ関数
	/// <summary>
	/// テクスチャ全読み込み
	/// </summary>
	static void TextureLoad();
};