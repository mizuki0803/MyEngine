#include "BaseStageScene.h"
#include "CannonEnemy.h"
#include "CircularEnemy.h"
#include "FallEnemy.h"
#include "UpDownEnemy.h"
#include "ComeGoEnemy.h"
#include "MeteoriteEnemy.h"
#include <fstream>

void BaseStageScene::AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet)
{
	//自機弾リストに登録
	playerBullets.push_back(std::move(playerBullet));
}

void BaseStageScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//敵弾リストに登録
	enemyBullets.push_back(std::move(enemyBullet));
}

void BaseStageScene::AddEnemyBreakEffect(std::unique_ptr<EnemyBreakEffect> enemyBreakEffect)
{
	//敵破壊エフェクトリストに登録
	enemyBreakEffects.push_back(std::move(enemyBreakEffect));
}

void BaseStageScene::LoadEnemySetData(std::stringstream& commands, const std::string& fileName)
{
	//ファイルを開く
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	commands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void BaseStageScene::UpdateEnemySetCommands(const Vector3& targetPosition)
{
	//待機処理
	if (isWait) {
		//カメラのZ座標が生成座標以上なら
		if (waitEnemySetPlayerPosition <= targetPosition.z) {
			//待機終了
			isWait = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンドを実行するループ
	while (getline(enemySetCommands, line)) {
		//1行分の文字列をストリーム変換して解析しやすく
		std::istringstream line_stream(line);

		std::string word;
		//「,」区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始める行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//敵の種類
			getline(line_stream, word, ',');
			int type = (int)std::atof(word.c_str());

			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生
			if (type == Enemy::EnemyType::Cannon) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CannonEnemy::Create({ x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Circular) {
				//角度
				getline(line_stream, word, ',');
				float angle = (float)std::atof(word.c_str());

				//半径の長さ
				getline(line_stream, word, ',');
				float length = (float)std::atof(word.c_str());

				//回転速度
				getline(line_stream, word, ',');
				float rotSpeed = (float)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CircularEnemy::Create({ x, y, z }, angle, length, rotSpeed));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Fall) {
				//降下する値
				getline(line_stream, word, ',');
				float fallNum = (float)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(FallEnemy::Create({ x, y, z }, fallNum));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::UpDown) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(UpDownEnemy::Create({ x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::ComeGo) {
				//x到着座標
				getline(line_stream, word, ',');
				float comeX = (float)std::atof(word.c_str());
				//y到着座標
				getline(line_stream, word, ',');
				float comeY = (float)std::atof(word.c_str());
				//z到着座標
				getline(line_stream, word, ',');
				float comeZ = (float)std::atof(word.c_str());

				//x出発座標
				getline(line_stream, word, ',');
				float goX = (float)std::atof(word.c_str());
				//y出発座標
				getline(line_stream, word, ',');
				float goY = (float)std::atof(word.c_str());
				//z出発座標
				getline(line_stream, word, ',');
				float goZ = (float)std::atof(word.c_str());

				//攻撃時間
				getline(line_stream, word, ',');
				int time = (int)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(ComeGoEnemy::Create({ x, y, z }, { comeX, comeY, comeZ }, { goX, goY, goZ }, time));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Meteorite) {
				//回転角x
				getline(line_stream, word, ',');
				float rotX = (float)std::atof(word.c_str());
				//回転角y
				getline(line_stream, word, ',');
				float rotY = (float)std::atof(word.c_str());
				//回転角z
				getline(line_stream, word, ',');
				float rotZ = (float)std::atof(word.c_str());

				//サイズ
				getline(line_stream, word, ',');
				float size = (float)std::atof(word.c_str());

				//速度x
				getline(line_stream, word, ',');
				float velX = (float)std::atof(word.c_str());
				//速度y
				getline(line_stream, word, ',');
				float velY = (float)std::atof(word.c_str());
				//速度z
				getline(line_stream, word, ',');
				float velZ = (float)std::atof(word.c_str());

				//回転速度x
				getline(line_stream, word, ',');
				float rotSpeedX = (float)std::atof(word.c_str());
				//回転速度y
				getline(line_stream, word, ',');
				float rotSpeedY = (float)std::atof(word.c_str());
				//回転速度z
				getline(line_stream, word, ',');
				float rotSpeedZ = (float)std::atof(word.c_str());

				//HP
				getline(line_stream, word, ',');
				int HP = (int)std::atof(word.c_str());

				std::unique_ptr<MeteoriteEnemy> newEnemy;
				newEnemy.reset(MeteoriteEnemy::Create({ x, y, z }, { rotX, rotY, rotZ }, size, { velX, velY, velZ }, { rotSpeedX, rotSpeedY, rotSpeedZ }, HP));
				enemys.push_back(std::move(newEnemy));
			}
		}

		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//生成対象座標更新
			float waitPosition = (float)atoi(word.c_str());

			//待機開始
			isWait = true;
			waitEnemySetPlayerPosition = waitPosition;

			//コマンドループを抜ける
			break;
		}
	}
}
