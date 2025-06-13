#pragma once
#include "../SceneBase.h"
class ResultScene :
	public SceneBase
{
private:
	int display_clear_count = 0;	// クリア回数を表示する変数
	int clear_timer = 0;			// クリア回数の表示タイマー
public:
	ResultScene();
	~ResultScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() const override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;
};

