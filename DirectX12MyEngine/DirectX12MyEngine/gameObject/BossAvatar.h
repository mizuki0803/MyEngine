#pragma once
#include "ObjObject3d.h"

//GameSceneの前方宣言
class GameScene;

/// <summary>
/// ボス(分身)
/// </summary>
class BossAvatar : public ObjObject3d
{
public: //静的メンバ関数
	//getter
	static const int GetMaxHP() { return BossAvatar::maxHP; }

	//setter
	static void SetGameScene(GameScene* gameScene) { BossAvatar::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { BossAvatar::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="damageNum">ダメージ量</param>
	void Damage(int damageNum);

	/// <summary>
	/// 攻撃内容B
	/// </summary>
	void AttackTypeB();

	/// <summary>
	/// 攻撃状態に変更
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ChangeAttackMode(const float time);

	/// <summary>
	/// 待機状態に変更
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ChangeWaitMode(const float time);

	/// <summary>
	/// 固定位置に戻る
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ReturnFixedPosition(const float time);

	/// <summary>
	/// 固定位置に戻るときの出発座標を記録
	/// </summary>
	void SetReturnStartPos();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }

protected:
	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

protected: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//体力
	static const int maxHP = 20;
	//攻撃状態のY軸回転
	static const float attackModeRotY;
	//待機状態のY軸回転
	static const float waitModeRotY;

protected: //メンバ変数
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//ボス本体との親子関係上の定位置座標
	Vector3 fixedPos;
	//固定位置に戻るときの出発座標
	Vector3 returnStartPos;
};
