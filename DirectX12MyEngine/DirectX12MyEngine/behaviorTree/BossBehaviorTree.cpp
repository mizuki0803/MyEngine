#include "BossBehaviorTree.h"
#include "Boss.h"
#include "DebugText.h"

BossBehaviorTree* BossBehaviorTree::Create(Boss* boss)
{
	//ビヘイビアツリーのインスタンスを生成
	BossBehaviorTree* bossBehaviorTree = new BossBehaviorTree();
	if (bossBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// 初期化
	if (!bossBehaviorTree->Initialize(boss)) {
		delete bossBehaviorTree;
		assert(0);
		return nullptr;
	}

	return bossBehaviorTree;
}

bool BossBehaviorTree::Initialize(Boss* boss)
{
	//ボスをセット
	this->boss = boss;

	//Selector,Sequencerの生成
	topSelector.reset(Selector::Create());
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

void BossBehaviorTree::Root()
{
	//関数自体をルートノードに見立て、ルートノード直下のノードを実行する
	topSelector->Select();
}

void BossBehaviorTree::MakeTree()
{
	//ルートノード

	//降下状態
	std::function<bool()> fallMode =
		std::bind(&Boss::FallMode, boss);
	topSelector->AddNode(fallMode);


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

void BossBehaviorTree::AttackModeSequenceNode()
{
	//攻撃状態シーケンスノード

	//時間経過
	std::function<bool()> attackModeCount =
		std::bind(&Boss::AttackModeCount, boss);
	attackModeSequencer->AddNode(attackModeCount);


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

void BossBehaviorTree::AttackTypeSelectNode()
{
	//攻撃内容セレクトノード

	//攻撃内容設定を開始する
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Boss::AttackTypeSelectStart, boss);
	attackTypeSelector->AddNode(attackTypeSelectStart);


	//攻撃内容:追従を設定する
	std::function<bool()> attackTypeTrackingSelect =
		std::bind(&Boss::AttackTypeTrackingSelect, boss);
	attackTypeSelector->AddNode(attackTypeTrackingSelect);


	//攻撃内容:超巨大弾を設定する
	std::function<bool()> attackTypeSuperGiantBulletSelect =
		std::bind(&Boss::AttackTypeSuperGiantBulletSelect, boss);
	attackTypeSelector->AddNode(attackTypeSuperGiantBulletSelect);


	//攻撃内容:回転を設定する
	std::function<bool()> attackTypeRotateSelect =
		std::bind(&Boss::AttackTypeRotateSelect, boss);
	attackTypeSelector->AddNode(attackTypeRotateSelect);


	//攻撃内容:分身体当たりを設定する
	std::function<bool()> attackTypeAvatarBodyBlowSelect =
		std::bind(&Boss::AttackTypeAvatarBodyBlowSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarBodyBlowSelect);


	//攻撃内容:分身ガトリング砲を設定する
	std::function<bool()> attackTypeAvatarGatlingSelect =
		std::bind(&Boss::AttackTypeAvatarGatlingSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarGatlingSelect);


	//攻撃内容:分身巨大弾を設定する
	std::function<bool()> attackTypeAvatarGiantBulletSelect =
		std::bind(&Boss::AttackTypeAvatarGiantBulletSelect, boss);
	attackTypeSelector->AddNode(attackTypeAvatarGiantBulletSelect);
}

void BossBehaviorTree::AttackModeActionSelectNode()
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

void BossBehaviorTree::AttackModeRotaSelectNode()
{
	//攻撃状態動き・回転対象セレクトノード

	//本体を回転
	std::function<bool()> attackModeMainBodyRota =
		std::bind(&Boss::AttackModeMainBodyRota, boss);
	attackModeRotaSelector->AddNode(attackModeMainBodyRota);


	//分身を回転
	std::function<bool()> attackModeAvatarRota =
		std::bind(&Boss::AttackModeAvatarRota, boss);
	attackModeRotaSelector->AddNode(attackModeAvatarRota);
}

void BossBehaviorTree::AttackSelectNode()
{
	//攻撃状態動き・攻撃セレクトノード

	//攻撃内容;追従を実行する
	std::function<bool()> attackTypeTracking =
		std::bind(&Boss::AttackTypeTracking, boss);
	attackSelector->AddNode(attackTypeTracking);


	//攻撃内容;超巨大弾を実行する
	std::function<bool()> attackTypeSuperGiantBullet =
		std::bind(&Boss::AttackTypeSuperGiantBullet, boss);
	attackSelector->AddNode(attackTypeSuperGiantBullet);


	//攻撃内容;回転を実行する
	std::function<bool()> attackTypeRotate =
		std::bind(&Boss::AttackTypeRotate, boss);
	attackSelector->AddNode(attackTypeRotate);


	//攻撃内容;分身体当たりを実行する
	std::function<bool()> attackTypeAvatarBodyBlow =
		std::bind(&Boss::AttackTypeAvatarBodyBlow, boss);
	attackSelector->AddNode(attackTypeAvatarBodyBlow);


	//攻撃内容;分身ガトリング砲を実行する
	std::function<bool()> attackTypeAvatarGatling =
		std::bind(&Boss::AttackTypeAvatarGatling, boss);
	attackSelector->AddNode(attackTypeAvatarGatling);


	//攻撃内容;分身巨大弾を実行する
	std::function<bool()> attackTypeAvatarGiantBullet =
		std::bind(&Boss::AttackTypeAvatarGiantBullet, boss);
	attackSelector->AddNode(attackTypeAvatarGiantBullet);
}

void BossBehaviorTree::WaitModeSequenceNode()
{
	//待機状態シーケンスノード

	//時間経過
	std::function<bool()> waitModeCount =
		std::bind(&Boss::WaitModeCount, boss);
	waitModeSequencer->AddNode(waitModeCount);


	//待機状態動きセレクター
	std::function<bool()> waitModeActionSelect =
		std::bind(&Selector::Select, waitModeActionSelector.get());
	waitModeSequencer->AddNode(waitModeActionSelect);
	//待機状態動きセレクトノード
	WaitModeActionSelectNode();
}

void BossBehaviorTree::WaitModeActionSelectNode()
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
		std::bind(&Boss::ReturnBasePosition, boss);
	waitModeActionSelector->AddNode(returnBasePosition);
}

void BossBehaviorTree::WaitModeRotaSelectNode()
{
	//待機状態動き・回転対象セレクトノード

	//本体を回転
	std::function<bool()> waitModeMainBodyRota =
		std::bind(&Boss::WaitModeMainBodyRota, boss);
	waitModeRotaSelector->AddNode(waitModeMainBodyRota);


	//分身を回転
	std::function<bool()> waitModeAvatarRota =
		std::bind(&Boss::WaitModeAvatarRota, boss);
	waitModeRotaSelector->AddNode(waitModeAvatarRota);
}

void BossBehaviorTree::DeadModeSelectNode()
{
	//死亡状態セレクトノード

	//死亡爆発
	std::function<bool()> deadExplosion =
		std::bind(&Boss::DeadExplosion, boss);
	deadModeSelector->AddNode(deadExplosion);


	//死亡落下
	std::function<bool()> deadFall =
		std::bind(&Boss::DeadFall, boss);
	deadModeSelector->AddNode(deadFall);
}
