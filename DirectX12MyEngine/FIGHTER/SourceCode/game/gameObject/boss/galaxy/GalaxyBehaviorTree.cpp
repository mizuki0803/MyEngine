#include "GalaxyBehaviorTree.h"
#include "Galaxy.h"

GalaxyBehaviorTree* GalaxyBehaviorTree::Create(Galaxy* galaxy)
{
	//ビヘイビアツリーのインスタンスを生成
	GalaxyBehaviorTree* galaxyBehaviorTree = new GalaxyBehaviorTree();
	if (galaxyBehaviorTree == nullptr) {
		assert(0);
		return nullptr;
	}

	// 初期化
	if (!galaxyBehaviorTree->Initialize(galaxy)) {
		delete galaxyBehaviorTree;
		assert(0);
		return nullptr;
	}

	return galaxyBehaviorTree;
}

bool GalaxyBehaviorTree::Initialize(Galaxy* galaxy)
{
	//ギャラクシーをセット
	this->galaxy = galaxy;

	//Selector,Sequencerの生成
	topSelector.reset(Selector::Create());
	appearModeSequencer.reset(Sequencer::Create());
	attackModeSequencer.reset(Sequencer::Create());
	attackTypeSelector.reset(Selector::Create());
	attackSelector.reset(Selector::Create());

	//木構造を作成
	MakeTree();

	return true;
}

void GalaxyBehaviorTree::Root()
{
	//関数自体をルートノードに見立て、ルートノード直下のノードを実行する
	topSelector->Select();
}

void GalaxyBehaviorTree::MakeTree()
{
	//ルートノード

	//登場状態シーケンサー
	std::function<bool()> appearModeSequence =
		std::bind(&Sequencer::Sequence, appearModeSequencer.get());
	topSelector->AddNode(appearModeSequence);
	//登場状態シーケンスノード
	AppearModeSequenceNode();

	//攻撃状態シーケンサー
	std::function<bool()> attackModeSequence =
		std::bind(&Sequencer::Sequence, attackModeSequencer.get());
	topSelector->AddNode(attackModeSequence);
	//登場状態シーケンスノード
	AttackModeSequenceNode();

	//待機状態
	std::function<bool()> waitMode =
		std::bind(&Galaxy::WaitMode, galaxy);
	topSelector->AddNode(waitMode);

	//攻撃するパーツ変更状態
	std::function<bool()> attackPartChangeMode =
		std::bind(&Galaxy::AttackPartChangeMode, galaxy);
	topSelector->AddNode(attackPartChangeMode);

	//死亡状態セレクター
	std::function<bool()> deadExplosion =
		std::bind(&Galaxy::DeadExplosion, galaxy);
	topSelector->AddNode(deadExplosion);
}

void GalaxyBehaviorTree::AppearModeSequenceNode()
{
	//登場状態シーケンスノード

	//登場
	std::function<bool()> appearMode =
		std::bind(&Galaxy::AppearMode, galaxy);
	appearModeSequencer->AddNode(appearMode);

	//UI生成
	std::function<bool()> appearUICreate =
		std::bind(&Galaxy::AppearUICreate, galaxy);
	appearModeSequencer->AddNode(appearUICreate);
}

void GalaxyBehaviorTree::AttackModeSequenceNode()
{
	//攻撃状態シーケンスノード

	//攻撃状態管理
	std::function<bool()> attackMode =
		std::bind(&Galaxy::AttackMode, galaxy);
	attackModeSequencer->AddNode(attackMode);

	//発射座標更新
	std::function<bool()> updateBulletShotPos =
		std::bind(&Galaxy::UpdateBulletShotPos, galaxy);
	attackModeSequencer->AddNode(updateBulletShotPos);

	//攻撃内容セレクター
	std::function<bool()> attackTypeSelect =
		std::bind(&Selector::Select, attackTypeSelector.get());
	attackModeSequencer->AddNode(attackTypeSelect);
	//攻撃内容セレクトノード
	AttackTypeSelectNode();

	//攻撃セレクター
	std::function<bool()> attackSelect =
		std::bind(&Selector::Select, attackSelector.get());
	attackModeSequencer->AddNode(attackSelect);
	//攻撃状態動き・攻撃セレクトノード
	AttackSelectNode();
}

void GalaxyBehaviorTree::AttackTypeSelectNode()
{
	//攻撃内容セレクトノード

	//攻撃内容設定を開始する
	std::function<bool()> attackTypeSelectStart =
		std::bind(&Galaxy::AttackTypeSelectStart, galaxy);
	attackTypeSelector->AddNode(attackTypeSelectStart);


	//攻撃内容:速射(大砲)を設定する
	std::function<bool()> attackTypeRapidFireCannonSelect =
		std::bind(&Galaxy::AttackTypeRapidFireCannonSelect, galaxy);
	attackTypeSelector->AddNode(attackTypeRapidFireCannonSelect);


	//攻撃内容:火炎放射(船首)を設定する
	std::function<bool()> attackTypeFlamethrowerBowSelect =
		std::bind(&Galaxy::AttackTypeFlamethrowerBowSelect, galaxy);
	attackTypeSelector->AddNode(attackTypeFlamethrowerBowSelect);


	//攻撃内容:あを設定する
	std::function<bool()> attackTypeASelect =
		std::bind(&Galaxy::AttackTypeASelect, galaxy);
	attackTypeSelector->AddNode(attackTypeASelect);
}

void GalaxyBehaviorTree::AttackSelectNode()
{
	//攻撃セレクトノード

	//攻撃内容;速射(大砲)を実行する
	std::function<bool()> attackTypeRapidFireCannon =
		std::bind(&Galaxy::AttackTypeRapidFireCannon, galaxy);
	attackSelector->AddNode(attackTypeRapidFireCannon);


	//攻撃内容:火炎放射(船首)を実行する
	std::function<bool()> attackTypeFlamethrowerBow =
		std::bind(&Galaxy::AttackTypeFlamethrowerBow, galaxy);
	attackSelector->AddNode(attackTypeFlamethrowerBow);


	//攻撃内容;あを実行する
	std::function<bool()> attackTypeA =
		std::bind(&Galaxy::AttackTypeA, galaxy);
	attackSelector->AddNode(attackTypeA);
}
