#pragma once
#include "../SceneBase.h"
#include "../../common.h"
#include "../../Object/Character/Player/Player.h"
#include "../../Object/Character/CharaBase.h"



class GameMainScene :
    public SceneBase
{
private:
	int stage_width_num;    //ステージの横のブロック数
	int stage_height_num;   //ステージの縦のブロック数

	int stage_data[STAGE_MAX_HEIGHT][STAGE_MAX_WIDTH];

	GameObject* player;	//プレイヤーのポインタ
	Vector2D goal_pos;

	std::vector<std::vector<MoveRecord>>stage_clear_history; //ステージクリア履歴

	int back_ground_image; //背景画像
	int back_ground_img[5];

	float clone_spawn_timer;
	bool is_create;

	bool is_game_over;         // ゲームオーバー状態かどうか
	int fade_alpha;

public:
	GameMainScene();
	~GameMainScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() const override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;

	//ステージデータを読み込む
	void LoadStage();

	//ステージデータを生成
	void SetStage();

	//カメラの位置を更新
	void UpdateCamera();

	//ステージクリア処理
	void StageClear();

	//ステージリロード
	void ReLoadStage();

	//プレイヤーを探す
	void FindPlayer();

	//エネミーを生成
	void CreateClone();

	//アイテムの生成
	void CreateItem();

	//ギミックの生成
	void CreateGimmick();

};

