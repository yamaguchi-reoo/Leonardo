#pragma once
#include "../CharaBase.h"
#include "../../../Utility/InputControl.h"
#include "HealParticle.h"
#include "../../Gimmick/GoalParticle.h"

#include <map>
#include <vector>

//プレイヤーの行動状態を表す列挙型
enum class ActionState { IDLE, WALK, JUMP, DAMAGE };

//移動履歴の構造体
struct PlayerMoveRecord : public MoveRecord
{
    ActionState action_state;
};

class Player :
    public CharaBase
{
private:
    //std::vector<int> animation_data;//アニメーションデータ

	std::vector<PlayerMoveRecord> move_history; //移動履歴

    int jump_time = 0;

    //ダメージを受けている時間
    int damage_timer;

    //プレイヤーの向き（移動方向）を管理
    enum class MoveDirection { NONE, LEFT, RIGHT };

    ActionState action_state = ActionState::JUMP;   // 現在の行動状態
    MoveDirection move = MoveDirection::NONE;       // 現在の移動方向


    bool is_invincible;
    int invincible_timer;

    bool is_goal;

    std::map<ActionState,std::vector<int>> animation_data;

    // 各行動状態のアニメーションのフレーム数
	std::map<ActionState, int> animation_frame_count;

	int animation_frame = 0; //現在のアニメーションフレーム

    //回復パーティクルの管理配列
    std::vector<HealParticle> heal_particles;


	bool is_teleport = false; // テレポート中かどうか
	int teleport_timer = 0; // テレポートのタイマー
    std::vector<GoalParticle> teleport_particles;

public:
    Player();
    ~Player();

    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) override;
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
    const std::vector<PlayerMoveRecord>& GetMoveHistory() const {
        return move_history;
    }

    // ダメージを受けた際の処理
    void ApplyDamage();

    // プレイヤー画像の読み込み処理
    void LoadPlayerImage();

    // 無敵状態のエフェクト描画
    void InvincibleEffect(Vector2D offset);

    // 回復パーティクルの更新処理
    void UpdateHealParticle(HealParticle& particle);

    // 回復パーティクルの描画処理
    void DrawHealParticle(const HealParticle& particle, Vector2D offset);

    // プレイヤーがゴールした際の処理
    void PlayerToGoal();

    // プレイヤーのテレポート処理開始
    void PlayerTeleport();

    // テレポート処理の更新（タイマー管理など）
    void UpdateTeleport();

    // テレポートエフェクトの描画
    void DrawTeleport(Vector2D offset);
};

