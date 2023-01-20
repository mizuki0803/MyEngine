#include "Galaxy.h"
#include "BasePlayer.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

BasePlayer* Galaxy::player = nullptr;
const float Galaxy::waitModeTime = 180.0f;

Galaxy* Galaxy::Create(const Vector3& bornPos, const Vector3& basePos)
{
	//ギャラクシーのインスタンスを生成
	Galaxy* galaxy = new Galaxy();
	if (galaxy == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!galaxy->Initialize(bornPos, basePos)) {
		delete galaxy;
		assert(0);
		return nullptr;
	}

	return galaxy;
}

bool Galaxy::Initialize(const Vector3& bornPos, const Vector3& basePos)
{
	//生成座標をセット
	this->bornPos = bornPos;
	//基準座標をセット
	this->basePos = basePos;

	//胴体生成
	body.reset(GalaxyBody::Create(bornPos, basePos));
	//プロペラ生成
	std::vector<Vector3> propellerPos = {
		{0, 2, -0.95f},
		{0.4f, 1.45f, 1.05f},
		{-0.4f, 1.45f, 1.05f},
	};
	std::vector<float> propellerSize = {
		2, 1.6f, 1.6f
	};
	float propellerRota = 0;
	const float propellerRotSpped = 5;
	for (int i = 0; i < propellerPos.size(); i++) {
		std::unique_ptr<GalaxyPropeller> newPropeller;
		newPropeller.reset(GalaxyPropeller::Create(body.get(), propellerPos[i],
			{ 0, propellerRota, 0 }, propellerRotSpped, propellerSize[i]));
		propellers.push_back(std::move(newPropeller));

		//次のプロペラを回転させる
		const float rotNum = 120;
		propellerRota += rotNum;
	}
	//船首生成
	bow.reset(GalaxyBow::Create(body.get(), { 0, 0.05f, -2.75f }));
	//大砲生成
	std::vector<Vector3> cannonPos = {
		{0.9f, 0, -1.5f},
		{0.9f, 0, -1},
		{0.9f, 0, -0.5f},
		{0.9f, 0, 0},
		{0.9f, 0, 0.5f},
		{0.9f, 0, 1},
		{0.9f, 0, 1.5f},
	};
	for (int i = 0; i < cannonPos.size(); i++) {
		std::unique_ptr<GalaxyCannon> newCannon;
		newCannon.reset(GalaxyCannon::Create(body.get(), cannonPos[i]));
		cannons.push_back(std::move(newCannon));
	}

	//HPセット
	int maxHP = GalaxyBow::GetMaxHP() + GalaxyCannon::GetMaxHP() * (int)cannons.size();
	HP = maxHP;

	//ビヘイビアツリー生成
	behaviorTree.reset(GalaxyBehaviorTree::Create(this));

	return true;
}

void Galaxy::Update()
{
	//登場,死亡状態以外なら胴体をゆらゆらさせる
	if (!(phase == Phase::Appear || phase == Phase::Dead)) {
		body->Sway();
	}

	//更新
	body->Update();//胴体
	for (const std::unique_ptr<GalaxyPropeller>& propeller : propellers) {
		propeller->Update();//プロペラ
	}
	bow->Update();//船首
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		cannon->Update();//大砲
	}

	//ビヘイビアツリーによる行動遷移
	behaviorTree->Root();

	//HPUI更新
	if (hpUI) {
		hpUI->Update();
	}
	//ボス名表示UI更新
	if (bossNameUI) {
		bossNameUI->Update();
	}
}

void Galaxy::Draw()
{
	//描画
	body->Draw();//胴体
	for (const std::unique_ptr<GalaxyPropeller>& propeller : propellers) {
		propeller->Draw();//プロペラ
	}
	bow->Draw();//船首
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		cannon->Draw();//大砲
	}
}

void Galaxy::DrawUI()
{
	//HPUI描画
	if (hpUI) {
		hpUI->Draw();
	}
	//ボス名表示UI描画
	if (bossNameUI) {
		bossNameUI->Draw();
	}
}

void Galaxy::OnCollisionBow(const int damageNum, const Vector3& collisionPos)
{
	//船首が攻撃するパーツでなければ抜ける
	if (!(attackPartPhase == AttackPartPhase::Front)) { return; }

	//船首にダメージ
	bow->Damage(damageNum, collisionPos);
	//胴体にも演出を入れる
	body->Damage();

	//ギャラクシーにダメージ(実際に喰らったダメージ量をセット)
	const int mainBodyDamageNum = bow->GetDamageNum();
	Damage(mainBodyDamageNum);
}

