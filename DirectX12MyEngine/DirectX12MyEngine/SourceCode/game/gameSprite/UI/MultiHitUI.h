#pragma once
#include "NumberSprite.h"
#include "Camera.h"
#include <vector>
#include <memory>

/// <summary>
/// 複数撃破UI
/// </summary>
class MultiHitUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>複数撃破UI</returns>
	static MultiHitUI* Create(const Vector3& position, Camera* camera, const int enemyHitNum);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(const Vector3& position, Camera* camera, const int enemyHitNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	bool GetIsDead() { return isDead; }

private: //メンバ関数
	/// <summary>
	/// スクリーン座標取得
	/// </summary>
	Vector2 GetScreenPos(const Vector3& position, Camera* camera);

	/// <summary>
	/// テキストを上に動かす
	/// </summary>
	void TextMove();

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 180;

private: //メンバ変数
	//カメラ
	Camera* camera = nullptr;
	//「HIT」テキストスプライト
	std::unique_ptr<Sprite> hitTextSprite;
	//数字スプライト
	std::vector<std::unique_ptr<NumberSprite>> numberSprites;
	//ワールド座標
	Vector3 hitWorldPos;
	//取得用撃破数
	int hitNum = 0;
	//取得用撃破数の桁数
	int hitNumDigit = 0;
	//寿命タイマー
	int32_t deadTimer = 0;
	//死亡フラグ
	bool isDead = false;
};