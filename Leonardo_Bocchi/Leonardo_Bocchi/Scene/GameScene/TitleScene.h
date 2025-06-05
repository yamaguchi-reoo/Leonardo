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

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() const override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;
};

