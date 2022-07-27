#pragma once
#include "Sprite.h"
#include "Reticle3D.h"
#include "Enemy.h"

/// <summary>
/// 2Dレティクル
/// </summary>
class Reticle2D : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>レティクル2D</returns>
	static Reticle2D* Create(UINT texNumber, const Vector2& size);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(UINT texNumber, Vector2 anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

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
	const bool GetIsLockon() { return isLockon; }
	Enemy* GetLockonEnemy() { return this->lockonEnemy; }

	//setter
	void SetWorldPos3d(const Vector3& worldPos3d) { this->worldPos3d = worldPos3d; }
	void SetMatViewProjection(const XMMATRIX& matView, const XMMATRIX& matProjection) {
		this->matView = matView;
		this->matProjection = matProjection;
	}

private: //メンバ変数
	/// <summary>
	/// ロックオン時の移動
	/// </summary>
	void MoveLockon();

	/// <summary>
	/// ロックオンではないときの移動
	/// </summary>
	void MoveNotLockon();

private: //静的メンバ変数
	//通常時の色
	static const XMFLOAT4 normalColor;
	//ロックオン時の色
	static const XMFLOAT4 lockonColor;

private:
	//3Dレティクルのワールド座標
	Vector3 worldPos3d = {};
	//ビュー行列
	XMMATRIX matView = {};
	//プロジェクション行列
	XMMATRIX matProjection = {};
	//敵
	Enemy* lockonEnemy = nullptr;
	//ロックオンしているか
	bool isLockon = false;
	//レティクルを元の位置(自機追従)に戻すか
	bool isBackPos = false;
	//レティクルを元の位置(自機追従)に戻すタイマー
	float backTimer = 0;
};
