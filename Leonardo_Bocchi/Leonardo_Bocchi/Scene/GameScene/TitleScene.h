#pragma once
#include "../SceneBase.h"
class TitleScene :
    public SceneBase
{
private:
	enum MenuItem
	{
		MENU_START,
		MENU_HELP,
		MENU_RANKING,
		MENU_END
	};

	int select_index;

	int menu_font;
	int title_font;
	int small_font;

public:
	TitleScene();
	~TitleScene();

	void Initialize() override;		//‰Šú‰»ˆ—
	eSceneType Update() override;	//XVˆ—
	void Draw() const override;		//•`‰æˆ—
	void Finalize() override;		//I—¹ˆ—

	eSceneType GetNowSceneType()const override;
};

