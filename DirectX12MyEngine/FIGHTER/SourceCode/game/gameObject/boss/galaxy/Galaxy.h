#pragma once
#include "GalaxyBody.h"
#include "GalaxyBow.h"
#include "GalaxyCannon.h"
#include "BossNameUI.h"
#include "BossHPUI.h"
#include "GalaxyBehaviorTree.h"

//���@�N���X��O���錾
class BasePlayer;

/// <summary>
/// �{�X �X�e�[�W2�{�X
/// </summary>
class Galaxy
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Appear,	//�o��
		Attack,	//�U��
		Wait,	//�ҋ@
		AttackPartChange, //�U������p�[�c��ύX
		Dead,	//���S
	};

	//�U������p�[�c�̃t�F�[�Y
	enum class AttackPartPhase {
		None,	//���ݒ�
		Cannon,	//��C
		Front,	//�D��
	};

	//�U�����
	enum class AttackType {
		None,				//���ݒ�
		RapidFireCannon,	//����(��C)
		a,					//��
		FlamethrowerBow,	//�Ή�����(�D��)
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="bornPos">�������W</param>
	/// <param name="basePos">����W</param>
	/// <returns>�M�����N�V�[</returns>
	static Galaxy* Create(const Vector3& bornPos, const Vector3& basePos);

	//setter
	static void SetPlayer(BasePlayer* player) { Galaxy::player = player; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="bornPos">�������W</param>
	/// <param name="basePos">����W</param>
	/// <returns>����</returns>
	bool Initialize(const Vector3& bornPos, const Vector3& basePos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�D��)
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	/// <param name="collisionPos">�Փˍ��W</param>
	void OnCollisionBow(const int damageNum, const Vector3& collisionPos);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(��C)
	/// </summary>
	/// <param name="cannon">�ǂ̑�C��</param>
	/// <param name="damageNum">�_���[�W��</param>
	/// <param name="collisionPos">�Փˍ��W</param>
	void OnCollisionCannon(GalaxyCannon* cannon, const int damageNum, const Vector3& collisionPos);

	/// <summary>
	/// �o����
	/// </summary>
	bool AppearMode();

	/// <summary>
	/// �o���Ԃ�UI����
	/// </summary>
	bool AppearUICreate();

	/// <summary>
	/// �U�����
	/// </summary>
	bool AttackMode();

	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	bool UpdateBulletShotPos();

	/// <summary>
	/// �U�����e�ݒ���J�n���邩
	/// </summary>
	bool AttackTypeSelectStart();

	/// <summary>
	/// �U�����e:����(��C)��ݒ肷�邩
	/// </summary>
	bool AttackTypeRapidFireCannonSelect();

	/// <summary>
	/// �U�����e:�u���v��ݒ肷�邩
	/// </summary>
	bool AttackTypeASelect();

	/// <summary>
	/// �U�����e:�Ή�����(�D��)��ݒ肷�邩
	/// </summary>
	bool AttackTypeFlamethrowerBowSelect();

	/// <summary>
	/// �U�����e;����(��C)
	/// </summary>
	bool AttackTypeRapidFireCannon();

	/// <summary>
	/// �U�����e;�u���v
	/// </summary>
	bool AttackTypeA();

	/// <summary>
	/// �U�����e:�Ή�����(�D��)
	/// </summary>
	bool AttackTypeFlamethrowerBow();

	/// <summary>
	/// �ҋ@���
	/// </summary>
	bool WaitMode();

	/// <summary>
	/// �U������p�[�c�ύX���
	/// </summary>
	bool AttackPartChangeMode();

	/// <summary>
	/// ���S����
	/// </summary>
	bool DeadExplosion();

	//getter
	const Phase& GetPhase() { return this->phase; }
	GalaxyBody* GetBody() { return body.get(); }
	GalaxyBow* GetBow() { return bow.get(); }
	const std::list<std::unique_ptr<GalaxyCannon>>& GetCannons() { return cannons; };
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(const int damageNum);

	/// <summary>
	/// �U����ԊJ�n����
	/// </summary>
	void AttackModeStart();

	/// <summary>
	/// ��C���S�ł������`�F�b�N����
	/// </summary>
	void CheckAllCannonDead();

private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static BasePlayer* player;
	//�ҋ@��Ԏ���
	static const float waitModeTime;

private: //�����o�ϐ�
	//����
	std::unique_ptr<GalaxyBody> body;
	//�D��
	std::unique_ptr<GalaxyBow> bow;
	//��C
	std::list<std::unique_ptr<GalaxyCannon>> cannons;
	//�X�e�[�W2�{�X�̍s���J�ڃr�w�C�r�A�c���[
	std::unique_ptr<GalaxyBehaviorTree> behaviorTree;
	//�s��
	Phase phase = Phase::Appear;
	//�������W
	Vector3 bornPos = {};
	//����W
	Vector3 basePos = {};
	//�U������p�[�c�t�F�[�Y
	AttackPartPhase attackPartPhase = AttackPartPhase::None;
	//�U�����e
	AttackType attackType = AttackType::None;
	//1�O�ɔ��������U�����e
	AttackType preAttackType = AttackType::None;
	//�ҋ@��Ԏ��ԃ^�C�}�[
	int32_t waitModeTimer = 0;
	//���S��Ԏ��ԃ^�C�}�[
	int32_t deadModeTimer = 0;
	//�{�X���\��UI
	std::unique_ptr<BossNameUI> bossNameUI;
	//HPUI
	std::unique_ptr<BossHPUI> hpUI;
	//�̗�
	int HP = 0;
	//���S�t���O
	bool isDead = false;
	//�폜�t���O
	bool isDelete = false;
};
