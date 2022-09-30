#pragma once
#include "Reticle2D.h"
#include "Enemy.h"

/// <summary>
/// 自機付属の2Dレティクル(遠)
/// </summary>
class PlayerFarReticle2D : public Reticle2D
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>自機付属の2Dレティクル(遠)</returns>
	static PlayerFarReticle2D* Create(UINT texNumber, const Vector2& size);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// チャージ状態を開始する
	/// </summary>
	void ChargeModeStart();

	/// <summary>
	/// チャージ状態を終了する
	/// </summary>
	void ChargeModeEnd();

	/// <summary>
	/// 敵ロックオン
	/// </summary>
	/// <param name="enemy">ロックオン対象の敵</param>
	void LockonEnemy(Enemy* enemy);

	/// <summary>
	/// 敵ロックオン解除
	/// </summary>
	void UnlockonEnemy();

	//getter
	const bool GetIsChargeMode() { return isChargeMode; }
	const bool GetIsLockon() { return isLockon; }
	Enemy* GetLockonEnemy() { return lockonEnemy; }

	//setter
	void SetWorldPos3d(const Vector3& worldPos3d) { this->worldPos3d = worldPos3d; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

private: //メンバ変数
	/// <summary>
	/// チャージ状態の挙動
	/// </summary>
	void ChargeMode();

	/// <summary>
	/// チャージ状態開始状態の大きさ変更処理
	/// </summary>
	void ChargeModeStartSizeChange();

	/// <summary>
	/// チャージ状態終了状態の大きさ変更処理
	/// </summary>
	void ChargeModeEndSizeChange();

	/// <summary>
	/// チャージ状態の通常時大きさ変更処理
	/// </summary>
	void ChargeModeSizeChange();

	/// <summary>
	/// ロックオン時の移動
	/// </summary>
	void MoveLockon();

	/// <summary>
	/// 通常移動
	/// </summary>
	void Move() override;


private: //静的メンバ変数
	//ロックオン時の色
	static const XMFLOAT4 chargeLockonColor;

private:
	//通常時の大きさ
	Vector2 normalSize;
	//チャージ状態か
	bool isChargeMode = false;
	//チャージ状態開始状態か
	bool isChargeModeStart = false;
	//チャージ状態終了状態か
	bool isChargeModeEnd = false;
	//チャージ状態用タイマー
	int32_t chargeTimer = 0;
	//変更前の大きさ記録用
	Vector2 beforeChangeSize;
	//ロックオン敵
	Enemy* lockonEnemy = nullptr;
	//ロックオンしているか
	bool isLockon = false;
	//レティクルを元の位置(自機追従)に戻すか
	bool isBackPos = false;
	//レティクルを元の位置(自機追従)に戻すタイマー
	float backTimer = 0;
};
