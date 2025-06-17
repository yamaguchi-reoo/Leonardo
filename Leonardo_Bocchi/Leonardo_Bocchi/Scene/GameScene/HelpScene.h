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

; 
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

	void DrawAButton(int x, int y);

	void DrawStick(int x, int y);
};

