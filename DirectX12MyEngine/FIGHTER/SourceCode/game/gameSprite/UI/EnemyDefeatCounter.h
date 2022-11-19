#pragma once

/// <summary>
/// �G��|�������J�E���^�[
/// </summary>
class EnemyDefeatCounter
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// �J�E���g�𑝂₷
	/// </summary>
	/// <param name="count">���₷��</param>
	static void AddCounter(int count = 1);

	/// <summary>
	/// �J�E���^�[���Z�b�g
	/// </summary>
	static void CounterReset();

	/// <summary>
	/// �n�C�X�R�A�X�V���m�F
	/// </summary>
	static void CheckHighScore();

	//getter
	static int GetDefeatCount() { return defeatCounter; }
	static int GetHighScore() { return highScore; }

private: //�ÓI�����o�ϐ�
	//�J�E���^�[
	static int defeatCounter;
	//�n�C�X�R�A
	static int highScore;
};