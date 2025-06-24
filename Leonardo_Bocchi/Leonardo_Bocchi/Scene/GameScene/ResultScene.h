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

	int box_anim_timer = 0;               // �{�b�N�X�W�J�p�̃A�j���[�V�����^�C�}�[
	const int box_anim_duration = 30;     // �W�J�ɂ����鎞�ԁi�t���[�����j
	bool is_box_expanded = false;         // �W�J����������
	bool is_score_count_done = false;

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

