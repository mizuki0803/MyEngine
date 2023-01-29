#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "PlayerReticles.h"
#include "PlayerHPUI.h"
#include "PlayerSpeedChangeUI.h"
#include "PlayerDamageEffect.h"
#include "PlayerVaporEffect.h"
#include "PlayerRollingEffect.h"
#include <memory>
#include <list>

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// 自機基盤
/// </summary>
class BasePlayer : public ObjObject3d
{
public:
	//移動速度状態
	enum class MoveSpeedPhase {
		NormalSpeed,	//通常移動
		HighSpeed,		//速い移動
		SlowSpeed,		//遅い移動
		ReturnNormalSpeed, //通常移動に戻す状態
	};

public: //静的メンバ関数

	//getter
	static const Vector2& GetRotLimit() { return rotLimit; }
	static const float GetMoveBaseSpeed() { return moveBaseSpeed; }
	static const float GetKnockbackBaseSpeed() { return knockbackBaseSpeed; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { BasePlayer::stageScene = stageScene; }
	static void SetBulletModel(ObjModel* model) { BasePlayer::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="startHP">開始時HP</param>
	/// <param name="maxHP">最大HP</param>
	/// <param name="isVaporCreate">飛行機雲演出を生成するか</param>
	/// <returns>成否</returns>
	virtual bool Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	/// UI更新
	/// </summary>
	virtual void UpdateUI();

	/// <summary>
	/// UI描画
	/// </summary>
	virtual void DrawUI();

	/// <summary>
	/// 衝突時コールバック関数(敵などダメージを喰らう)
	/// </summary>
	/// <param name="subjectPos">衝突した相手の座標</param>
	virtual void OnCollisionDamage(const Vector3& subjectPos);

	/// <summary>
	/// 衝突時コールバック関数(アイテムなど回復する)
	/// </summary>
	virtual void OnCollisionHeal();

	//getter
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const int GetHP() { return HP; }
	const Vector3& GetBasePos() { return basePos; }
	const Vector3& GetJetPos() { return jetPos; }
	const bool GetIsDamageKnockback() { return isDamageKnockBack; }
	const bool GetIsCrash() { return isCrash; }
	const bool GetIsDead() { return isDead; }
	const bool GetIsRoll() { return isRoll; }
	PlayerVaporEffect* GetVaporEffect() { return vaporEffect.get(); }
	MoveSpeedPhase GetMoveSpeedPhase() { return moveSpeedPhase; }
	const Vector2& GetMoveLimitMax() { return moveLimitMax; }
	const Vector2& GetMoveLimitMin() { return moveLimitMin; }
	const Vector3& GetKnockbackVel() { return knockbackVel; }
	PlayerReticles* GetReticles() { return reticles.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }
	const bool GetIsStageClearModeCompletion() { return isStageClearModeCompletion; }

protected: //メンバ関数
	/// <summary>
	/// ジェット発射座標を更新
	/// </summary>
	void UpdateJetPos();

	/// <summary>
	/// 両翼の座標を更新
	/// </summary>
	void UpdateWingPos();

	/// <summary>
	/// 行動
	/// </summary>
	void Action();

	/// <summary>
	/// 自機のジェットエフェクト管理
	/// </summary>
	void JetEffectManager();

	/// <summary>
	/// 墜落時の黒煙エフェクト
	/// </summary>
	void CrashBlackSmoke();

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	void Damage();

	/// <summary>
	/// ダメージ状態の処理
	/// </summary>
	void DamageMode();

	/// <summary>
	/// ダメージ状態のノックバック情報をセット
	/// </summary>
	/// <param name="subjectPos">対象の座標</param>
	void SetDamageKnockback(const Vector3& subjectPos);

	/// <summary>
	/// ダメージ状態のノックバック
	/// </summary>
	void DamageKnockback();

	/// <summary>
	/// ダメージを喰らった状態の色をチカチカさせる処理
	/// </summary>
	void DamageColorChange();

	/// <summary>
	/// 墜落開始
	/// </summary>
	virtual void CrashStart();

	/// <summary>
	/// 墜落
	/// </summary>
	virtual void Crash() = 0;

	/// <summary>
	/// 回復
	/// </summary>
	void Heal();

	/// <summary>
	/// 回転
	/// </summary>
	virtual void Rotate();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 旋回
	/// </summary>
	void Roll();

	/// <summary>
	/// 旋回開始
	/// </summary>
	void RollStart();

	/// <summary>
	/// 旋回状態
	/// </summary>
	void RollMode();

	/// <summary>
	/// 速度変更
	/// </summary>
	void SpeedChange();

	/// <summary>
	/// 速度変更開始
	/// </summary>
	/// <param name="isPushHighSpeedInput">加速入力をしたか</param>
	/// <param name="isPushSlowSpeedInput">減速入力をしたか</param>
	void SpeedChangeStart(bool isPushHighSpeedInput, bool isPushSlowSpeedInput);

	/// <summary>
	/// 速度変更中(加速or減速)の処理
	/// </summary>
	/// <param name="isPushHighSpeedInput">加速入力をしたか</param>
	/// <param name="isPushSlowSpeedInput">減速入力をしたか</param>
	void SpeedChangeMode(bool isPushHighSpeedInput, bool isPushSlowSpeedInput);

	/// <summary>
	/// 速度変更終了をセット
	/// </summary>
	void SetSpeedChangeModeEnd();

	/// <summary>
	/// 速度変更終了(元の速度に戻していく)の処理
	/// </summary>
	void SpeedChangeModeEnd();

	/// <summary>
	/// 移動速度を速くするときの処理
	/// </summary>
	void SpeedChangeHighSpeed();

	/// <summary>
	/// 移動速度を遅くするときの処理
	/// </summary>
	void SpeedChangeSlowSpeed();

	/// <summary>
	/// 移動速度を通常に戻すときの処理
	/// </summary>
	void SpeedChangeNormalSpeed();

	/// <summary>
	/// 通常移動時のブラーの強さに戻すときの処理
	/// </summary>
	void SpeedChangeNormalBlur();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻撃ボタンを押しているときの処理
	/// </summary>
	void PushAttackButton();

	/// <summary>
	/// 攻撃ボタンを離したときの処理
	/// </summary>
	void ReleaseAttackButton();

	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// 直進弾発射
	/// </summary>
	void ShotStraightBullet();

	/// <summary>
	/// ホーミング弾発射
	/// </summary>
	void ShotHomingBullet();

	/// <summary>
	/// ステージクリア行動
	/// </summary>
	virtual void StageClear() = 0;

protected: //静的メンバ変数
	//ステージシーン
	static BaseStageScene* stageScene;
	//自機弾のモデル
	static ObjModel* bulletModel;
	//ホーミング弾の大きさ
	static const float homingBulletSize;
	//自機の回転限界
	static const Vector2 rotLimit;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;
	//自機の移動の基準の速さ
	static const float moveBaseSpeed;
	//自機のノックバックの基準の速さ
	static const float knockbackBaseSpeed;
	//速度変更最大ゲージ数
	static const float maxSpeedChangeGauge;
	//通常移動時のブラーの強さ
	static const float normalSpeedBlurStrength;

protected: //メンバ変数
	//最大体力
	int maxHP = 0;
	//体力
	int HP = 0;
	//HPUI
	std::unique_ptr<PlayerHPUI> hpUI;
	//基準の座標
	Vector3 basePos = {};
	//ジェット発射座標
	Vector3 jetPos = {};
	//ダメージ演出
	std::unique_ptr<PlayerDamageEffect> damageEffect;
	//ダメージフラグ
	bool isDamage = false;
	//ダメージ状態タイマー
	int32_t damageTimer = 0;
	//ダメージノックバック状態か
	bool isDamageKnockBack = false;
	//ノックバック方向
	Vector3 knockbackVec;
	//ノックバック速度
	Vector3 knockbackVel;
	//ダメージ色か
	bool isDamageColor = false;
	//墜落中か
	bool isCrash = false;
	//黒煙用タイマー
	int32_t blackSmokeTimer = 0;
	//死亡フラグ
	bool isDead = false;
	//緊急回避中か
	bool isRoll = false;
	//緊急回避用演出
	std::list<std::unique_ptr<PlayerRollingEffect>> rollingEffects;
	//緊急回避用タイマー
	int32_t rollTimer = 0;
	//緊急回避開始時のZ軸角度
	float rollStartRot = 0;
	//緊急回避終了時のZ軸角度
	float rollEndRot = 0;
	//飛行機雲演出を出すか
	bool isVaporCreate = false;
	//飛行機雲演出
	std::unique_ptr<PlayerVaporEffect> vaporEffect;
	//速度変更ゲージ
	float speedChangeGauge = maxSpeedChangeGauge;
	//速度変更ゲージUI
	std::unique_ptr<PlayerSpeedChangeUI> speedChangeUI;
	//速度変更開始可能か
	bool isSpeedChangeStartPossible = true;
	//速度変更中か
	bool isSpeedChange = false;
	//移動速度
	MoveSpeedPhase moveSpeedPhase = MoveSpeedPhase::NormalSpeed;
	//速度変更用タイマー
	int32_t speedChangeTimer = 0;
	//自機の移動限界
	Vector2 moveLimitMax = {};
	Vector2 moveLimitMin = {};
	//レティクル
	std::unique_ptr<PlayerReticles> reticles;
	//弾発射座標
	Vector3 bulletShotPos;
	//直進弾の発射待機中か
	bool isStraightShotWait = false;
	//直進弾の発射待機タイマー
	int32_t straightShotWaitTimer = 0;
	//チャージショット中か
	bool isChargeShotMode = false;
	//チャージした時間
	int32_t chargeTimer = 0;
	//z軸ゆらゆら回転が右回転か
	bool isRotZRight = true;
	//z軸ゆらゆら回転用
	float swayZ = 0.0f;
	//ステージクリア後の動きをするか
	bool isStageClearMode = false;
	//ステージクリア後の行動が完了したか
	bool isStageClearModeCompletion = false;
};
