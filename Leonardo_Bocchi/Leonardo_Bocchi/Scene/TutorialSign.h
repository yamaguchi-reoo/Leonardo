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


	float box_width = 0.0f;           // 展開中の幅
	float box_height = 0.0f;          // 展開中の高さ
	float box_max_width = 0.0f;       // 最大幅
	float box_max_height = 0.0f;      // 最大高さ
	int anim_timer = 0;            // 展開用タイマー
	bool is_animating = false;         // アニメーション中かどうか

public:
	TutorialSign(const Vector2D& pos, const std::string& msg, float dis);
	~TutorialSign();

	void Initialize();
	
	void Update(const Vector2D& player_pos);
	void Draw();

};

