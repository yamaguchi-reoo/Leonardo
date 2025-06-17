#pragma once
#include "../SceneBase.h"
#include <vector>
#include <string>
#include <map>

enum class HelpObjectType {
	Image,
	Shapes,
};

struct HelpInfo
{
	std::string image_path; //�摜�̃p�X
	std::string name;		//���O
	std::string text;		//�e�L�X�g
	HelpObjectType type;	//�I�u�W�F�N�g�̎��
};

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

	void DrawPlayerControls();
	void DrawObjectPage();

	void DrawTeleport(int x, int y);

	void DrawEllipseAA(float cx, float cy, float rx, float ry, int num_segments, int color, bool fill, int line_thickness);
};

