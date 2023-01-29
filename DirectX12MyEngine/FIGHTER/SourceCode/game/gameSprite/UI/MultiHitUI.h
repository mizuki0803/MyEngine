#pragma once
#include "NumberSprite.h"
#include "BaseGameCamera.h"
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
	/// <param name="position">座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="enemyHitNum">撃破数</param>
	/// <returns>複数撃破UI</returns>
	static MultiHitUI* Create(const Vector3& position, BaseGameCamera* camera, const int enemyHitNum);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="enemyHitNum">撃破数</param>
	/// <returns>成否</returns>
	bool Initialize(const Vector3& position, BaseGameCamera* camera, const int enemyHitNum);

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
	/// <param name="position">座標</param>
	/// <param name="camera">カメラ</param>
	/// <returns>撃破数を表示するスクリーン座標</returns>
	Vector2 GetScreenPos(const Vector3& position, BaseGameCamera* camera);

	/// <summary>
	/// テキストを上に動かす
	/// </summary>
	void TextMove();

	/// <summary>
	/// 画面手前まで行ったら削除する処理
	/// </summary>
	void FrontOfScreenDelete();

private: //静的メンバ変数
	//寿命時間
	static const int32_t lifeTime = 180;

private: //メンバ変数
	//カメラ
	BaseGameCamera* camera = nullptr;
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