void Galaxy::OnCollisionCannon(GalaxyCannon* cannon, const int damageNum, const Vector3& collisionPos)
{
	//大砲が攻撃するパーツでなければ抜ける
	if (!(attackPartPhase == AttackPartPhase::Cannon)) { return; }

	//大砲が既に死亡していたら抜ける
	if (cannon->GetIsDead()) { return; }

	//衝突した大砲にダメージ
	cannon->Damage(damageNum, collisionPos);

	//大砲が全滅したかチェック
	CheckAllCannonDead();

	//ギャラクシー全体にダメージ(大砲が実際に喰らったダメージ量をセット)
	const int avatarBodyDamageNum = cannon->GetDamageNum();
	Damage(avatarBodyDamageNum);
}

bool Galaxy::AppearMode()
{
	//登場状態でなければ抜ける
	if (!(phase == Phase::Appear)) { return false; }

	//胴体を登場させる
	body->Appear();

	//ボスの登場が完了したら
	if (!body->GetIsAppear()) {
		//攻撃するパーツを大砲に設定
		attackPartPhase = AttackPartPhase::Cannon;

		//次のフェーズへ
		phase = Phase::Attack;
		//攻撃開始用、変数などを初期化
		AttackModeStart();

		//ボス名表示UIはもう使用しないので解放しておく
		bossNameUI.reset();
	}

	return true;
}

bool Galaxy::AppearUICreate()
{
	//登場状態でなければ抜ける
	if (!(phase == Phase::Appear)) { return false; }

	//ボス座標が指定したラインよりこちら側になったらボス名表示UI生成
	const float bossNameUICreateBornPosDistance = 100;
	const float bossNameUICreatePos = bornPos.z - bossNameUICreateBornPosDistance;
	if (body->GetPosition().z <= bossNameUICreatePos && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(1));
	}

	//回転が指定したラインまで進んだらHPバー生成
	const float bossNameUICreateRota = 45;
	if (body->GetRotation().y >= bossNameUICreateRota && !hpUI) {
		const Vector2 hpUIPosition = { 30, 170 };
		hpUI.reset(BossHPUI::Create(hpUIPosition, HP));
	}

	return true;
}

bool Galaxy::AttackMode()
{
	//攻撃状態でなければ抜ける
	if (!(phase == Phase::Attack)) { return false; }

	//攻撃するパーツが大砲のときは大砲の攻撃が終了したか判定
	if (attackPartPhase == AttackPartPhase::Cannon) {
		for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
			//大砲が既に死亡していたら飛ばす
			if (cannon->GetIsDead()) { continue; }
			//攻撃中なら抜ける
			if (cannon->GetIsAttack()) { return true; }
		}
		//攻撃が終了したので待機状態にする
		phase = Phase::Wait;
	}
	//攻撃するパーツが船首のときは船首の攻撃が終了したか判定
	else if (attackPartPhase == AttackPartPhase::Front) {
		//攻撃中なら抜ける
		if (bow->GetIsAttack()) { return true; }

		//攻撃が終了したので待機状態にする
		phase = Phase::Wait;
	}

	return true;
}

bool Galaxy::UpdateBulletShotPos()
{
	//攻撃状態でなければ抜ける
	if (!(phase == Phase::Attack)) { return false; }

	//攻撃するパーツが大砲のときは大砲の発射座標を更新
	if (attackPartPhase == AttackPartPhase::Cannon) {
		for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
			//大砲が既に死亡していたら飛ばす
			if (cannon->GetIsDead()) { continue; }

			cannon->UpdateBulletShotPos();
		}
	}
	//攻撃するパーツが船首のときは船首の発射座標を更新
	else if (attackPartPhase == AttackPartPhase::Front) {
		bow->UpdateBulletShotPos();
	}

	return true;
}

bool Galaxy::AttackTypeSelectStart()
{
	//攻撃内容が既に決まっていたらtrueを返す
	if (!(attackType == AttackType::None)) { return true; }

	//攻撃内容設定を開始するためfalseを返す
	return false;
}

bool Galaxy::AttackTypeRapidFireCannonSelect()
{
	//大砲が攻撃するパーツでなければ抜ける
	if (!(attackPartPhase == AttackPartPhase::Cannon)) { return false; }

	//大砲に攻撃内容:速射の開始時待機フレーム数をセット
	const std::vector<int> rapidFireCannonStartWaitTime = {
		60, 80, 100, 120, 100, 80, 60
	};
	int i = 0;
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		cannon->SetRapidFireStartWaitTime(rapidFireCannonStartWaitTime[i]);
		i++;
	}

	//攻撃内容:速射(大砲)をセット
	attackType = AttackType::RapidFireCannon;

	return true;
}

bool Galaxy::AttackTypeFlamethrowerBowSelect()
{
	//船首が攻撃するパーツでなければ抜ける
	if (!(attackPartPhase == AttackPartPhase::Front)) { return false; }

	//攻撃内容:速射(大砲)をセット
	attackType = AttackType::FlamethrowerBow;

	return true;
}

