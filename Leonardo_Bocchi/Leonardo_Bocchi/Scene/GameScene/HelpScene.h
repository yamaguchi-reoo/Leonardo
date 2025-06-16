#pragma once
#include "../SceneBase.h"
#include <vector>
#include <string>
#include <map>

class HelpScene :
    public SceneBase
{
private:
	int font_small;
	int font_large;

	int current_page; // ���݂̃y�[�W�ԍ�
	int total_pages; // ���y�[�W��

	std::vector < std::vector<std::string>> help_texts; // �y�[�W���Ƃ̃w���v�e�L�X�g

	enum class PlayerAnimationType { IDLE, WALK, JUMP };

	int frame_width = 64;
	int frame_height = 64;

	std::map<PlayerAnimationType, int> animation_frame;
	std::map<PlayerAnimationType, int> animation_timer;

	std::map<PlayerAnimationType, std::vector<int>> animation_data; // {�J�n�t���[��, �t���[����}
public:
	HelpScene();
	~HelpScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;

private:
	void ObjectAnimation();

	void LoadResource();

	void DrawObject();
};

