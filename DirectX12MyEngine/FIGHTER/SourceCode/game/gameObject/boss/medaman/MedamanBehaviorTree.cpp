#include "MedamanBehaviorTree.h"
#include "Medaman.h"
#include "DebugText.h"

MedamanBehaviorTree* MedamanBehaviorTree::Create(Medaman* medaman)
{
	//ビヘイビアツリーのインスタンスを生成
	MedamanBehaviorTree* medamanBehaviorTree = new MedamanBehaviorTree();
	if (medamanBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// 初期化
	if (!medamanBehaviorTree->Initialize(medaman)) {
		delete medamanBehaviorTree;
		assert(0);
		return nullptr;
	}

	return medamanBehaviorTree;
}

bool MedamanBehaviorTree::Initialize(Medaman* medaman)
{
	//メダマーンをセット
	this->medaman = medaman;

	//Selector,Sequencerの生成
	topSelector.reset(Selector::Create());
	appearModeSequencer.reset(Sequencer::Create());
	attackModeSequencer.reset(Sequencer::Create());
	waitModeSequencer.reset(Sequencer::Create());
	deadModeSelector.reset(Selector::Create());
	attackTypeSelector.reset(Selector::Create());
	attackModeActionSelector.reset(Selector::Create());
	attackModeRotaSelector.reset(Selector::Create());
	attackSelector.reset(Selector::Create());
	waitModeActionSelector.reset(Selector::Create());
	waitModeRotaSelector.reset(Selector::Create());

	//木構造を作成
	MakeTree();

	return true;
}

void MedamanBehaviorTree::Root()
{
	//関数自体をルートノードに見立て、ルートノード直下のノードを実行する
	topSelector->Select();
}

void MedamanBehaviorTree::MakeTree()
{
	//ルートノード

	//登場状態
	std::function<bool()> appearModeSequence =
		std::bind(&Sequencer::Sequence, appearModeSequencer.get());
	topSelector->AddNode(appearModeSequence);
	//登場状態シーケンスノード
	AppearModeSequenceNode();


	//攻撃状態シーケンサー
	std::function<bool()> attackModeSequence =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackModeSequence);
	//攻撃状態シーケンスノード
	AttackModeSequenceNode();


	//待機状態シーケンサー
	std::function<bool()> waitModeSequence =
		std::bind(&Sequencer::Sequence, waitModeSequencer.get());
	topSelector->AddNode(waitModeSequence);
	//待機状態シーケンスノード
	WaitModeSequenceNode();


	//死亡状態セレクター
	std::function<bool()> deadModeSelect =
		std::bind(&Selector::Select, deadModeSelector.get());
	topSelector->AddNode(deadModeSelect);
	//死亡状態セレクトノード
	DeadModeSelectNode();
}

void MedamanBehaviorTree::AppearModeSequenceNode()
{
	//登場状態シーケンスノード

	//時間経過
	std::function<bool()> appearModeCount =
		std::bind(&Medaman::AppearModeCount, medaman);
	appearModeSequencer->AddNode(appearModeCount);

	//登場降下
	std::function<bool()> appearFall =
		std::bind(&Medaman::AppearFall, medaman);
	appearModeSequencer->AddNode(appearFall);
}

void MedamanBehaviorTree::AttackModeSequenceNode()
{
	//攻撃状態シーケンスノード

	//時間経過
	std::function<bool()> attackModeCount =
		std::bind(&Medaman::AttackModeCount, medaman);
	attackModeSequencer->AddNode(attackModeCount);

	//発射座標更新
	std::function<bool()> updateBulletShotPos =
		std::bind(&Medaman::UpdateBulletShotPos, medaman);
	attackModeSequencer->AddNode(updateBulletShotPos);


	//攻撃内容セレクター
	std::function<bool()> attackTypeSelect =
		std::bind(&Selector::Select, attackTypeSelector.get());
	attackModeSequencer->AddNode(attackTypeSelect);
	//攻撃内容セレクトノード
	AttackTypeSelectNode();


	//攻撃状態動きセレクター
	std::function<bool()> attackModeActionSelect =
		std::bind(&Selector::Select, attackModeActionSelector.get());
	attackModeSequencer->AddNode(attackModeActionSelect);
	//攻撃状態動きセレクトノード
	AttackModeActionSelectNode();
}

void MedamanBehaviorTree::AttackTypeSelectNode()
{
	//攻撃内容セレクトノード

	//攻撃内容設定を開始する
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Medaman::AttackTypeSelectStart, medaman);
	attackTypeSelector->AddNode(attackTypeSelectStart);


	//攻撃内容:追従を設定する
	std::function<bool()> attackTypeTrackingSelect =
		std::bind(&Medaman::AttackTypeTrackingSelect, medaman);
	attackTypeSelector->AddNode(attackTypeTrackingSelect);


	//攻撃内容:超巨大弾を設定する
	std::function<bool()> attackTypeSuperGiantBulletSelect =
		std::bind(&Medaman::AttackTypeSuperGiantBulletSelect, medaman);
	attackTypeSelector->AddNode(attackTypeSuperGiantBulletSelect);


	//攻撃内容:回転を設定する
	std::function<bool()> attackTypeRotateSelect =
		std::bind(&Medaman::AttackTypeRotateSelect, medaman);
	attackTypeSelector->AddNode(attackTypeRotateSelect);


	//攻撃内容:分身体当たりを設定する
	std::function<bool()> attackTypeAvatarBodyBlowSelect =
		std::bind(&Medaman::AttackTypeAvatarBodyBlowSelect, medaman);
	attackTypeSelector->AddNode(attackTypeAvatarBodyBlowSelect);


	//攻撃内容:分身ガトリング砲を設定する
	std::function<bool()> attackTypeAvatarGatlingSelect =
		std::bind(&Medaman::AttackTypeAvatarGatlingSelect, medaman);
	attackTypeSelector->AddNode(attackTypeAvatarGatlingSelect);


	//攻撃内容:分身巨大弾を設定する
	std::function<bool()> attackTypeAvatarGiantBulletSelect =
		std::bind(&Medaman::AttackTypeAvatarGiantBulletSelect, medaman);
	attackTypeSelector->AddNode(attackTypeAvatarGiantBulletSelect);
}

