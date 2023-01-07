#include "BasePlayer.h"
#include "Input.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "SpriteTextureLoader.h"
#include "StraightBullet.h"
#include "HomingBullet.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

BaseStageScene* BasePlayer::stageScene = nullptr;
ObjModel* BasePlayer::bulletModel = nullptr;
const float BasePlayer::homingBulletSize = 2.5f;
const Vector2 BasePlayer::rotLimit = { 35.0f, 25.0f };
const XMFLOAT4 BasePlayer::damageColor = { 1, 0, 0, 1 };
const float BasePlayer::moveBaseSpeed = 0.16f;
const float BasePlayer::knockbackBaseSpeed = 0.25f;
const float BasePlayer::maxSpeedChangeGauge = 100.0f;
const float BasePlayer::normalSpeedBlurStrength = 0.03f;

bool BasePlayer::Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate)
{
	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//初期座標をセット
	position = basePos;
	//大きさをセット
	scale = { 1.5f, 1.5f, 1.5f };

	//モデルをセット
	assert(model);
	this->model = model;

	//レティクルを生成
	reticles.reset(PlayerReticles::Create());

	//HPをセット
	HP = startHP;
	//最大HPをセット
	this->maxHP = maxHP;

	//飛行機雲演出を生成するかセット
	this->isVaporCreate = isVaporCreate;

	//HPUI生成
	const Vector2 hpUIPosition = { 20, 20 };
	hpUI.reset(PlayerHPUI::Create(hpUIPosition, HP, maxHP));
	//速度変更ゲージUI生成
	const Vector2 speedChangeUIPosition = { 20, 70 };
	speedChangeUI.reset(PlayerSpeedChangeUI::Create(speedChangeUIPosition, maxSpeedChangeGauge));
	//ダメージ演出生成
	damageEffect.reset(PlayerDamageEffect::Create());

	//飛行機雲演出生成
	if (isVaporCreate) { vaporEffect.reset(new PlayerVaporEffect()); }

	//ポストエフェクトにラジアルブラーをかける
	if (!GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
		GamePostEffect::GetPostEffect()->SetRadialBlur(true);

		//通常移動時のブラーの強さをセット
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(normalSpeedBlurStrength);
	}

	return true;
}

void BasePlayer::Update()
{
	//削除状態の緊急回避演出を削除
	rollingEffects.remove_if([](std::unique_ptr<PlayerRollingEffect>& rollingEffect) {
		return rollingEffect->GetIsDelete();
		});
	//緊急回避演出更新
	for (const std::unique_ptr<PlayerRollingEffect>& rollingEffect : rollingEffects) {
		rollingEffect->Update();
	}

	//自機のジェット噴射演出用パーティクル生成
	JetEffectManager();

	//死亡状態なら抜ける
	if (isDead) { return; }

	//行動
	Action();

	//オブジェクト更新
	ObjObject3d::Update();

	//両翼の座標更新
	if (isVaporCreate) { UpdateWingPos(); }

	//レティクル更新
	reticles->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());
}

void BasePlayer::Draw()
{
	//死亡状態なら抜ける
	if (isDead) { return; }

	//オブジェクト描画
	ObjObject3d::Draw();
}

void BasePlayer::UpdateUI()
{
	//死亡状態なら抜ける
	if (isDead) { return; }

	//HPUI更新
	hpUI->Update();
	//速度変更ゲージUI更新
	speedChangeUI->Update();
	//ダメージ演出更新
	damageEffect->Update();
}

void BasePlayer::DrawUI()
{
	//墜落状態でないなら&&ステージクリア状態でないなら
	if (!isCrash && !isStageClearMode) {
		//レティクル描画
		reticles->Draw();

		//HPUI描画
		hpUI->Draw();

		//速度変更ゲージUI描画
		speedChangeUI->Draw();
	}

	//ダメージ演出描画
	damageEffect->Draw();
}

void BasePlayer::OnCollisionDamage(const Vector3& subjectPos)
{
	//ダメージを喰らう
	Damage();

	//ノックバック情報をセット
	SetDamageKnockback(subjectPos);

	//ダメージを喰らったのでHPバーの長さを変更する
	hpUI->Damage(HP);

	//ダメージ演出を開始する
	damageEffect->DamageEffectStart(maxHP, HP);
}

