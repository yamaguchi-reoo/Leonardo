#pragma once
#include "../SceneBase.h"
#include <vector>
class TitleScene :
    public SceneBase
{
private:
	enum MenuItem
	{
		MENU_START,
		MENU_HELP,
		MENU_RANKING,
		MENU_CREDITS,
		MENU_END
	};

	int select_index;

;	int menu_font;
	int title_font;
	int small_font;

	std::vector<int> animation_data;
	int cursor_image;
	int cursor_frame;
	int cursor_timer;

public:
	TitleScene();
	~TitleScene();

	void Initialize() override;		//‰Šú‰»ˆ—
	eSceneType Update() override;	//XVˆ—
	void Draw() const override;		//•`‰æˆ—
	void Finalize() override;		//I—¹ˆ—

	eSceneType GetNowSceneType()const override;

	void LoadTitleImage();
};

