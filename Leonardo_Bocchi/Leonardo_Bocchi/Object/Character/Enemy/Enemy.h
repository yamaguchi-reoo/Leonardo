#pragma once
#include "../CharaBase.h"
#include <vector>

class Enemy :
    public CharaBase
{
private:
	//エネミーの状態
    std::vector<MoveRecord> replay_history;
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
    void Draw(Vector2D offset, double rate) const override;
    //終了時処理
    void Finalize()override;

public:
    
    //プレイヤーの動き
    void Movement();
    //アニメーション管理
    void AnimationControl();
    //当たった時の挙動
    void OnHitCollision(GameObject* hit_object)override;

	
	//移動履歴をセット
    void SetReplayHistory(const std::vector<MoveRecord>& history);
};

