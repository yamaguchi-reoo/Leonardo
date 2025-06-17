#pragma once
#include <string>
#include "../SceneBase.h"

class NameInputScene :
	public SceneBase
{
private:
	std::string player_name;	// �v���C���[�����i�[����ϐ�
	const int max_name_length = 10; // �ő啶����

	std::vector<std::string> keys;
	int cursor_x;
	int cursor_y; 

	int grid_width; // �O���b�h�̕�
	int grid_height; // �O���b�h�̍���

	bool is_enter_name;
	int input_cooldown; // ���̓N�[���_�E���^�C�}�[

	std::vector<int> sounds_data;
	int select_se;
	int decision_se;


public:
	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType() const override;

private:
	void HandleInput(); // ���͏���
	void DrawKeyboard() const; // �L�[�{�[�h�̕`��

	void LoadResource();
	void PlaySoundSe(int _handle, int volume);
};

