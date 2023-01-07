#pragma once
#include "ObjObject3d.h"

//自機クラスを前方宣言
class BasePlayer;

/// <summary>
/// 回復アイテム
/// </summary>
class HealingItem : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>回復アイテム</returns>
	static HealingItem* Create(ObjModel* model, const Vector3& position, const float size);

	//setter
	static void SetPlayer(BasePlayer* player) { HealingItem::player = player; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision();

	//getter
	bool GetIsTouched() const { return isTouched; }
	bool GetIsDelete() const { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// ぐるぐる回転
	/// </summary>
	void Round();

	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

	/// <summary>
	/// キラキラ演出管理
	/// </summary>
	void ShineEffect();

	/// <summary>
	/// キラキラ演出サイズ変更
	/// </summary>
	void ShineEffectSizeChange();

	/// <summary>
	/// キラキラ演出生成間隔変更
	/// </summary>
	void ShineEffectIntervalChange();

	/// <summary>
	/// 接触後の動き
	/// </summary>
	void TouchedAction();

private: //静的メンバ変数
	//プレイヤー自機
	static BasePlayer* player;
	//キラキラ演出の初期サイズ
	static const float shineEffectCreateSize;
	//キラキラ演出の初期生成間隔
	static const int shineEffectCreateInterval = 5;

private: //メンバ変数
	//回転の速さ
	float rotSpeed = 2.0f;
	//キラキラ演出用タイマー
	int32_t shineEffectTimer = 0;
	//キラキラ演出サイズ
	float shineEffectSize = 0;
	//キラキラ演出生成間隔
	int shineEffectInterval = shineEffectCreateInterval;
	//キラキラ演出生成数
	int shineEffectNum = 5;
	//接触フラグ
	bool isTouched = false;
	//接触後タイマー
	int32_t touchedTimer = 0;
	//削除フラグ
	bool isDelete = false;
};
