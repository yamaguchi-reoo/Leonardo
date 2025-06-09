#pragma once
#include "../CharaBase.h"
#include <vector>
#include "../Player/Player.h"

#include <map>


class Enemy :
    public CharaBase
{
private:
	//エネミーの状態
    std::vector<PlayerMoveRecord> replay_history;                    // 現在の再生インデックス

    ActionState action_state = ActionState::IDLE;

    std::map<ActionState, std::vector<int>> animation_data;  // アニメーションデータ
    int animation_frame = 0;


    int current_frame = 0;

    int frame_timer = 0;            // 経過フレーム数
    int frame_duration = 1;         // 何フレームかけて1ステップ進むか


    Vector2D start_position;
    Vector2D end_position;
public:
    Enemy();
    ~Enemy();

    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate)  override;
    //終了時処理
    void Finalize()override;

public:
    
    //プレイヤーの動き
    void Movement();
    //アニメーション管理
    void AnimationControl(ActionState state);
    //当たった時の挙動
    void OnHitCollision(GameObject* hit_object)override;

	
	//移動履歴をセット
    void SetReplayHistory(const std::vector<PlayerMoveRecord>& history);

	void LoadEnemyImage();
};

