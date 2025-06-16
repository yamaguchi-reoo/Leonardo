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

	int current_page; // 現在のページ番号
	int total_pages; // 総ページ数


	std::vector < std::vector<std::string>> help_texts; // ページごとのヘルプテキスト


public:
	HelpScene();
	~HelpScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() const override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;

private:
	void LoadResource();
};