void BasePlayer::OnCollisionHeal()
{
	//回復
	Heal();

	//回復アイテムを獲得したのでHPUIを回復状態にする
	hpUI->ItemGet(HP);
}

void BasePlayer::UpdateWingPos()
{
	//自機の中心座標からの距離
	const Vector3 leftDistancePos = { -2.0f, -0.2f, -0.9f };
	const Vector3 rightDistancePos = { 2.0f, -0.2f, -0.9f };
	//左翼座標を取得
	const Vector3 leftWingPos = LocalTranslation(leftDistancePos, matWorld);
	//右翼座標を取得
	const Vector3 rightWingPos = LocalTranslation(rightDistancePos, matWorld);

	vaporEffect->Update(leftWingPos, rightWingPos);
}

void BasePlayer::Action()
{
	//ステージクリア状態
	if (isStageClearMode) {
		//ステージクリア後行動
		StageClear();
	}
	//ステージクリア以外の状態
	else {
		//墜落状態
		if (isCrash) {
			Crash();
		}
		//それ以外
		else {
			//回転
			Rotate();

			//緊急回避
			Roll();

			//速度変更
			SpeedChange();

			//ダメージノックバック状態
			if (isDamageKnockBack) {
				DamageKnockback();
			}
			//ダメージ状態でないなら通常移動
			else {
				Move();
			}

			//攻撃
			Attack();
		}
	}

	//ダメージ状態
	if (isDamage) {
		DamageMode();
	}
}

void BasePlayer::JetEffectManager()
{
	//墜落状態以外なら通常ジェット演出
	if (!isCrash) {
		ParticleEmitter::GetInstance()->PlayerJet(matWorld, (int)moveSpeedPhase);
	}
	//墜落状態なら黒煙をジェットの代わりに出す
	else {
		CrashBlackSmoke();
	}
}

void BasePlayer::CrashBlackSmoke()
{
	//墜落開始してから黒煙を出すまでの時間
	const int smokeStartTime = 30;
	//黒煙用タイマー更新
	blackSmokeTimer++;

	//タイマーが黒煙を出すまでの時間以下なら抜ける
	if (blackSmokeTimer < smokeStartTime) { return; }

	//毎フレーム出すと多いので間隔を設定
	const int smokeInterval = 4;
	//指定した間隔以外なら抜ける
	if (blackSmokeTimer % smokeInterval != 0) { return; }

	//黒煙パーティクル生成
	ParticleEmitter::GetInstance()->PlayerBlackSmokeJet(matWorld);
}

void BasePlayer::Damage()
{
	//体力を減らす
	HP -= 10;

	//HPが0以下になったら墜落させる
	if (HP <= 0) {
		CrashStart();

		//HPは0以下にならない
		HP = 0;

		//爆発演出用パーティクル生成
		const float explosionSize = 0.5f;
		const int explosionTime = 20;
		ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
	}

	//ダメージ状態にする
	isDamage = true;
	//ダメージ状態タイマー初期化
	damageTimer = 0;
	//ノックバック状態にする
	isDamageKnockBack = true;
	//ダメージ状態色に変更
	color = damageColor;
}

void BasePlayer::DamageMode()
{
	//ダメージ状態の時間
	const int damageTime = 80;
	//ダメージ状態タイマーを更新
	damageTimer++;

	//ダメージ色切り替え
	DamageColorChange();

	//タイマーが指定した時間になったら
	if (damageTimer >= damageTime) {
		//ダメージ状態を終了
		isDamage = false;

		//色を元に戻しておく
		color = { 1, 1, 1, 1 };
	}
}

void BasePlayer::SetDamageKnockback(const Vector3& subjectPos)
{
	//ノックバックする方向を決める(自機のワールド座標 - 対象のワールド座標)
	knockbackVec = GetWorldPos() - subjectPos;
	//ベクトルを正規化
	knockbackVec.normalize();
}

