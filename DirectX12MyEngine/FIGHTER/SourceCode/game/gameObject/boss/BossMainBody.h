#pragma once
#include "ObjObject3d.h"

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// ボス(本体)
/// </summary>
class BossMainBody : public ObjObject3d
{
private: //攻撃内容ごとのフェーズ
	enum class AttackTypeTrackingPhase {
		Lockon,
		Shot,
		Stay,
	};

	enum class AttackTypeSuperGiantBulletPhase {
		Move,
		ChargeShot,
		Stay,
	};

	enum class AttackTypeRotatePhase {
		Wait,
		Shot,
		Move,
		Stay,
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ボス(本体)</returns>
	static BossMainBody* Create(const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return BossMainBody::maxHP; }
	static const Vector3& GetNormalSize() { return BossMainBody::normalSize; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { BossMainBody::stageScene = stageScene; }
	static void SetBossMainBodyModel(ObjModel* model) { BossMainBody::mainBodyModel = model; }
	static void SetBossMainBodyDamageModel(ObjModel* model) { BossMainBody::mainBodyDamageModel = model; }
	static void SetBossMainBodySleepModel(ObjModel* model) { BossMainBody::mainBodySleepModel = model; }
	static void SetBossMainBodyDeadModel(ObjModel* model) { BossMainBody::mainBodyDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { BossMainBody::bulletModel = model; }

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
	/// HPが少ない状態のモデルに変更する
	/// </summary>
	void DamageModelChange();

	/// <summary>
	/// 降下
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void Fall(const float time);

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// 攻撃内容:追従
	/// </summary>
	void AttackTypeTracking(const Vector3& playerPosition);

	/// <summary>
	/// 攻撃内容:超巨大弾
	/// </summary>
	void AttackTypeSuperGiantBullet();

	/// <summary>
	/// 攻撃内容:回転
	/// </summary>
	void AttackTypeRotate();

	/// <summary>
	/// 攻撃内容:分身体当たり
	/// </summary>
	void AttackTypeAvatarBodyBlow();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲
	/// </summary>
	void AttackTypeAvatarGatling(const Vector3& playerPosition);

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
	/// 基準位置に戻るときの出発座標を記録する
	/// </summary>
	void SetReturnBasePosition();

	/// <summary>
	/// 基準位置に戻る
	/// </summary>
	/// <param name="time">イージング用(0～1)の数値</param>
	void ReturnBasePosition(const float time);

	/// <summary>
	/// 攻撃状態を終了するので必要な情報をセット
	/// 基準位置に戻るときの出発座標を記録する、弾発射状態解除
	/// </summary>
	void AttackEnd();

	/// <summary>
	/// 死亡後基準位置に戻る
	/// </summary>
	void DeadBasePosBack(const float time);

	/// <summary>
	/// 死亡落下
	/// </summary>
	void DeadFall();

	/// <summary>
	/// 寝ている状態のモデルから起きている状態のモデルに変更
	/// </summary>
	void ChangeModel();

	/// <summary>
	/// 死亡状態のモデルに変更
	/// </summary>
	void ChangeDeadModel();

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }

private: //メンバ関数
	/// <summary>
	/// 通常弾発射
	/// </summary>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// チャージ弾発射
	/// </summary>
	void ChargeBulletFire(const float scale, const float bulletSpeed);

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

	/// <summary>
	/// 攻撃内容:追従のロックオン処理
	/// </summary>
	void AttackTypeTrackingLockon();

	/// <summary>
	/// 攻撃内容:追従の弾発射処理
	/// </summary>
	void AttackTypeTrackingShot();

	/// <summary>
	/// 攻撃内容:超巨大弾の移動処理
	/// </summary>
	void AttackTypeSuperGiantBulletMove();

	/// <summary>
	/// 攻撃内容:超巨大弾のチャージショット処理
	/// </summary>
	void AttackTypeSuperGiantBulletChargeShot();

	/// <summary>
	/// 攻撃内容:回転の待機処理
	/// </summary>
	void AttackTypeRotateWait();

	/// <summary>
	/// 攻撃内容:回転の弾発射処理
	/// </summary>
	void AttackTypeRotateShot();

	/// <summary>
	/// 攻撃内容:回転の移動処理
	/// </summary>
	void AttackTypeRotateMove();

	/// <summary>
	/// 死亡時落下の爆発
	/// </summary>
	void DeadFallExplosion();

	/// <summary>
	/// 待機処理
	/// </summary>
	void Stay();

private: //静的メンバ変数
	//ステージシーン
	static BaseStageScene* stageScene;
	//本体のモデル
	static ObjModel* mainBodyModel;
	//HPが少ない状態のモデル
	static ObjModel* mainBodyDamageModel;
	//本体の寝ている状態のモデル
	static ObjModel* mainBodySleepModel;
	//本体の死亡状態のモデル
	static ObjModel* mainBodyDeadModel;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//通常サイズ
	static const Vector3 normalSize;
	//ダメージ状態のサイズ
	static const Vector3 damageSize;
	//体力
	static const int maxHP = 30;
	//攻撃状態のY軸回転
	static const float attackModeRotY;
	//待機状態のY軸回転
	static const float waitModeRotY;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;
	//攻撃内容:追従の行動遷移
	static void (BossMainBody::* attackTypeTrackingPhaseFuncTable[])();
	//攻撃内容:超巨大弾の行動遷移
	static void (BossMainBody::* attackTypeSuperGiantBulletPhaseFuncTable[])();
	//攻撃内容:回転の行動遷移
	static void (BossMainBody::* attackTypeRotatePhaseFuncTable[])();

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
	//HPが少ない状態のモデルか
	bool isDamageModel = false;
	//ノックバック方向
	Vector3 knockbackVec;
	//ノックバック速度
	Vector3 knockbackVel;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//弾発射座標
	Vector3 bulletShotPos;
	//基準位置に戻るときの出発座標
	Vector3 returnStartPos;
	//攻撃で使うタイマー
	int32_t attackTimer = 0;
	//攻撃内容:追従の行動
	AttackTypeTrackingPhase attackTrackingPhase = AttackTypeTrackingPhase::Lockon;
	//攻撃内容:超巨大弾の行動
	AttackTypeSuperGiantBulletPhase attackSuperGiantBulletPhase = AttackTypeSuperGiantBulletPhase::Move;
	//攻撃内容:回転の行動
	AttackTypeRotatePhase attackRotatePhase = AttackTypeRotatePhase::Wait;
	//攻撃内容:回転で使う攻撃回数
	int attackRotateShotCount = 0;
	//攻撃内容:回転で移動前座標
	Vector3 attackRotateMoveBeforePos;
	//攻撃内容:回転で移動後座標
	Vector3 attackRotateMpveAfterPos;
	//死亡時の座標
	Vector3 deadPos;
	//死亡時の角度
	Vector3 deadRota;
	//死亡時落下速度
	Vector3 deadFallVel;
	//死亡時落下爆発タイマー
	int32_t deadFallExplosionTimer = 0;
};
