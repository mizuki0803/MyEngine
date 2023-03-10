#include "StageSelectPlayer.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

void (StageSelectPlayer::* StageSelectPlayer::actionFuncTable[])() = {
	&StageSelectPlayer::GooutPlanet,
	&StageSelectPlayer::StageSelectMove,
	&StageSelectPlayer::EnterPlanet,
};

void (StageSelectPlayer::* StageSelectPlayer::enterPlanetActionFuncTable[])() = {
	&StageSelectPlayer::EnterPlanetRotate,
	&StageSelectPlayer::EnterPlanetStay,
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

	//ジェット発射座標を更新
	UpdateJetPos();
	//自機のジェット噴射演出用パーティクル生成
	std::function<Vector3()> getTargetPos = std::bind(&StageSelectPlayer::GetJetPos, this);
	ParticleEmitter::GetInstance()->PlayerJet(getTargetPos, matWorld, particleJetSizePhaseNum);
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
	rotation = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);
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
	changeAfterRota = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);

	//惑星に入る状態にする
	actionPhase = ActionPhase::EnterPlanet;

	//行動タイマーを初期化
	actionTimer = 0;
}

void StageSelectPlayer::UpdateJetPos()
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.25f, -1.2f };

	//ジェット発射座標を取得
	jetPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void StageSelectPlayer::GooutPlanet()
{
	//惑星から出てくるのにかかる時間
	const float gooutTime = 210;
	//タイマー更新
	actionTimer++;
	const float time = actionTimer / gooutTime;

	//移動する方向を向く
	rotation = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);

	//イージングで動かす
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//大きさを変える
	const float beforeSize = 0.1f;
	const float afterSize = 1.5f;
	scale.x = Easing::InQuint(beforeSize, afterSize, time);
	scale.y = Easing::InQuint(beforeSize, afterSize, time);
	scale.z = Easing::InQuint(beforeSize, afterSize, time);

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
	const float moveTime = 40;
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
		const float changeAfterRota = 180;
		rotation.y = Easing::OutCubic(changeBeforeRota.y, changeAfterRota, rotTime);
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
		enterPlanetActionPhase = EnterPlanetActionPhase::Stay;

		//タイマーを初期化
		actionTimer = 0;
	}
}

void StageSelectPlayer::EnterPlanetStay()
{
	//待機する時間
	const float stayTime = 10;
	//タイマー更新
	actionTimer++;

	//タイマーが指定した時間になったら
	if (actionTimer >= stayTime) {
		//次のフェーズへ
		enterPlanetActionPhase = EnterPlanetActionPhase::Boost;

		//タイマーを初期化
		actionTimer = 0;

		//パーティクルジェットを大きくする(加速用にする)
		const int32_t particleSizePhase = 1;
		particleJetSizePhaseNum = particleSizePhase;

		//ポストエフェクトにラジアルブラーをかける
		if (!GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
			GamePostEffect::GetPostEffect()->SetRadialBlur(true);

			//ブラーの強さをセット
			const float blurStrength = 0.4f;
			GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
		}
	}
}

void StageSelectPlayer::EnterPlanetBoost()
{
	//惑星入るのにかかる時間
	const float enterTime = 100;
	//タイマー更新
	actionTimer++;
	float time = actionTimer / enterTime;
	time = min(time, 1);

	//移動する方向を向く
	rotation = Vector3::BetweenPointRotate(moveAfterPos, moveBeforePos);

	//イージングで動かす
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//大きさを変える
	const float beforeSize = 1.5f;
	const float afterSize = 0.2f;
	scale.x = Easing::OutQuint(beforeSize, afterSize, time);
	scale.y = Easing::OutQuint(beforeSize, afterSize, time);
	scale.z = Easing::OutQuint(beforeSize, afterSize, time);

	//タイマーが惑星に入るのを終える時間になったら
	const int endTime = 120;
	if (actionTimer >= endTime) {
		//ステージ選択で行う全ての行動を終える
		isStageSelectModeEnd = true;
	}
}
