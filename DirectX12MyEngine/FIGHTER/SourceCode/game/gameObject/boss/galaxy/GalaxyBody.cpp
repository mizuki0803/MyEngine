#include "GalaxyBody.h"
#include "Easing.h"
#include "ParticleEmitter.h"

void (GalaxyBody::* GalaxyBody::appearPhaseFuncTable[])() = {
	&GalaxyBody::AppaerAdvance,
	&GalaxyBody::AppaerWait,
	&GalaxyBody::AppaerRotation,
};

void (GalaxyBody::* GalaxyBody::attackPartChangePhaseFuncTable[])() = {
	&GalaxyBody::AttackPartChangeWait,
	&GalaxyBody::AttackPartChangeRotation,
};


BaseStageScene* GalaxyBody::stageScene = nullptr;
ObjModel* GalaxyBody::bodyModel = nullptr;
const Vector3 GalaxyBody::normalSize = { 25, 25, 25 };
const Vector3 GalaxyBody::damageSize = GalaxyBody::normalSize * 1.05f;
const XMFLOAT4 GalaxyBody::damageColor = { 1, 0.2f, 0.2f, 1 };

GalaxyBody* GalaxyBody::Create(const Vector3& bornPos, const Vector3& basePos)
{
	//ギャラクシー(胴体)のインスタンスを生成
	GalaxyBody* galaxyBody = new GalaxyBody();
	if (galaxyBody == nullptr) {
		return nullptr;
	}

	//寝ている状態のモデルをセット
	assert(bodyModel);
	galaxyBody->model = bodyModel;

	// 初期化
	if (!galaxyBody->Initialize()) {
		delete galaxyBody;
		assert(0);
		return nullptr;
	}

	//生成座標をセット
	galaxyBody->position = bornPos;
	//停止座標をセット
	galaxyBody->basePos = basePos;
	//大きさをセット
	galaxyBody->scale = normalSize;

	return galaxyBody;
}

