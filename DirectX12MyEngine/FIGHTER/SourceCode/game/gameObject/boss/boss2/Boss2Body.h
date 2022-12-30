#pragma once
#include "ObjObject3d.h"

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// ボス2(胴体)
/// </summary>
class Boss2Body : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボス2(胴体)</returns>
	static Boss2Body* Create(const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return Boss2Body::maxHP; }
	static const Vector3& GetNormalSize() { return Boss2Body::normalSize; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { Boss2Body::stageScene = stageScene; }
	static void SetBodyModel(ObjModel* model) { Boss2Body::bodyModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Update() override;

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="attackPower">攻撃力</param>
	void Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// 降下
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void Fall(const float time);

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }
	const int GetDamageNum() { return damageNum; }

private: //メンバ関数
	/// <summary>
	/// 待機処理
	/// </summary>
	void Wait();

	/// <summary>
	/// ダメージ状態の処理
	/// </summary>
	void DamageMode();

	/// <summary>
	/// ダメージ状態のノックバック情報をセット
	/// </summary>
	/// <param name="subjectVel">対象の速度</param>
	void SetDamageKnockback(const Vector3& subjectVel);

	/// <summary>
	/// ダメージ状態のノックバック
	/// </summary>
	void DamageKnockback();

	/// <summary>
	/// ダメージ状態で大きくしたサイズを戻していく処理
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// ダメージ爆発
	/// </summary>
	void DamageExplosion(const Vector3& position);

	/// <summary>
	/// ダメージを喰らった状態の色にする
	/// </summary>
	void DamageColorMode();

private: //静的メンバ変数
	//ステージシーン
	static BaseStageScene* stageScene;
	//胴体のモデル
	static ObjModel* bodyModel;
	//通常サイズ
	static const Vector3 normalSize;
	//ダメージ状態のサイズ
	static const Vector3 damageSize;
	//体力
	static const int maxHP = 30;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;

private: //メンバ変数
	//停止する基準の座標
	Vector3 basePos;
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//削除フラグ
	bool isDelete = false;
	//喰らうダメージ量
	int damageNum;
	//ダメージフラグ
	bool isDamage = false;
	//ダメージを喰らった瞬間か
	bool isDamageTrigger = false;
	//ダメージ状態タイマー
	int32_t damageTimer = 0;
	//ダメージ色か
	bool isDamageColor = false;
	//ノックバック方向
	Vector3 knockbackVec;
	//ノックバック速度
	Vector3 knockbackVel;
};
