#pragma once
#include "../SceneBase.h"
class ResultScene :
	public SceneBase
{
private:
	int display_clear_count = 0;	// �N���A�񐔂�\������ϐ�
	int clear_timer = 0;			// �N���A�񐔂̕\���^�C�}�[
public:
	ResultScene();
	~ResultScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() const override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;
};

