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
	//���@�e���X�g�ɓo�^
	playerBullets.push_back(std::move(playerBullet));
}

void BaseStageScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//�G�e���X�g�ɓo�^
	enemyBullets.push_back(std::move(enemyBullet));
}

void BaseStageScene::AddEnemyBreakEffect(std::unique_ptr<EnemyBreakEffect> enemyBreakEffect)
{
	//�G�j��G�t�F�N�g���X�g�ɓo�^
	enemyBreakEffects.push_back(std::move(enemyBreakEffect));
}

void BaseStageScene::LoadEnemySetData(std::stringstream& commands, const std::string& fileName)
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	commands << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void BaseStageScene::UpdateEnemySetCommands(const Vector3& targetPosition)
{
	//�ҋ@����
	if (isWait) {
		//�J������Z���W���������W�ȏ�Ȃ�
		if (waitEnemySetPlayerPosition <= targetPosition.z) {
			//�ҋ@�I��
			isWait = false;
		}
		return;
	}

	//1�s���̕����������ϐ�
	std::string line;

	//�R�}���h�����s���郋�[�v
	while (getline(enemySetCommands, line)) {
		//1�s���̕�������X�g���[���ϊ����ĉ�͂��₷��
		std::istringstream line_stream(line);

		std::string word;
		//�u,�v��؂�ōs�̐擪�������擾
		getline(line_stream, word, ',');

		//"//"����n�߂�s�̓R�����g
		if (word.find("//") == 0) {
			//�R�����g�s���΂�
			continue;
		}

		//POP�R�}���h
		if (word.find("POP") == 0) {
			//�G�̎��
			getline(line_stream, word, ',');
			int type = (int)std::atof(word.c_str());

			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//�G�𔭐�
			if (type == Enemy::EnemyType::Cannon) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CannonEnemy::Create({ x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Circular) {
				//�p�x
				getline(line_stream, word, ',');
				float angle = (float)std::atof(word.c_str());

				//���a�̒���
				getline(line_stream, word, ',');
				float length = (float)std::atof(word.c_str());

				//��]���x
				getline(line_stream, word, ',');
				float rotSpeed = (float)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CircularEnemy::Create({ x, y, z }, angle, length, rotSpeed));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Fall) {
				//�~������l
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
				//x�������W
				getline(line_stream, word, ',');
				float comeX = (float)std::atof(word.c_str());
				//y�������W
				getline(line_stream, word, ',');
				float comeY = (float)std::atof(word.c_str());
				//z�������W
				getline(line_stream, word, ',');
				float comeZ = (float)std::atof(word.c_str());

				//x�o�����W
				getline(line_stream, word, ',');
				float goX = (float)std::atof(word.c_str());
				//y�o�����W
				getline(line_stream, word, ',');
				float goY = (float)std::atof(word.c_str());
				//z�o�����W
				getline(line_stream, word, ',');
				float goZ = (float)std::atof(word.c_str());

				//�U������
				getline(line_stream, word, ',');
				int time = (int)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(ComeGoEnemy::Create({ x, y, z }, { comeX, comeY, comeZ }, { goX, goY, goZ }, time));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Meteorite) {
				//��]�px
				getline(line_stream, word, ',');
				float rotX = (float)std::atof(word.c_str());
				//��]�py
				getline(line_stream, word, ',');
				float rotY = (float)std::atof(word.c_str());
				//��]�pz
				getline(line_stream, word, ',');
				float rotZ = (float)std::atof(word.c_str());

				//�T�C�Y
				getline(line_stream, word, ',');
				float size = (float)std::atof(word.c_str());

				//���xx
				getline(line_stream, word, ',');
				float velX = (float)std::atof(word.c_str());
				//���xy
				getline(line_stream, word, ',');
				float velY = (float)std::atof(word.c_str());
				//���xz
				getline(line_stream, word, ',');
				float velZ = (float)std::atof(word.c_str());

				//��]���xx
				getline(line_stream, word, ',');
				float rotSpeedX = (float)std::atof(word.c_str());
				//��]���xy
				getline(line_stream, word, ',');
				float rotSpeedY = (float)std::atof(word.c_str());
				//��]���xz
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

		//WAIT�R�}���h
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//�����Ώۍ��W�X�V
			float waitPosition = (float)atoi(word.c_str());

			//�ҋ@�J�n
			isWait = true;
			waitEnemySetPlayerPosition = waitPosition;

			//�R�}���h���[�v�𔲂���
			break;
		}
	}
}
