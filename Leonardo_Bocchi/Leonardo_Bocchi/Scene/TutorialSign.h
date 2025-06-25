#pragma once
#include "../Utility/UtilityList.h"
#include <string>

class TutorialSign
{
private:
	Vector2D position;
	std::string message;
	int font;

	float trigger_distance;
	bool is_visible;
	int visible_timer;
	bool is_close = false;


	float box_width = 0.0f;           // �W�J���̕�
	float box_height = 0.0f;          // �W�J���̍���
	float box_max_width = 0.0f;       // �ő啝
	float box_max_height = 0.0f;      // �ő卂��
	int anim_timer = 0;            // �W�J�p�^�C�}�[
	bool is_animating = false;         // �A�j���[�V���������ǂ���

public:
	TutorialSign(const Vector2D& pos, const std::string& msg, float dis);
	~TutorialSign();

	void Initialize();
	
	void Update(const Vector2D& player_pos);
	void Draw();

};