void BasePlayer::DamageKnockback()
{
	//ノックバックする時間
	const float knockbackTime = 35;
	//指定した時間以上なら抜ける
	if (damageTimer > knockbackTime) {
		//ノックバック状態なら解除してから抜ける
		if (isDamageKnockBack) { isDamageKnockBack = false; }
		return;
	}

	const float time = damageTimer / knockbackTime;

	//速度を作成(ノックバック速度 * 基準の速度 * 時間による減速)
	knockbackVel = knockbackVec * knockbackBaseSpeed * (1 - time);
	//Z軸方向には移動しないようにする
	knockbackVel.z = 0;

	//自機をノックバックさせる
	position += knockbackVel;

	//移動限界から出ないようにする
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void BasePlayer::DamageColorChange()
{
	//ダメージ色切り替え間隔時間
	const int colorChangeInterval = 3;

	//タイマーが指定した間隔以外なら抜ける
	if (damageTimer % colorChangeInterval != 0) { return; }

	//ダメージ色状態を切り替える
	if (isDamageColor) {
		isDamageColor = false;

		//色を元に戻す
		color = { 1, 1, 1, 1 };
	}
	else {
		isDamageColor = true;

		//ダメージ色にする
		color = damageColor;
	}
}

void BasePlayer::CrashStart()
{
	//カメラ追従を解除
	SetIsCameraFollow(false);
	//カメラ追従解除により、ローカル座標にワールド座標を代入
	position = GetWorldPos();

	//パーティクルの大きさを統一するため、移動はもうしないが通常移動状態にしておく
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;

	//飛行機雲の生成を終了する
	if (isVaporCreate) { vaporEffect->VaporEnd(); }

	//墜落状態にする
	isCrash = true;
}

void BasePlayer::Heal()
{
	//体力を増やす
	const int healNum = 10;
	HP += healNum;

	//HPは最大HP以上にならない
	if (HP >= maxHP) {
		HP = maxHP;
	}
}

void BasePlayer::Rotate()
{
	Input* input = Input::GetInstance();

	//回転速度
	const float rotSpeed = 1.0f;
	//角度修正基準速度
	const float backBaseSpeed = rotSpeed / 1.5f;
	Vector3 rot = { 0, 0, 0 };


	//どこまで傾けたら判定をとるか
	const float stickNum = 200;
	//y軸回転
	{
		//キー入力で回転させる
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
			if (input->PushKey(DIK_A)) { rot.y -= rotSpeed; }
			else if (input->PushKey(DIK_D)) { rot.y += rotSpeed; }
		}
		//パッドスティックX軸の判定を取る
		else if (input->TiltGamePadLStickX(stickNum) || input->TiltGamePadLStickX(-stickNum)) {
			//自機はスティックを倒した方向に動く
			const float padRota = input->GetPadLStickAngle();
			const float moveAngle = XMConvertToRadians(padRota);
			const float padStickIncline = input->GetPadLStickIncline().x;
			rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
		}
		//キー入力なし&スティックを倒していない場合
		else {
			//角度修正速度倍率
			float backSpeedRatio = fabsf(rotation.y / (rotLimit.y * 2)) + 0.5f;
			//角度修正速度
			const float backSpeed = backBaseSpeed * backSpeedRatio;
			//y軸回転の傾きを修正する
			const float rotMin = 0.5f;
			if (rotation.y > rotMin) {
				rot.y -= backSpeed;
			}
			else if (rotation.y < -rotMin) {
				rot.y += backSpeed;
			}
			else {
				rotation.y = 0;
			}
		}
	}
	//x軸回転
	{
		//キー入力で回転させる
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S)) {
			if (input->PushKey(DIK_W)) { rot.x -= rotSpeed; }
			if (input->PushKey(DIK_S)) { rot.x += rotSpeed; }
		}
		//パッドスティックY軸の判定を取る
		else if (input->TiltGamePadLStickY(stickNum) || input->TiltGamePadLStickY(-stickNum)) {
			//自機はスティックを倒した方向に動く
			const float padRota = input->GetPadLStickAngle();
			const float moveAngle = XMConvertToRadians(padRota);
			const float padStickIncline = input->GetPadLStickIncline().y;
			rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
		}
		//キー入力なし&スティックを倒していない場合
		else {
			//角度修正速度倍率
			float backSpeedRatio = fabsf(rotation.x / (rotLimit.x * 2)) + 0.5f;
			//角度修正速度
			const float backSpeed = backBaseSpeed * backSpeedRatio;
			//x軸回転の傾きを修正する
			const float rotMin = 0.5f;
			if (rotation.x > rotMin) {
				rot.x -= backSpeed;
			}
			else if (rotation.x < -rotMin) {
				rot.x += backSpeed;
			}
			else {
				rotation.x = 0;
			}
		}
	}
	//z軸回転
	{
		//動きがないと寂しいのでゆらゆらさせておく
		const float rotZSpeed = 0.04f;
		const float rotZLimit = 1.0f;
		//右回転
		if (isRotZRight) {
			swayZ += rotZSpeed;
			if (swayZ >= rotZLimit) {
				isRotZRight = false;
			}
		}
		//左回転
		else {
			swayZ -= rotZSpeed;
			if (swayZ <= -rotZLimit) {
				isRotZRight = true;
			}
		}

		rotation.z = -rotation.y + swayZ;
	}

	//回転の更新
	rotation += rot;

	//角度の限界値からはみ出さない
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);
	rotation.x = max(rotation.x, -rotLimit.x);
	rotation.x = min(rotation.x, +rotLimit.x);
}

