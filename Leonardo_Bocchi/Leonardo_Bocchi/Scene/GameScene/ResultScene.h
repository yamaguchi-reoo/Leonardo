#pragma once
#include "../SceneBase.h"
class ResultScene :
	public SceneBase
{
private:
	int display_clear_count = 0;	// �N���A�񐔂�\������ϐ�
	int clear_timer = 0;			// �N���A�񐔂̕\���^�C�}�[

	std::vector<int> sounds_data;
	int decision_se;
	int result_se;

public:
	ResultScene();
	~ResultScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;


	void PlaySoundSe(int _handle, int volume);
};

