#pragma once
#include "../CharaBase.h"
#include <vector>
#include "../../../Utility/InputControl.h"
#include <map>


class Player :
    public CharaBase
{
private:
    //std::vector<int> animation_data;//アニメーションデータ

	std::vector<MoveRecord> move_history; //移動履歴

    int jump_time = 0;

    //ダメージを受けている時間
    int damage_timer;

    //状態管理
    enum class ActionState { IDLE, WALK, JUMP, DAMAGE };
    enum class MoveDirection { NONE, LEFT, RIGHT };

    ActionState action_state = ActionState::IDLE;
    MoveDirection move = MoveDirection::NONE;


    bool is_invincible;
    int invincible_timer;


    std::map<ActionState,std::vector<int>> animation_data;
	std::map<ActionState, int> animation_frame_count;
	int animation_frame = 0; //現在のアニメーションフレーム


public:
    Player();
    ~Player();

    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) const override;
    //終了時処理
    void Finalize()override;

public:

    void ApplyDeceleration();
    void ConstrainVelocity();
    

    void HandleInput();

    //アニメーション管理
    void AnimationControl();
    //当たった時の挙動
    void OnHitCollision(GameObject* hit_object)override;

	//移動履歴を保存
	void SaveMoveHistory();

	//移動履歴を取得
    const std::vector<MoveRecord>& GetMoveHistory() const {
        return move_history;
    }

    void ApplyDamage();


    void LoadPlayerImage();
};