void BasePlayer::Move()
{
	//自機が傾いている角度に移動させる
	Vector3 velocity = { 0, 0, 0 };
	velocity.x = moveBaseSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveBaseSpeed * -(rotation.x / rotLimit.x);
	position += velocity;

	//移動限界から出ないようにする
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void BasePlayer::Roll()
{
	//緊急回避時の動き
	if (isRoll) {
		RollMode();
	}
	//緊急回避可能時
	else {
		//開始するか判定
		RollStart();
	}
}

void BasePlayer::RollStart()
{
	//ダメージ状態なら緊急回避は発動できないで抜ける
	if (isDamage) { return; }

	Input* input = Input::GetInstance();
	//移動方向の入力かつ正しい方向の緊急回避入力をしていなければ抜ける
	bool isInputRightRoll = (input->TriggerKey(DIK_RSHIFT) || input->TriggerGamePadButton(Input::PAD_RB));
	bool isInputLeftRoll = (input->TriggerKey(DIK_LSHIFT) || input->TriggerGamePadButton(Input::PAD_LB));

	if (!(isInputRightRoll || isInputLeftRoll)) { return; }

	//緊急回避状態にする
	isRoll = true;
	//タイマーを初期化
	rollTimer = 0;
	//緊急回避開始時のZ軸角度を記憶
	rollStartRot = rotation.z;

	//緊急回避終了時のZ軸角度をセット
	const float rotAmount = 360; //回転量
	if (isInputRightRoll) { rollEndRot = -rotAmount; }		//右回転
	else if (isInputLeftRoll) { rollEndRot = rotAmount; }	//左回転

	//緊急回避演出生成
	std::unique_ptr<PlayerRollingEffect> newRollingEffect;
	std::function<Vector3()> getTergetPos = std::bind(&BasePlayer::GetWorldPos, this);
	newRollingEffect.reset(PlayerRollingEffect::Create(getTergetPos, true));
	rollingEffects.push_back(std::move(newRollingEffect));
}

void BasePlayer::RollMode()
{
	//タイマーを更新
	const float rollTime = 45;
	rollTimer++;
	const float time = rollTimer / rollTime;

	//Z軸回転する
	const float endRot = rollEndRot - rotation.y + swayZ;
	rotation.z = Easing::OutBack(rollStartRot, endRot, time);

	//タイマーが指定した時間になったら緊急回避終了
	if (rollTimer >= rollTime) {
		isRoll = false;
	}
}

void BasePlayer::SpeedChange()
{
	Input* input = Input::GetInstance();

	//加速入力
	const bool isPushHighSpeedInput = (input->PushGamePadButton(Input::PAD_X) || input->PushKey(DIK_X));
	//減速入力
	const bool isPushSlowSpeedInput = (input->PushGamePadButton(Input::PAD_A) || input->PushKey(DIK_Z));

	//速度変更開始可能時
	if (isSpeedChangeStartPossible) {
		//開始するか判定
		SpeedChangeStart(isPushHighSpeedInput, isPushSlowSpeedInput);
	}
	//速度変更開始可能ではないとき
	else {
		//速度変更中のとき
		if (isSpeedChange) {
			//速度変更中(加速or減速)の処理
			SpeedChangeMode(isPushHighSpeedInput, isPushSlowSpeedInput);
		}
		//速度変更を終え、元の速度に戻るとき
		else {
			//ゲージを元に戻していく
			SpeedChangeModeEnd();
		}

		//ゲージが溢れないようにする
		speedChangeGauge = max(speedChangeGauge, 0);
		speedChangeGauge = min(speedChangeGauge, maxSpeedChangeGauge);
		//ゲージ変更処理
		speedChangeUI->ChangeLength(speedChangeGauge);
	}

	//加速or減速どちらでもない場合、元の位置に戻す
	SpeedChangeNormalSpeed();
	//ブラーの強さを通常移動時の強さに戻していく
	SpeedChangeNormalBlur();
}

void BasePlayer::SpeedChangeStart(bool isPushHighSpeedInput, bool isPushSlowSpeedInput)
{
	//加速or減速のキーボタンの入力がなければ抜ける
	if (!(isPushHighSpeedInput || isPushSlowSpeedInput)) { return; }

	//加速状態をセット
	if (isPushHighSpeedInput) {
		moveSpeedPhase = MoveSpeedPhase::HighSpeed;

		//飛行機雲の生成を開始する
		if (isVaporCreate) { vaporEffect->VaporStart(); }

		//ポストエフェクトのラジアルブラーを強める
		if (GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
			const float highSpeedBlur = 0.4f;
			GamePostEffect::GetPostEffect()->SetRadialBlurStrength(highSpeedBlur);
		}
	}
	else if (isPushSlowSpeedInput) {
		moveSpeedPhase = MoveSpeedPhase::SlowSpeed;

		//ポストエフェクトのラジアルブラーを弱める
		if (GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
			const float slowSpeedBlur = 0.005f;
			GamePostEffect::GetPostEffect()->SetRadialBlurStrength(slowSpeedBlur);
		}
	}

	//速度変更状態にする
	isSpeedChange = true;
	//速度変更開始可能ではなくする
	isSpeedChangeStartPossible = false;
	//速度変更タイマーを初期化
	speedChangeTimer = 0;
}

void BasePlayer::SpeedChangeMode(bool isPushHighSpeedInput, bool isPushSlowSpeedInput)
{
	//タイマー更新
	speedChangeTimer++;

	//ゲージを減らす速さ
	const float gaugeDecSpeed = 1.3f;
	//加速状態のとき
	if (moveSpeedPhase == MoveSpeedPhase::HighSpeed) {
		//加速入力を続けている場合は、ゲージを減らし続ける
		if (isPushHighSpeedInput) {
			speedChangeGauge -= gaugeDecSpeed;

			//加速するので前に移動させる
			SpeedChangeHighSpeed();
		}
		//加速入力をやめた瞬間に速度変更を終了
		else {
			//速度変更終了
			SetSpeedChangeModeEnd();
		}
	}
	//減速状態のとき
	else if (moveSpeedPhase == MoveSpeedPhase::SlowSpeed) {
		//減速入力を続けている場合は、ゲージを減らし続ける
		if (isPushSlowSpeedInput) {
			speedChangeGauge -= gaugeDecSpeed;

			//減速するので後ろに移動させる
			SpeedChangeSlowSpeed();
		}
		//加速入力をやめた瞬間に速度変更を終了
		else {
			//速度変更終了
			SetSpeedChangeModeEnd();
		}
	}

	//ゲージがなくなったら
	if (!(speedChangeGauge <= 0)) { return; }

	//速度変更終了
	SetSpeedChangeModeEnd();
}

void BasePlayer::SetSpeedChangeModeEnd()
{
	//速度変更を終了
	isSpeedChange = false;

	//飛行機雲の生成を終了する
	if (isVaporCreate) { vaporEffect->VaporEnd(); }

	//移動速度を通常に戻す状態にする
	moveSpeedPhase = MoveSpeedPhase::ReturnNormalSpeed;
}

void BasePlayer::SpeedChangeModeEnd()
{
	//減らしたゲージを増やしていく
	const float gaugeIncSpeed = 0.55f;
	speedChangeGauge += gaugeIncSpeed;

	//ゲージが最大まで溜まっていなければ抜ける
	if (speedChangeGauge < maxSpeedChangeGauge) { return; }

	//速度変更開始可能にする
	isSpeedChangeStartPossible = true;

	//ゲージが溜まった合図に、通常移動状態にしておく
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;
}

void BasePlayer::SpeedChangeHighSpeed()
{
	//加速時に移動する最大限界座標
	const float highSpeedMaxPosZ = 25.0f;

	//速度変更時間
	const int speedChangeTime = 30;
	float timeRatio = ((float)speedChangeTimer / (float)speedChangeTime);
	timeRatio = min(timeRatio, 1);
	//タイマーの値から移動速度に緩急をつける
	float speedRatio = 1 - timeRatio;
	speedRatio = max(speedRatio, 0.2f);

	//前に移動させる
	const float moveSpeed = 0.4f;
	position.z += moveSpeed * speedRatio;

	//最大限界座標を越えないようにする
	position.z = min(position.z, highSpeedMaxPosZ);
}

void BasePlayer::SpeedChangeSlowSpeed()
{
	//減速時に移動する最小限界座標
	const float slowSpeedMinPosZ = 10.0f;

	//速度変更時間
	const int speedChangeTime = 30;
	float timeRatio = ((float)speedChangeTimer / (float)speedChangeTime);
	timeRatio = min(timeRatio, 1);
	//タイマーの値から移動速度に緩急をつける
	float speedRatio = 1 - timeRatio;
	speedRatio = max(speedRatio, 0.15f);

	//後ろに移動させる
	const float moveSpeed = 0.2f;
	position.z -= moveSpeed * speedRatio;

	//最小限界座標を越えないようにする
	position.z = max(position.z, slowSpeedMinPosZ);
}

void BasePlayer::SpeedChangeNormalSpeed()
{
	//通常移動に戻す状態でなければ抜ける
	if (!(moveSpeedPhase == MoveSpeedPhase::NormalSpeed || moveSpeedPhase == MoveSpeedPhase::ReturnNormalSpeed)) { return; }

	//移動させるか判定する差分
	const float distanceBasePos = 0.02f;
	//差分がない状態だったら抜ける
	if (position.z < basePos.z + distanceBasePos && position.z > basePos.z - distanceBasePos) {
		//基準座標にしておく
		position.z = basePos.z;

		//抜ける
		return;
	}

	//元の座標に戻していく
	const float moveSpeed = 0.08f;
	if (position.z >= basePos.z + distanceBasePos) {
		position.z -= moveSpeed;
	}
	else if (position.z <= basePos.z - distanceBasePos) {
		position.z += moveSpeed;
	}
}

void BasePlayer::SpeedChangeNormalBlur()
{
	//通常移動に戻す状態でなければ抜ける
	if (!(moveSpeedPhase == MoveSpeedPhase::NormalSpeed || moveSpeedPhase == MoveSpeedPhase::ReturnNormalSpeed)) { return; }

	//ブラーをかけない状態なら抜ける
	if (!(GamePostEffect::GetPostEffect()->GetIsRadialBlur())) { return; }

	//ブラーの強さ変更させるか判定する差分
	const float distanceBlurStrength = 0.01f;
	//差分がない状態だったら抜ける
	float blurStrength = GamePostEffect::GetPostEffect()->GetRadialBlurStrength();
	if (blurStrength < normalSpeedBlurStrength + distanceBlurStrength && blurStrength > normalSpeedBlurStrength - distanceBlurStrength) {
		//通常移動時のブラーの強さにしておく
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(normalSpeedBlurStrength);

		//抜ける
		return;
	}

	//通常移動時のブラーの強さに戻していく
	const float changeSpeed = 0.01f;
	if (blurStrength >= normalSpeedBlurStrength + distanceBlurStrength) {
		blurStrength -= distanceBlurStrength;
	}
	else if (blurStrength <= normalSpeedBlurStrength - distanceBlurStrength) {
		blurStrength += distanceBlurStrength;
	}
	//変更した強さをセット
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void BasePlayer::Attack()
{
	Input* input = Input::GetInstance();

	//弾発射座標を更新
	UpdateBulletShotPos();

	//攻撃ボタンを押したら
	if (input->PushKey(DIK_SPACE) || input->PushGamePadButton(Input::PAD_B)) {
		//攻撃ボタンを押しているときの処理
		PushAttackButton();
	}
	//攻撃ボタンを離したら
	else if (input->ReleaseKey(DIK_SPACE) || input->ReleaseGamePadButton(Input::PAD_B)) {
		//攻撃ボタンを離したときの処理
		ReleaseAttackButton();
	}
}

void BasePlayer::PushAttackButton()
{
	//ホーミング弾に切り替わる時間
	const int32_t changeModeTime = 50;
	//チャージ時間を加算
	chargeTimer++;

	//レティクルがチャージ状態でないとき && レティクルがロックオン状態でないとき
	if (!reticles->GetIsChargeMode() && !reticles->GetIsLockon()) {
		//タイマーが指定した時間になったら
		if (chargeTimer >= changeModeTime) {
			//レティクルをチャージ状態にする
			reticles->ChargeModeStart();
		}
	}

	//チャージ未完了時
	if (!isChargeShotMode) {
		//タイマーが指定した時間になったら
		if (chargeTimer >= changeModeTime) {
			//チャージショット状態にする
			isChargeShotMode = true;
		}

		//直進弾発射待機処理
		if (isStraightShotWait) {
			straightShotWaitTimer--;
			if (straightShotWaitTimer <= 0) {
				//待機終了
				isStraightShotWait = false;
			}
			return;
		}

		//直進弾発射
		ShotStraightBullet();

		//直進弾発射待ち時間を設定
		const int32_t waitTime = 10;
		//待機開始
		isStraightShotWait = true;
		straightShotWaitTimer = waitTime;
	}
	//チャージ完了時
	else {
		//チャージ演出が最大の大きさになるまでのフレーム数
		const int maxRatioFrame = 5;
		//チャージショット演出用パーティクルの大きさを決める(チャージした時間で大きくなる)
		float sizeRatio = (float)(chargeTimer - changeModeTime) / maxRatioFrame;
		//最大比率1.0を越えない
		const float maxRatio = 1.0f;
		if (sizeRatio >= maxRatio) { sizeRatio = maxRatio; }
		const float particleSize = homingBulletSize * sizeRatio;

		//チャージショット演出用パーティクル生成
		ParticleEmitter::GetInstance()->ChargeShot(bulletShotPos, particleSize);
	}
}

void BasePlayer::ReleaseAttackButton()
{
	//チャージ完了時
	if (isChargeShotMode) {
		//ホーミング弾発射
		ShotHomingBullet();

		//レティクルが敵をロックオンしていなければ、発射と同時にチャージ状態終了
		if (!reticles->GetIsLockon()) {
			reticles->ChargeModeEnd();
		}
	}

	//次に発射ボタンを押した時にすぐ発射できるよう直進弾の発射待機をリセット
	isStraightShotWait = false;
	straightShotWaitTimer = 0;
	//チャージショット状態をリセット
	isChargeShotMode = false;
	chargeTimer = 0;
}

void BasePlayer::UpdateBulletShotPos()
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.3f, 4.0f };

	//弾発射座標を取得
	bulletShotPos = LocalTranslation(distancePos, matWorld);
}

void BasePlayer::ShotStraightBullet()
{
	//弾の速度を設定
	const float bulletSpeed = 6;
	//自機からレティクルへのベクトルに合わせて飛ばす
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//直進弾を生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, bulletShotPos, velocity));
	stageScene->AddPlayerBullet(std::move(newBullet));

	//ショット演出用パーティクル生成
	ParticleEmitter::GetInstance()->Shot(bulletShotPos);
}

void BasePlayer::ShotHomingBullet()
{
	//弾の速度を設定
	const float bulletSpeed = 1.6f;
	//自機からレティクルへのベクトルに合わせて飛ばす
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//ホーミング弾を生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(HomingBullet::Create(bulletShotPos, velocity, homingBulletSize, reticles->GetLockonEnemy()));
	stageScene->AddPlayerBullet(std::move(newBullet));
}