void GalaxyBody::Update()
{
	//ダメージ状態の処理
	if (isDamage) {
		DamageMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void GalaxyBody::Damage()
{
	//ダメージ状態にする
	isDamage = true;
	//ダメージを喰らった瞬間なのでtrue
	isDamageTrigger = true;
	//ダメージ状態タイマー初期化
	damageTimer = 0;
	//色を変更
	color = damageColor;

	//サイズを少し大きくする
	scale = damageSize;

	//爆発生成する
	DamageExplosion();
}

void GalaxyBody::Appear()
{
	//登場行動
	(this->*appearPhaseFuncTable[static_cast<size_t>(appearPhase)])();
}

void GalaxyBody::AttackPartChangeRotaStart(const float rotSpeed, const float changeRota)
{
	//回転速度をセット
	attackPartChangeRotSpeed = rotSpeed;
	//回転後の角度をセット
	attackPartChangeRota = changeRota;

	//回転が右向きか判定
	if (rotSpeed >= 0) { isAttackPartChangeRotaRight = true; }
	else { isAttackPartChangeRotaRight = false; }

	//攻撃するパーツ変更の行動フェーズを待機にしておく
	attackPartChangePhase = AttackPartChangePhase::Wait;
	//攻撃するパーツ変更をする状態にする
	isAttackPartChangeRota = true;
}

void GalaxyBody::AttackPartChange()
{
	//行動
	(this->*attackPartChangePhaseFuncTable[static_cast<size_t>(attackPartChangePhase)])();
}

void GalaxyBody::Dead()
{
	//上向きに回転させる
	const float rotSpeed = 0.1f;
	rotation.x += rotSpeed;

	//少し下に移動
	position.y -= 0.05f;
}

void GalaxyBody::DamageMode()
{
	//ダメージ状態の時間
	const int damageTime = 20;
	damageTimer++;

	//大きくしたサイズを戻す
	DamageSizeReturn();

	//ダメージ色切り替え
	DamageColorMode();

	//ダメージトリガーフラグがtrueなら下ろしておく
	if (isDamageTrigger) { isDamageTrigger = false; }

	//タイマーが指定した時間になったら
	if (damageTimer >= damageTime) {
		//ダメージ状態を終了
		isDamage = false;

		//色を元に戻しておく
		color = { 1, 1, 1, 1 };
	}
}

void GalaxyBody::DamageSizeReturn()
{
	//大きくしたサイズを元に戻す時間
	const float sizeReturnTime = 10;
	//指定した時間以上なら抜ける
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//大きさを元に戻す
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void GalaxyBody::DamageExplosion()
{
	//敵内部に演出が出てしまうことがあるので、敵の大きさ分押し戻す
	Vector3 pos = position;
	pos.z -= scale.z / 2;
	//ランダムでずらす
	const Vector3 randPos = { 15, 15, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//爆発演出用パーティクル生成
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void GalaxyBody::DamageColorMode()
{
	//ダメージ色切り替え時間
	const int damageColorChangeTime = 2;

	//タイマーが指定した時間になったら
	if (damageTimer % damageColorChangeTime == 0) {
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
}

void GalaxyBody::AppaerAdvance()
{
	//基準の位置の前進してくる
	const float advanceSpeed = 0.8f;
	position.z -= advanceSpeed;

	//停止座標まで進んでいなければ抜ける
	if (position.z > basePos.z) { return; }
	
	//次のフェーズへ
	appearPhase = AppearPhase::Wait;
}

void GalaxyBody::AppaerWait()
{
	//待機する時間
	const int waitTime = 50;
	//タイマー更新
	actionTimer++;

	//タイマーが指定した時間以下なら抜ける
	if (actionTimer < waitTime) { return; }

	//タイマーを初期化しておく
	actionTimer = 0;
	//次のフェーズへ
	appearPhase = AppearPhase::Rotation;
}

void GalaxyBody::AppaerRotation()
{
	//右向きにする
	const float rotSpeed = 0.5f;
	rotation.y += rotSpeed;

	//右を完全に向いていなければ抜ける
	if (rotation.y <= 90) { return; }

	//登場行動終了
	isAppear = false;
}

void GalaxyBody::AttackPartChangeWait()
{
	//待機する時間
	const int waitTime = 100;
	//タイマー更新
	actionTimer++;

	//タイマーが指定した時間以下なら抜ける
	if (actionTimer < waitTime) { return; }

	//タイマーを初期化しておく
	actionTimer = 0;
	//次のフェーズへ
	attackPartChangePhase = AttackPartChangePhase::Rotation;
}

void GalaxyBody::AttackPartChangeRotation()
{
	//回転速度を加算
	rotation.y += attackPartChangeRotSpeed;

	//回転後の角度まで行っていなければ抜ける
	if (isAttackPartChangeRotaRight && rotation.y <= attackPartChangeRota) { return; }		//右回転
	else if (!isAttackPartChangeRotaRight && rotation.y >= attackPartChangeRota) { return; }//左回転

	//攻撃するパーツ変更をする回転を終了
	isAttackPartChangeRota = false;
}

void GalaxyBody::Sway()
{
	//ゆらゆら最大の速さ
	const float rotXMaxSpeed = 0.02f;
	//ゆらゆら折り返し
	const float swayLimitBasePosDistance = 10;
	//ゆらゆらの加速度
	const float swayXSpeedAccel = 0.001f;
	//上移動
	if (isSwayUp) {
		//速度が最大でないとき
		if (!isSwaySpeedMax) {
			//速度に加速度を加算する
			swayVel.y += swayXSpeedAccel;

			//速さが最大になったらフラグを立てる
			if (swayVel.y >= rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//ゆらゆらの座標が折り返しまで来たら
		if (position.y >= basePos.y + swayLimitBasePosDistance) {
			//速度に加速度を減算していく
			swayVel.y -= swayXSpeedAccel;

			//速度が0になったら
			if (swayVel.y <= 0) {
				//下移動に変更
				isSwayUp = false;
				//速度最大フラグを下ろしておく
				isSwaySpeedMax = false;
			}
		}
	}
	//下移動
	else {
		//速度が最大でないとき
		if (!isSwaySpeedMax) {
			//速度に加速度を減算する
			swayVel.y -= swayXSpeedAccel;

			//速度が最大になったらフラグを立てる
			if (swayVel.y <= -rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//ゆらゆらの速度が折り返しまで来たら
		if (position.y <= basePos.y - swayLimitBasePosDistance) {
			//速度に加速度を加算していく
			swayVel.y += swayXSpeedAccel;

			//速度が0になったら
			if (swayVel.y >= 0) {
				//上移動に変更
				isSwayUp = true;
				//速度最大フラグを下ろしておく
				isSwaySpeedMax = false;
			}
		}
	}

	//Y座標に速度を加算してゆらゆらさせる
	position += swayVel;
}
