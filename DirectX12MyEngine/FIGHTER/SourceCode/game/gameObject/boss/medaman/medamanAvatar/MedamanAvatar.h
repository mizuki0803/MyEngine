#pragma once
#include "ObjObject3d.h"

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// メダマーン(分身)
/// </summary>
class MedamanAvatar : public ObjObject3d
{
protected: //攻撃内容ごとのフェーズ
	enum class AttackTypeAvatarBodyBlowPhase {
		Lockon,
		Shot,
		Back,
		Stay,
	};

	enum class AttackTypeAvatarGatlingPhase {
		MoveCenter,
		RotStart,
		Shot,
		RotEnd,
		Stay,
	};

	enum class AttackTypeAvatarGiantBulletPhase {
		Wait,
		Move,
		ChargeShot,
		Recoil,
		Stay,
	};

public: //静的メンバ関数
	//getter
	static const int GetMaxHP() { return MedamanAvatar::maxHP; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { MedamanAvatar::stageScene = stageScene; }
	static void SetAvatarModel(ObjModel* model) { MedamanAvatar::avatarModel = model; }
	static void SetAvatarDamageModel(ObjModel* model) { MedamanAvatar::avatarDamageModel = model; }
	static void SetAvatarSleepModel(ObjModel* model) { MedamanAvatar::avatarSleepModel = model; }
	static void SetAvatarDeadModel(ObjModel* model) { MedamanAvatar::avatarDeadModel = model; }
	static void SetBulletModel(ObjModel* model) { MedamanAvatar::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Update() override;

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	/// <param name="attackPower">攻撃力</param>
	/// <param name="collisionPos">衝突座標</param>
	/// <param name="subjectVel">衝突した相手とのベクトル</param>
	void Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// HPが少ない状態のモデルに変更する
	/// </summary>
	void DamageModelChange();

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// 攻撃内容:分身体当たり
	/// </summary>
	/// <param name="playerPosition">自機座標</param>
	void AttackTypeAvatarBodyBlow(const Vector3& playerPosition);

	/// <summary>
	/// 攻撃内容:分身ガトリング砲
	/// </summary>
	void AttackTypeAvatarGatling();

	/// <summary>
	/// 攻撃内容:分身巨大弾
	/// </summary>
	void AttackTypeAvatarGiantBullet();

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
	/// 寝ている状態のモデルから起きている状態のモデルに変更
	/// </summary>
	void ChangeModel();

	//getter
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }
	const int GetDamageNum() { return damageNum; }
	const bool GetIsDamageTrigger() { return isDamageTrigger; }

protected: //メンバ関数
	/// <summary>
	/// 通常弾発射
	/// </summary>
	/// <param name="scale">弾の大きさ</param>
	/// <param name="bulletSpeed">弾の速度</param>
	void Fire(const float scale, const float bulletSpeed);

	/// <summary>
	/// チャージ弾発射
	/// </summary>
	/// <param name="scale">弾の大きさ</param>
	/// <param name="bulletSpeed">弾の速度</param>
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
	/// <param name="collisionPos">衝突座標</param>
	void DamageExplosion(const Vector3& collisionPos);

	/// <summary>
	/// ダメージを喰らった状態の色をチカチカさせる処理
	/// </summary>
	void DamageColorChange();

	/// <summary>
	/// 攻撃内容:分身体当たりのロックオン処理
	/// </summary>
	virtual void AttackTypeAvatarBodyBlowLockon() = 0;

	/// <summary>
	/// 攻撃内容:分身体当たりの飛ばす処理
	/// </summary>
	void AttackTypeAvatarBodyBlowShot();

	/// <summary>
	/// 攻撃内容:分身体当たりの元の位置に戻る処理
	/// </summary>
	void AttackTypeAvatarBodyBlowBack();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の中心に移動させる処理
	/// </summary>
	virtual void AttackTypeAvatarGatlingMoveCenter() = 0;

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の回転開始処理
	/// </summary>
	void AttackTypeAvatarGatlingRotStart();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の飛ばす処理
	/// </summary>
	void AttackTypeAvatarGatlingShot();

	/// <summary>
	/// 攻撃内容:分身ガトリング砲の回転終了処理
	/// </summary>
	void AttackTypeAvatarGatlingRotEnd();

	/// <summary>
	/// 攻撃内容:分身巨大弾の待機処理
	/// </summary>
	virtual void AttackTypeAvatarGiantBulletWait() = 0;

	/// <summary>
	/// 攻撃内容:分身巨大弾の移動させる処理
	/// </summary>
	virtual void AttackTypeAvatarGiantBulletMove() = 0;

	/// <summary>
	/// 攻撃内容:分身巨大弾のチャージショット処理
	/// </summary>
	void AttackTypeAvatarGiantBulletChargeShot();

	/// <summary>
	/// 攻撃内容:分身巨大弾の反動で移動する処理
	/// </summary>
	void AttackTypeAvatarGiantBulletRecoil();

	/// <summary>
	/// 待機処理
	/// </summary>
	void Stay();

	/// <summary>
	/// 死亡時の処理
	/// </summary>
	void Dead();

	/// <summary>
	/// 死亡したときの動き
	/// </summary>
	virtual void DeadAction();

	/// <summary>
	/// 死亡後の黒煙エフェクト
	/// </summary>
	void DeadBlackSmoke();

protected: //静的メンバ変数
	//ステージシーン
	static BaseStageScene* stageScene;
	//分身のモデル
	static ObjModel* avatarModel;
	//HPが少ない状態のモデル
	static ObjModel* avatarDamageModel;
	//分身の寝ている状態のモデル
	static ObjModel* avatarSleepModel;
	//死亡状態のモデル
	static ObjModel* avatarDeadModel;
	//敵弾のモデル
	static ObjModel* bulletModel;
	//通常サイズ
	static const Vector3 normalSize;
	//ダメージ状態のサイズ
	static const Vector3 damageSize;
	//体力
	static const int maxHP = 8;
	//攻撃状態のY軸回転
	static const float attackModeRotY;
	//待機状態のY軸回転
	static const float waitModeRotY;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;
	//攻撃内容:分身体当たりの行動遷移
	static void (MedamanAvatar::* attackTypeAvatarBodyBlowPhaseFuncTable[])();
	//攻撃内容:分身ガトリング砲の行動遷移
	static void (MedamanAvatar::* attackTypeAvatarGatlingPhaseFuncTable[])();
	//攻撃内容:分身ガトリング砲で使う円の半径の長さ
	static const float attackAvatarGatlingLength;
	//攻撃内容:分身巨大弾の行動遷移
	static void (MedamanAvatar::* attackTypeAvatarGiantBulletPhaseFuncTable[])();

protected: //メンバ変数
	//体力
	int HP = maxHP;
	//死亡フラグ
	bool isDead = false;
	//死亡してからの時間タイマー
	int32_t deadTimer = 0;
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
	//メダマーン本体との親子関係上の基準座標
	Vector3 basePos;
	//基準位置に戻るときの出発座標
	Vector3 returnStartPos;
	//攻撃で使うタイマー
	int32_t attackTimer = 0;
	//攻撃内容:分身体当たりの行動
	AttackTypeAvatarBodyBlowPhase attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Lockon;
	//攻撃内容:分身体当たりで飛ばす角度
	Vector3 attackAvatarBodyBlowVelocity;
	//攻撃内容:分身体当たりでロックオン対象になる座標
	Vector3 attackAvatarBodyBlowLockonPos;
	//攻撃内容:分身ガトリング砲の行動
	AttackTypeAvatarGatlingPhase attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::MoveCenter;
	//攻撃内容:分身ガトリング砲で使う円運動用角度
	float attackAvatarGatlingAngle;
	//攻撃内容:分身ガトリング砲で使う回転速度
	float attackAvatarGatlingRotSpeed = 0;
	//攻撃内容:分身巨大弾の行動
	AttackTypeAvatarGiantBulletPhase attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Wait;
	//攻撃内容:分身巨大弾で使う反動速度
	Vector3 attackAvatarGiantBulletRecoilVelocity = { 0, 0, 2 };
	//攻撃内容:分身巨大弾で使う反動加速度
	Vector3 attackAvatarGiantBulletRecoilAccel = { 0, 0, -0.05f };
	//死亡時墜落速度
	Vector3 crashVel = { 0, 0, 0 };
};