bool Galaxy::AttackTypeRapidFireCannon()
{
	//攻撃内容が速射(大砲)でなければ抜ける
	if (!(attackType == AttackType::RapidFireCannon)) { return false; }

	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		//大砲が既に死亡していたら飛ばす
		if (cannon->GetIsDead()) { continue; }

		cannon->AttackTypeRapidFire();
	}

	return true;
}

bool Galaxy::AttackTypeFlamethrowerBow()
{
	//攻撃内容が火炎放射(船首)でなければ抜ける
	if (!(attackType == AttackType::FlamethrowerBow)) { return false; }

	//弾発射の標的座標をセット
	const Vector3 targetPos = player->GetWorldPos();
	bow->AttackTypeFlamethrower(targetPos);

	return true;
}

bool Galaxy::WaitMode()
{
	//待機状態でなければ抜ける
	if (!(phase == Phase::Wait)) { return false; }

	//タイマーを更新
	waitModeTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (waitModeTimer >= waitModeTime) {
		phase = Phase::Attack;
		//攻撃開始用、変数などを初期化
		AttackModeStart();

		//次に待機状態になったときのためにタイマーを初期化しておく
		waitModeTimer = 0;
	}

	return true;
}

bool Galaxy::AttackPartChangeMode()
{
	//攻撃するパーツ変更状態でなければ抜ける
	if (!(phase == Phase::AttackPartChange)) { return false; }

	//胴体の攻撃するパーツ変更行動
	body->AttackPartChange();

	//胴体を回転させる行動中なら抜ける
	if (body->GetIsAttackPartChangeRota()) { return true; }

	//攻撃するパーツが大砲の場合、攻撃するパーツを船首に変更
	if (attackPartPhase == AttackPartPhase::Cannon) { attackPartPhase = AttackPartPhase::Front; }

	//攻撃するパーツの変更が完了したので、攻撃状態にする
	phase = Phase::Attack;
	//攻撃開始用、変数などを初期化
	AttackModeStart();

	return true;
}

bool Galaxy::DeadExplosion()
{
	//死亡状態でなければ抜ける
	if (!(phase == Phase::Dead)) { return false; }

	//死亡状態の時間
	const int deadModeTime = 560;
	//タイマー更新
	deadModeTimer++;

	//胴体の死亡行動
	body->Dead();

	//一定間隔で爆発
	const int explosionInterval = 10;
	if ((deadModeTimer % (int)explosionInterval) == 0) {
		//爆発演出用パーティクル生成
		Vector3 particlePos = body->GetWorldPos();
		//演出の大きさ(削除が近いと大きくする)
		float size = 4.0f;
		//削除が近くないときは小爆発を散らばせる
		if (deadModeTimer < deadModeTime - 30) {
			const float distance = 80.0f;
			particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
			particlePos.y += (float)rand() / RAND_MAX * distance - distance / 4.0f;
			particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		}
		//削除が近いときは大爆発を散らばせる
		else { 
			size = 40;
		}

		ParticleEmitter::GetInstance()->Explosion(particlePos, size);
	}

	//タイマーが指定した時間になったら削除
	if (deadModeTimer >= deadModeTime) {
		isDelete = true;
	}

	return true;
}

void Galaxy::Damage(const int damageNum)
{
	//引数の数字の分ダメージを喰らう
	HP -= damageNum;

	//HPが0以下になったら死亡状態にする
	if (HP <= 0) {
		HP = 0;

		phase = Phase::Dead;
		isDead = true;

		//プロペラの回転を止める
		for (const std::unique_ptr<GalaxyPropeller>& propeller : propellers) {
			propeller->SetIsRotation(false);
		}
	}

	//ダメージを喰らったのでHPバーの長さを変更する
	if (hpUI) {
		hpUI->Damage(HP);
	}
}

void Galaxy::AttackModeStart()
{
	//攻撃するパーツが大砲のときは大砲の攻撃状態を開始させる
	if (attackPartPhase == AttackPartPhase::Cannon) {
		for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
			//大砲が既に死亡していたら飛ばす
			if (cannon->GetIsDead()) { continue; }

			cannon->AttackModeStart();
		}
	}
	//攻撃するパーツが船首のときは船首の攻撃状態を開始させる
	else if (attackPartPhase == AttackPartPhase::Front) {
		bow->AttackModeStart();
	}

	//攻撃内容を未設定にしておく
	attackType = AttackType::None;
}

void Galaxy::CheckAllCannonDead()
{
	//一体でも生きていたら抜ける
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		if (!cannon->GetIsDead()) {
			return;
		}
	}

	//攻撃するパーツを変更する回転を開始
	const float rotSpeed = -0.5f;
	const float changeRota = 0;
	body->AttackPartChangeRotaStart(rotSpeed, changeRota);

	//攻撃するパーツを変更する状態にする
	phase = Phase::AttackPartChange;

	//船首を起きている状態のモデルに変更
	bow->ChangeModel();
}
