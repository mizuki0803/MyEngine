#pragma once
#include "ObjObject3d.h"

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// ギャラクシー(胴体)
/// </summary>
class GalaxyBody : public ObjObject3d
{
public: //enum
	//登場の行動フェーズ
	enum class AppearPhase {
		Advance,	//前進
		Wait,		//待機
		Rotation,	//回転
	};

	//攻撃するパーツ変更の行動フェーズ
	enum class AttackPartChangePhase {
		Wait,		//待機
		Rotation,	//回転
	};


public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="bornPos">生成座標</param>
	/// <param name="basePos">基準座標</param>
	/// <returns>ギャラクシー(胴体)</returns>
	static GalaxyBody* Create(const Vector3& bornPos, const Vector3& basePos);

	//getter
	static const Vector3& GetNormalSize() { return GalaxyBody::normalSize; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { GalaxyBody::stageScene = stageScene; }
	static void SetBodyModel(ObjModel* model) { GalaxyBody::bodyModel = model; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Update() override;

	/// <summary>
	/// ダメージを喰らう
	/// </summary>
	void Damage();

	/// <summary>
	/// 上下ゆらゆら
	/// </summary>
	void Sway();

	/// <summary>
	/// 登場
	/// </summary>
	void Appear();

	/// <summary>
	/// 攻撃するパーツを変更する回転を開始
	/// </summary>
	/// <param name="rotSpeed">回転速度</param>
	/// <param name="changeRota">変更後の角度</param>
	void AttackPartChangeRotaStart(const float rotSpeed, const float changeRota);

	/// <summary>
	/// 攻撃するパーツを変更行動
	/// </summary>
	void AttackPartChange();

	/// <summary>
	/// 死亡
	/// </summary>
	void Dead();

	//getter
	const bool GetIsAppear() { return isAppear; }
	const bool GetIsAttackPartChangeRota() { return isAttackPartChangeRota; }

private: //メンバ関数
	/// <summary>
	/// ダメージ状態の処理
	/// </summary>
	void DamageMode();

	/// <summary>
	/// ダメージ状態で大きくしたサイズを戻していく処理
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// ダメージ爆発
	/// </summary>
	void DamageExplosion();

	/// <summary>
	/// ダメージを喰らった状態の色にする
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// 登場の前進行動
	/// </summary>
	void AppaerAdvance();

	/// <summary>
	/// 登場の待機行動
	/// </summary>
	void AppaerWait();

	/// <summary>
	/// 登場の回転行動
	/// </summary>
	void AppaerRotation();

	/// <summary>
	/// 攻撃するパーツ変更の待機行動
	/// </summary>
	void AttackPartChangeWait();

	/// <summary>
	/// 攻撃するパーツ変更の回転行動
	/// </summary>
	void AttackPartChangeRotation();

private: //静的メンバ変数
	//ステージシーン
	static BaseStageScene* stageScene;
	//胴体のモデル
	static ObjModel* bodyModel;
	//通常サイズ
	static const Vector3 normalSize;
	//ダメージ状態のサイズ
	static const Vector3 damageSize;
	//ダメージ状態の色
	static const XMFLOAT4 damageColor;
	//登場状態の行動遷移
	static void (GalaxyBody::* appearPhaseFuncTable[])();
	//攻撃するパーツ変更状態の行動遷移
	static void (GalaxyBody::* attackPartChangePhaseFuncTable[])();

private: //メンバ変数
	//生成座標
	Vector3 bornPos;
	//停止する基準の座標
	Vector3 basePos;
	//ダメージフラグ
	bool isDamage = false;
	//ダメージを喰らった瞬間か
	bool isDamageTrigger = false;
	//ダメージ状態タイマー
	int32_t damageTimer = 0;
	//ダメージ色か
	bool isDamageColor = false;
	//ゆらゆらが上移動か
	bool isSwayUp = false;
	//ゆらゆらの速度
	Vector3 swayVel = {};
	//ゆらゆらの速度が最大か
	bool isSwaySpeedMax = false;
	//行動に使用するタイマー
	int32_t actionTimer = 0;
	//登場中か
	bool isAppear = true;
	//登場の行動フェーズ
	AppearPhase appearPhase = AppearPhase::Advance;
	//攻撃するパーツ変更をする回転をするか
	bool isAttackPartChangeRota = false;
	//攻撃するパーツ変更の行動フェーズ
	AttackPartChangePhase attackPartChangePhase = AttackPartChangePhase::Wait;
	//攻撃するパーツ変更をする回転速度
	float attackPartChangeRotSpeed = 0;
	//攻撃するパーツ変更をする回転後の角度
	float  attackPartChangeRota = 0;
	//攻撃するパーツ変更をする回転が右向きか
	bool isAttackPartChangeRotaRight = true;
};
