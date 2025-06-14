#pragma once
#include <string>
#include "../SceneBase.h"

class NameInputScene :
	public SceneBase
{
private:
	std::string player_name;	// プレイヤー名を格納する変数
	const int max_name_length = 10; // 最大文字数

	std::vector<std::string> keys;
	int cursor_x;
	int cursor_y; 

	int grid_width; // グリッドの幅
	int grid_height; // グリッドの高さ

	bool is_enter_name;
	int input_cooldown; // 入力クールダウンタイマー


public:
	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() const override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType() const override;

private:
	void HandleInput(); // 入力処理
	void DrawKeyboard() const; // キーボードの描画
};