void MedamanBehaviorTree::AttackModeActionSelectNode()
{
	//攻撃状態動きセレクトノード

	//攻撃状態動き・回転対象セレクター
	std::function<bool()> attackModeRotaSelect =
		std::bind(&Selector::Select, attackModeRotaSelector.get());
	attackModeActionSelector->AddNode(attackModeRotaSelect);
	//攻撃状態動き・回転対象セレクトノード
	AttackModeRotaSelectNode();


	//攻撃状態動き・攻撃セレクター
	std::function<bool()> attackSelect =
		std::bind(&Selector::Select, attackSelector.get());
	attackModeActionSelector->AddNode(attackSelect);
	//攻撃状態動き・攻撃セレクトノード
	AttackSelectNode();
}

void MedamanBehaviorTree::AttackModeRotaSelectNode()
{
	//攻撃状態動き・回転対象セレクトノード

	//本体を回転
	std::function<bool()> attackModeMainBodyRota =
		std::bind(&Medaman::AttackModeMainBodyRota, medaman);
	attackModeRotaSelector->AddNode(attackModeMainBodyRota);


	//分身を回転
	std::function<bool()> attackModeAvatarRota =
		std::bind(&Medaman::AttackModeAvatarRota, medaman);
	attackModeRotaSelector->AddNode(attackModeAvatarRota);
}

void MedamanBehaviorTree::AttackSelectNode()
{
	//攻撃状態動き・攻撃セレクトノード

	//攻撃内容;追従を実行する
	std::function<bool()> attackTypeTracking =
		std::bind(&Medaman::AttackTypeTracking, medaman);
	attackSelector->AddNode(attackTypeTracking);


	//攻撃内容;超巨大弾を実行する
	std::function<bool()> attackTypeSuperGiantBullet =
		std::bind(&Medaman::AttackTypeSuperGiantBullet, medaman);
	attackSelector->AddNode(attackTypeSuperGiantBullet);


	//攻撃内容;回転を実行する
	std::function<bool()> attackTypeRotate =
		std::bind(&Medaman::AttackTypeRotate, medaman);
	attackSelector->AddNode(attackTypeRotate);


	//攻撃内容;分身体当たりを実行する
	std::function<bool()> attackTypeAvatarBodyBlow =
		std::bind(&Medaman::AttackTypeAvatarBodyBlow, medaman);
	attackSelector->AddNode(attackTypeAvatarBodyBlow);


	//攻撃内容;分身ガトリング砲を実行する
	std::function<bool()> attackTypeAvatarGatling =
		std::bind(&Medaman::AttackTypeAvatarGatling, medaman);
	attackSelector->AddNode(attackTypeAvatarGatling);


	//攻撃内容;分身巨大弾を実行する
	std::function<bool()> attackTypeAvatarGiantBullet =
		std::bind(&Medaman::AttackTypeAvatarGiantBullet, medaman);
	attackSelector->AddNode(attackTypeAvatarGiantBullet);
}

void MedamanBehaviorTree::WaitModeSequenceNode()
{
	//待機状態シーケンスノード

	//時間経過
	std::function<bool()> waitModeCount =
		std::bind(&Medaman::WaitModeCount, medaman);
	waitModeSequencer->AddNode(waitModeCount);


	//待機状態動きセレクター
	std::function<bool()> waitModeActionSelect =
		std::bind(&Selector::Select, waitModeActionSelector.get());
	waitModeSequencer->AddNode(waitModeActionSelect);
	//待機状態動きセレクトノード
	WaitModeActionSelectNode();
}

void MedamanBehaviorTree::WaitModeActionSelectNode()
{
	//待機状態動きセレクトノード

	//待機状態動き・回転対象セレクター
	std::function<bool()> waitModeRotaSelect =
		std::bind(&Selector::Select, waitModeRotaSelector.get());
	waitModeActionSelector->AddNode(waitModeRotaSelect);
	//待機状態動き・回転対象セレクトノード
	WaitModeRotaSelectNode();


	//基準位置に戻る
	std::function<bool()> returnBasePosition =
		std::bind(&Medaman::ReturnBasePosition, medaman);
	waitModeActionSelector->AddNode(returnBasePosition);
}

void MedamanBehaviorTree::WaitModeRotaSelectNode()
{
	//待機状態動き・回転対象セレクトノード

	//本体を回転
	std::function<bool()> waitModeMainBodyRota =
		std::bind(&Medaman::WaitModeMainBodyRota, medaman);
	waitModeRotaSelector->AddNode(waitModeMainBodyRota);


	//分身を回転
	std::function<bool()> waitModeAvatarRota =
		std::bind(&Medaman::WaitModeAvatarRota, medaman);
	waitModeRotaSelector->AddNode(waitModeAvatarRota);
}

void MedamanBehaviorTree::DeadModeSelectNode()
{
	//死亡状態セレクトノード

	//死亡爆発
	std::function<bool()> deadExplosion =
		std::bind(&Medaman::DeadExplosion, medaman);
	deadModeSelector->AddNode(deadExplosion);


	//死亡落下
	std::function<bool()> deadFall =
		std::bind(&Medaman::DeadFall, medaman);
	deadModeSelector->AddNode(deadFall);
}
