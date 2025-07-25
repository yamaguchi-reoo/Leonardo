#pragma once
#include "../GameObject.h"
class MoveBlock :
    public GameObject
{
private:
    float move_range_right = 0.0f;
    float move_range_left = 0.0f;
public:
    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) override;
    //終了時処理
    void Finalize()override;

    //当たった時の挙動
    void OnHitCollision(GameObject* hit_object)override;
};

