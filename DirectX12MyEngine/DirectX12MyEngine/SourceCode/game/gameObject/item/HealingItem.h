#pragma once
#include "ObjObject3d.h"

//自機クラスを前方宣言
class Player;

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
	static void SetPlayer(Player* player) { HealingItem::player = player; }

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
	Vector3 GetWorldPos();
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
	/// 接触後の動き
	/// </summary>
	void TouchedAction();

private: //静的メンバ変数
	//プレイヤー自機
	static Player* player;

private: //メンバ変数
	//回転の速さ
	float rotSpeed = 2.0f;
	//接触フラグ
	bool isTouched = false;
	//接触後タイマー
	int32_t touchedTimer = 0;
	//削除フラグ
	bool isDelete = false;
};
