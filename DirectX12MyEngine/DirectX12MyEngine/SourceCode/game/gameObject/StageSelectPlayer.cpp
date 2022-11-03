#include "StageSelectPlayer.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (StageSelectPlayer::* StageSelectPlayer::actionFuncTable[])() = {
	&StageSelectPlayer::GooutPlanet,
	&StageSelectPlayer::StageSelectMove,
	&StageSelectPlayer::EnterPlanet,
};

void (StageSelectPlayer::* StageSelectPlayer::enterPlanetActionFuncTable[])() = {
	&StageSelectPlayer::EnterPlanetRotate,
	&StageSelectPlayer::EnterPlanetBoost,
};

StageSelectPlayer* StageSelectPlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//ステージ選択シーン用自機のインスタンスを生成
	StageSelectPlayer* stageSelectPlayer = new StageSelectPlayer();
	if (stageSelectPlayer == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	stageSelectPlayer->model = model;

	// 初期化
	if (!stageSelectPlayer->Initialize()) {
		delete stageSelectPlayer;
		assert(0);
		return nullptr;
	}

	//初期座標をセット
	stageSelectPlayer->position = startPosition;

	//大きさをセット
	const float size = 1.5f;
	stageSelectPlayer->scale = { size ,size ,size };

	return stageSelectPlayer;
}

void StageSelectPlayer::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(actionPhase)])();

	//オブジェクト更新
	ObjObject3d::Update();

	//パーティクル演出
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}

void StageSelectPlayer::GooutPlanetStart(const Vector3& targetPos)
{
	//移動前座標をセット
	moveBeforePos = position;
	//移動後座標をセット
	const Vector3 distance = { 0, 5, 0 };
	const Vector3 afterPos = targetPos + distance;
	moveAfterPos = afterPos;

	//惑星から出てくる状態にする
	actionPhase = ActionPhase::GooutPlanet;

	//行動タイマーを初期化
	actionTimer = 0;
}

void StageSelectPlayer::StageSelectMoveStart(const Vector3& targetPos, bool isStageSelectMoveRight)
{
	//移動前座標をセット
	moveBeforePos = position;
	//移動後座標をセット
	const Vector3 distance = { 0, 5, 0 };
	const Vector3 afterPos = targetPos + distance;
	moveAfterPos = afterPos;

	//移動する方向を向く
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	rotation.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));
	//変更前の角度を記録
	changeBeforeRota = rotation;

	//移動方向が左なら角度を修正
	if (!isStageSelectMoveRight) { changeBeforeRota.y += 360; }

	//ステージ選択で移動中にする
	isStageSelectMove = true;

	//行動タイマーを初期化
	actionTimer = 0;
}

void StageSelectPlayer::EnterPlanetStart(const Vector3& selectPlanetPos)
{
	//移動前座標をセット
	moveBeforePos = position;
	//移動後座標をセット
	moveAfterPos = selectPlanetPos;
	//惑星の手前に設定しておく
	const Vector3 distance = { 0, 6.0f, -27.0f };
	moveAfterPos += distance;

	//変更前の角度をセット
	changeBeforeRota = rotation;

	//変更後の角度をセット
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	changeAfterRota.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-changeAfterRota.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	changeAfterRota.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));

	//惑星に入る状態にする
	actionPhase = ActionPhase::EnterPlanet;

	//行動タイマーを初期化
	actionTimer = 0;
}

void StageSelectPlayer::GooutPlanet()
{
	//惑星から出てくるのにかかる時間
	const float gooutTime = 300;
	//タイマー更新
	actionTimer++;
	const float time = actionTimer / gooutTime;

	//移動する方向を向く
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	rotation.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));

	//イージングで動かす
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//大きさを変える
	scale.x = Easing::InQuint(0.1f, 1.5f, time);
	scale.y = Easing::InQuint(0.1f, 1.5f, time);
	scale.z = Easing::InQuint(0.1f, 1.5f, time);

	//タイマーが指定した時間になったら
	if (actionTimer >= gooutTime) {
		//次のフェーズへ
		actionPhase = ActionPhase::StageSelect;

		//惑星から出てくる行動を終える
		isGooutPlanetEnd = true;
	}
}

void StageSelectPlayer::StageSelectMove()
{
	//ステージ選択で移動中状態でなければ抜ける
	if (!isStageSelectMove) { return; }

	//移動にかかる時間
	const float moveTime = 50;
	//タイマー更新
	actionTimer++;
	const float time = actionTimer / moveTime;

	//イージングで動かす
	position.x = Easing::OutCubic(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutCubic(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutCubic(moveBeforePos.z, moveAfterPos.z, time);

	//残りの指定した時間から角度を修正する
	const float rotChangeTime = 10;
	if (actionTimer >= moveTime - rotChangeTime) {
		const float rotTime = (actionTimer - (moveTime - rotChangeTime)) / rotChangeTime;
		//イージングで動かす
		rotation.y = Easing::OutCubic(changeBeforeRota.y, 180, rotTime);
	}

	//タイマーが指定した時間になったら
	if (actionTimer >= moveTime) {
		//ステージ選択で移動中状態を解除
		isStageSelectMove = false;
	}
}

void StageSelectPlayer::EnterPlanet()
{
	//惑星に入る行動
	(this->*enterPlanetActionFuncTable[static_cast<size_t>(enterPlanetActionPhase)])();
}

void StageSelectPlayer::EnterPlanetRotate()
{
	//回転にかかる時間
	const float rotTime = 180;
	//タイマー更新
	actionTimer++;
	const float time = actionTimer / rotTime;

	//イージングで回転させる
	rotation.x = Easing::OutQuint(changeBeforeRota.x, changeAfterRota.x, time);
	rotation.y = Easing::OutQuint(changeBeforeRota.y, changeAfterRota.y, time);

	//タイマーが指定した時間になったら
	if (actionTimer >= rotTime) {
		//次のフェーズへ
		enterPlanetActionPhase = EnterPlanetActionPhase::Boost;

		//タイマーを初期化
		actionTimer = 0;
	}
}

void StageSelectPlayer::EnterPlanetBoost()
{
	//惑星入るのにかかる時間
	const float enterTime = 300;
	//タイマー更新
	actionTimer++;
	const float time = actionTimer / enterTime;

	//移動する方向を向く
	const Vector3 moveVec = moveAfterPos - moveBeforePos;
	rotation.y = XMConvertToDegrees(std::atan2(moveVec.x, moveVec.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 moveVecZ = MatrixTransformDirection(moveVec, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-moveVecZ.y, moveVecZ.z));

	//イージングで動かす
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//大きさを変える
	scale.x = Easing::OutQuint(1.5f, 0.2f, time);
	scale.y = Easing::OutQuint(1.5f, 0.2f, time);
	scale.z = Easing::OutQuint(1.5f, 0.2f, time);

	//タイマーが惑星に入るのを終える時間になったら
	const int endTime = 240;
	if (actionTimer >= endTime) {
		//ステージ選択で行う全ての行動を終える
		isStageSelectModeEnd = true;
	}
}
