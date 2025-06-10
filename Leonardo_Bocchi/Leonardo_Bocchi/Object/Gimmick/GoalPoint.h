#pragma once
#include "../GameObject.h"
#include "GoalParticle.h"
#include <vector>

class GoalPoint :
    public GameObject
{
private:
	std::vector<GoalParticle> particles; // ゴールパーティクルのリスト

	bool is_active = false; // ゴールがアクティブかどうか
	int effect_timer = 0; // エフェクトのタイマー

public:
    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) override;
    //終了時処理
    void Finalize()override;

    void OnHitCollision(GameObject* hit_object)override;

    void CreateParticles();

    bool IsActive() const;

};

