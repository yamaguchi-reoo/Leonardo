#pragma once
#include "../SceneBase.h"
class ResultScene :
	public SceneBase
{
private:
	int display_clear_count = 0;	// クリア回数を表示する変数
	int clear_timer = 0;			// クリア回数の表示タイマー

	std::vector<int> sounds_data;
	int decision_se;
	int result_se;

	int box_anim_timer = 0;               // ボックス展開用のアニメーションタイマー
	const int box_anim_duration = 30;     // 展開にかかる時間（フレーム数）
	bool is_box_expanded = false;         // 展開完了したか
	bool is_score_count_done = false;

public:
	ResultScene();
	~ResultScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;


	void PlaySoundSe(int _handle, int volume);
};

