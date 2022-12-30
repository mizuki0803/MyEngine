#include "MedamanAvatarType02.h"
#include "Easing.h"

MedamanAvatarType02* MedamanAvatarType02::Create(ObjObject3d* parent, const Vector3& position)
{
	//メダマーン(分身：タイプ02)のインスタンスを生成
	MedamanAvatarType02* medamanAvatarType02 = new MedamanAvatarType02();
	if (medamanAvatarType02 == nullptr) {
		return nullptr;
	}

	//寝ている状態のモデルをセット
	assert(avatarSleepModel);
	medamanAvatarType02->model = avatarSleepModel;

	// 初期化
	if (!medamanAvatarType02->Initialize()) {
		delete medamanAvatarType02;
		assert(0);
		return nullptr;
	}

	//親をセット
	medamanAvatarType02->parent = parent;

	//座標をセット
	medamanAvatarType02->position = position;

	//親子関係上での基準位置をセット
	medamanAvatarType02->basePos = position;

	return medamanAvatarType02;
}

void MedamanAvatarType02::AttackTypeAvatarBodyBlowLockon()
{
	//タイマーを更新
	const float backTime = 150;
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

void MedamanAvatarType02::AttackTypeAvatarGatlingMoveCenter()
{
	//タイマーを更新
	const float moveTime = 120;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//親子関係上での基準位置に移動させる
	position.x = Easing::OutQuad(basePos.x, -attackAvatarGatlingLength, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGatlingPhase = AttackTypeAvatarGatlingPhase::RotStart;

		//回転開始用に角度を設定する
		attackAvatarGatlingAngle = 180;

		//タイマー初期化
		attackTimer = 0;
	}
}

void MedamanAvatarType02::AttackTypeAvatarGiantBulletWait()
{
	//次のフェーズへ
	attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::Move;
}

void MedamanAvatarType02::AttackTypeAvatarGiantBulletMove()
{
	//タイマーを更新
	const float moveTime = 150;
	attackTimer++;
	const float time = attackTimer / moveTime;

	//発射位置に移動させる
	const Vector3 movePos = { -6, 0, 0 };
	position.x = Easing::OutQuad(basePos.x, movePos.x, time);
	position.y = Easing::OutQuad(basePos.y, movePos.y, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackTimer >= moveTime) {
		attackAvatarGiantBulletPhase = AttackTypeAvatarGiantBulletPhase::ChargeShot;

		//タイマー初期化
		attackTimer = 0;
	}
}
