#include "MedamanAvatarType04.h"
#include "Easing.h"

MedamanAvatarType04* MedamanAvatarType04::Create(ObjObject3d* parent, const Vector3& position)
{
	//メダマーン(分身：タイプ04)のインスタンスを生成
	MedamanAvatarType04* medamanAvatarType04 = new MedamanAvatarType04();
	if (medamanAvatarType04 == nullptr) {
		return nullptr;
	}

	//寝ている状態のモデルをセット
	assert(avatarSleepModel);
	medamanAvatarType04->model = avatarSleepModel;

	// 初期化
	if (!medamanAvatarType04->Initialize()) {
		delete medamanAvatarType04;
		assert(0);
		return nullptr;
	}

	//親をセット
	medamanAvatarType04->parent = parent;

	//座標をセット
	medamanAvatarType04->position = position;

	//親子関係上での基準位置をセット
	medamanAvatarType04->basePos = position;

	return medamanAvatarType04;
}

void MedamanAvatarType04::AttackTypeAvatarBodyBlowLockon()
{
	//タイマーを更新
	const float backTime = 180;
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= backTime) {
		attackAvatarBodyBlowPhase = AttackTypeAvatarBodyBlowPhase::Shot;

		//飛ばす速度を確定させる
		attackAvatarBodyBlowVelocity = attackAvatarBodyBlowLockonPos - GetWorldPos();
		const float moveSpeed = 0.5f;
		attackAvatarBodyBlowVelocity = attackAvatarBodyBlowVelocity.normalize() * moveSpeed;

		//タイマー初期化
		attackTimer = 0;
	}
}

void MedamanAvatarType04::AttackTypeAvatarGatlingMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//親子関係上での基準位置に移動させる
	position.y = Easing::OutQuad(basePos.y, -attackAvatarGatlingLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotStart;

		//回転開始用に角度を設定する
		attackAvatarGatlingAngle = 270;

		//タイマー初期化
		attackTimer = 0;
	}
}

void MedamanAvatarType04::AttackTypeAvatarGiantBulletWait()
{
	//タイマーを更新
	const float waitTime = 60;
	attackTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= waitTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;

		//タイマー初期化
		attackTimer = 0;
	}
}

void MedamanAvatarType04::AttackTypeAvatarGiantBulletMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//発射位置に移動させる
	const Vector3 movePos = { 2, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::ChargeShot;

		//タイマー初期化
		attackTimer = 0;
	}
}
