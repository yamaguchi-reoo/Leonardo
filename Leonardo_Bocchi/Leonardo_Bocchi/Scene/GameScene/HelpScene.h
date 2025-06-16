#pragma once
#include "../SceneBase.h"
#include <vector>
#include <string>

class HelpScene :
    public SceneBase
{
private:
	int font_small;
	int font_large;

	int current_page; // ���݂̃y�[�W�ԍ�
	int total_pages; // ���y�[�W��


	std::vector < std::vector<std::string>> help_texts; // �y�[�W���Ƃ̃w���v�e�L�X�g


public:
	HelpScene();
	~HelpScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() const override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;

private:
	void LoadResource();
};

