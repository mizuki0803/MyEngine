#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "PlayerReticles.h"
#include "PlayerHPUI.h"
#include "PlayerSpeedChangeUI.h"
#include "PlayerDamageEffect.h"
#include <memory>
#include <list>

//ゲームシーンの前方宣言
class GameScene;

/// <summary>
/// 自機
/// </summary>
class Player : public ObjObject3d
{
public:
	//移動速度状態
	enum class MoveSpeedPhase {
		NormalSpeed,	//通常移動
		HighSpeed,		//速い移動
		SlowSpeed,		//遅い移動
		ReturnNormalSpeed, //通常移動に戻す状態
	};

	//ステージクリア後行動フェーズ
	enum class StageClearModePhase {
		SideMove,	//横旋回移動
		Return,		//旋回帰還
		Up,			//上昇
		Stay,		//停止
		Boost,		//ブースト
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="startHP">開始時HP</param>
	/// <param name="maxHP">最大HP</param>
	/// <returns>自機</returns>
	static Player* Create(ObjModel* model, const int startHP, const int maxHP);

	//getter
	static const Vector2& GetRotLimit() { return rotLimit; }
	static const Vector2& GetMoveLimitMax() { return moveLimitMax; }
	static const Vector2& GetMoveLimitMin() { return moveLimitMin; }
	static const float GetMoveBaseSpeed() { return moveBaseSpeed; }
	static const float GetKnockbackBaseSpeed() { return knockbackBaseSpeed; }

	//setter
	static void SetGameScene(GameScene* gameScene) { Player::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Player::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="startHP">開始時HP</param>
	/// <param name="maxHP">最大HP</param>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* model, const int startHP, const int maxHP);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// UI更新
	/// </summary>
	void UpdateUI();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 衝突時コールバック関数(敵などダメージを喰らう)
	/// </summary>
	void OnCollisionDamage(const Vector3& subjectPos);

	/// <summary>
	/// 衝突時コールバック関数(アイテムなど回復する)
	/// </summary>
	void OnCollisionHeal();

	/// <summary>
	/// ステージクリア後の動きを開始する
	/// </summary>
	void StageClearModeStart();

	/// <summary>
	/// ステージクリア後の帰還を開始する
	/// </summary>
	/// <param name="cameraPos">カメラ座標</param>
	void StageClearReturnStart(const Vector3& cameraPos);

	/// <summary>
	/// ステージクリア後のブーストを開始する
	/// </summary>
	void StageClearBoostStart();

	//getter
	Vector3 GetWorldPos();
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const int GetHP() { return HP; }
	const bool GetIsDamageKnockback() { return isDamageKnockBack; }
	const bool GetIsCrash() { return isCrash; }
	const int GetCrashBoundCount() { return crashBoundCount; }
	const bool GetIsDead() { return isDead; }
	const bool GetIsRoll() { return isRoll; }
	MoveSpeedPhase GetMoveSpeedPhase() { return moveSpeedPhase; }
	const Vector3& GetKnockbackVel() { return knockbackVel; }
	PlayerReticles* GetReticles() { return reticles.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }
	const bool GetIsStageClearModeCompletion() { return isStageClearModeCompletion; }

private: //メンバ関数
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
	void CrashStart();

	/// <summary>
	/// 墜落
	/// </summary>
	void Crash();

	/// <summary>
	/// 回復
	/// </summary>
	void Heal();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

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
	/// ステージクリア後の横移動
	/// </summary>
	void StageClearSideMove();

	/// <summary>
	/// ステージクリア後の帰還
	/// </summary>
	void StageClearReturn();
	
	/// <summary>
	/// ステージクリア後の上昇
	/// </summary>
	void StageClearUp();

	/// <summary>
	/// ステージクリア後の停止
	/// </summary>
	void StageClearStay();

	/// <summary>
	/// ステージクリア後のブースト
	/// </summary>
	void StageClearBoost();

private: //静的メンバ変数
	//ゲームシーン
	static GameScene* gameScene;
	//自機弾のモデル
	static ObjModel* bulletModel;
	//ホーミング弾の大きさ
	static const float homingBulletSize;
	//自機の基準座標
	static const Vector3 basePos;
	//自機の回転限界
	static const Vector2 rotLimit;
	//自機の移動限界
	static const Vector2 moveLimitMax;
	static const Vector2 moveLimitMin;
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
	//ステージクリア後行動遷移
	static void (Player::* stageClearActionFuncTable[])();

private: //メンバ変数
	//最大体力
	int maxHP = 0;
	//体力
	int HP = 0;
	//HPUI
	std::unique_ptr<PlayerHPUI> hpUI;
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
	//墜落バウンド回数
	int crashBoundCount = 0;
	//墜落速度
	Vector3 crashVel = { 0, 0, 0.2f };
	//死亡フラグ
	bool isDead = false;
	//緊急回避中か
	bool isRoll = false;
	//緊急回避用タイマー
	int32_t rollTimer = 0;
	//緊急回避開始時のZ軸角度
	float rollStartRot = 0;
	//緊急回避終了時のZ軸角度
	float rollEndRot = 0;
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
	//ステージクリア後行動
	StageClearModePhase stageClearModePhase = StageClearModePhase::SideMove;
	//ステージクリア後に使用するタイマー
	int32_t stageClearModeTimer = 0;
	//ステージクリア移動方向が右か
	bool isStageClearMoveRight = true;
	//ステージクリア移動速度
	Vector3 stageClearMoveVelocity;
	//ステージクリア時角度
	Vector3 stageClearRota;
	//ステージクリア後に使用するカメラホーミング用座標
	Vector3 stageClearCameraPos;
	//カメラホーミング用座標
	Vector3 cameraHomingPos;
	//ステージクリア後の行動が完了したか
	bool isStageClearModeCompletion = false;
};
