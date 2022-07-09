#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

/// <summary>
/// 自機
/// </summary>
class Player : public ObjObject3d
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>自機</returns>
	static Player* Create(ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;


private: //メンバ関数
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private:
	//自機弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
};
