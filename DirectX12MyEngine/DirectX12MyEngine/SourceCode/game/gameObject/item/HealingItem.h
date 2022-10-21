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
	static HealingItem* Create(ObjModel* model, const Vector3& position);

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
	bool GetIsDead() const { return isDead; }

private: //メンバ関数
	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

private: //静的メンバ変数
	//プレイヤー自機
	static Player* player;

private: //メンバ変数
	//死亡フラグ
	bool isDead = false;
};
