#include "MultiHitUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include <DirectXMath.h>

MultiHitUI* MultiHitUI::Create(const Vector3& position, GameCamera* camera, const int enemyHitNum)
{
	//�������jUI�̃C���X�^���X�𐶐�
	MultiHitUI* multiHitUI = new MultiHitUI();
	if (multiHitUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!multiHitUI->Initialize(position, camera, enemyHitNum)) {
		delete multiHitUI;
		assert(0);
		return nullptr;
	}

	return multiHitUI;
}

bool MultiHitUI::Initialize(const Vector3& position, GameCamera* camera, const int enemyHitNum)
{
	//HIT���[���h���W�擾
	this->hitWorldPos = position;
	//�J�����擾
	this->camera = camera;
	//HIT���擾
	this->hitNum = enemyHitNum;


	Vector2 hitScreenPos = GetScreenPos(hitWorldPos, camera);

	//�g�X�v���C�g����
	hitTextSprite.reset(Sprite::Create(SpriteTextureLoader::HitPlusText));
	const Vector2 hitTextPos = { hitScreenPos.x + 25, hitScreenPos.y };
	hitTextSprite->SetPosition(hitTextPos);
	const Vector2 hitTextSize = { 150, 37.5 };
	const Vector2 hitTextTexSize = { 200, 50 };
	hitTextSprite->SetSize(hitTextSize);
	hitTextSprite->SetTexSize(hitTextTexSize);

	//�������擾
	int defeatNum = enemyHitNum;
	while (defeatNum != 0) {
		defeatNum = defeatNum / 10;
		hitNumDigit++;
	}

	//�����̕��A�����X�v���C�g����
	for (int i = 0; i < hitNumDigit; i++) {
		std::unique_ptr<NumberSprite> newNumberSprite;
		const Vector2 size = { 36, 36 };
		const Vector2 texSize = { 48, 48 };
		const Vector2 pos = { hitTextPos.x + texSize.x + ((hitNumDigit - i) * size.x), hitTextPos.y };
		newNumberSprite.reset(NumberSprite::Create(SpriteTextureLoader::HitPlusNumber, pos, size, texSize));
		//���鐔
		int divisionNum = 1;
		//������10����Z����(1,10,100,...)
		for (int j = 0; j < i; j++) {
			divisionNum *= 10;
		}
		//�w�肵�����̐������o��
		int numberDigit = (hitNum / divisionNum) % 10;
		//�Z�o�������̐����𐔎��X�v���C�g�Ɋ��蓖�Ă�
		newNumberSprite->SetNumber(numberDigit);
		numberSprites.push_back(std::move(newNumberSprite));
	}

	return true;
}

void MultiHitUI::Update()
{
	//�����^�C�}�[�X�V
	deadTimer++;
	//�^�C�}�[���B�����玀�S
	if (deadTimer >= lifeTime) {
		isDead = true;
	}

	//�e�L�X�g�����[���h���W��ŏ�ɓ�����
	TextMove();

	//�J��������O�ɂ��邩�m�F
	FrontOfScreenDelete();

	//�uHIT�v�e�L�X�g�X�V
	hitTextSprite->Update();

	//�����̕��A�����X�v���C�g�X�V
	for (int i = 0; i < hitNumDigit; i++) {
		numberSprites[i]->Update();
	}
}

void MultiHitUI::Draw()
{
	//�uHIT�v�e�L�X�g�`��
	hitTextSprite->Draw();

	//�����̕��A�����X�v���C�g�`��
	for (int i = 0; i < hitNumDigit; i++) {
		numberSprites[i]->Draw();
	}
}

Vector2 MultiHitUI::GetScreenPos(const Vector3& position, GameCamera* camera)
{
	Vector3 worldPos = position;

	//�r���[�|�[�g�s��
	DirectX::XMMATRIX matViewport = {
		{1280 / 2, 0, 0, 0 },
		{0, -720 / 2, 0, 0},
		{0, 0, 1, 0},
		{1280 / 2, 720 / 2, 0, 1}
	};

	//�r���[�s��ƃv���W�F�N�V�����s��ƃr���[�|�[�g�s�������
	DirectX::XMMATRIX matViewProjectionViewport =
		camera->GetMatView() * camera->GetMatProjection() * matViewport;

	//���[���h���W����X�N���[�����W�ɕϊ�
	worldPos = MatrixTransformWDivision(worldPos, matViewProjectionViewport);
	Vector2 screenPos = { worldPos.x, worldPos.y };

	return screenPos;
}

void MultiHitUI::TextMove()
{
	//�C�[�W���O�p�^�C���擾
	const float time = (float)deadTimer / (float)lifeTime;

	//���[���h���W��ŏ�ɓ�����
	Vector3 worldPos = hitWorldPos;
	const float upNum = 5.0f;
	worldPos.y = Easing::OutQuint(hitWorldPos.y, hitWorldPos.y + upNum, time);

	//�X�N���[�����W�ɕϊ�
	Vector2 screenPos = GetScreenPos(worldPos, camera);

	//HIT�X�v���C�g�̍��W���Z�b�g
	const Vector2 hitTextPos = { screenPos.x - 10, screenPos.y };
	hitTextSprite->SetPosition(hitTextPos);

	//�����̕��A�����X�v���C�g�̍��W���Z�b�g
	for (int i = 0; i < hitNumDigit; i++) {
		const Vector2 size = numberSprites[i]->GetSize();
		const Vector2 pos = { hitTextPos.x + 48 + ((hitNumDigit - i) * size.x), hitTextPos.y };
		numberSprites[i]->SetPosition(pos);
	}
}

void MultiHitUI::FrontOfScreenDelete()
{
	//���W���J��������O(��ʊO��O)�܂ōs������폜
	if (hitWorldPos.z > camera->GetPosition().z) { return; }

	isDead = true;
}
