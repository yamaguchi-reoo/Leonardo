#pragma once
#include "../GameObject.h"
#include "../../Utility/UserTemplate.h"
#include <vector>


struct MoveRecord {
	Vector2D position;
	bool is_jumping;
	bool flip;
};

class CharaBase :
    public GameObject
{
protected:
	float g_velocity = 0.0f;         //重力加速度
	float max_fall_speed = 0.0f;     //最大落下速度
	bool on_ground = false;           //地面にいるかどうか


	int jump_count = 0;

	bool damage_flg = false;	//ダメージを受けたかどうか

	int animation_count = 0;    //アニメーションカウント

	std::vector<GameObject*> hit_objects;
public:
	//初期化処理
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//更新処理
	virtual void Update()override;
	//描画処理
	virtual void Draw(Vector2D offset, double rate) const override;
	//終了時処理
	virtual void Finalize()override;

	virtual void OnHitCollision(GameObject* hit_object)override;

	bool IsOnGround() const { return on_ground; }

	//重力適用
	virtual void ApplyGravity(); 

	void SetOnGround(bool flg) { on_ground = flg; }
};

