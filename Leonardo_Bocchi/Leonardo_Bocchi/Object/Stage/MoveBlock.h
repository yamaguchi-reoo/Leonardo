#pragma once
#include "../GameObject.h"
class MoveBlock :
    public GameObject
{
private:
    float move_range_right = 0.0f;
    float move_range_left = 0.0f;
public:
    //‰Šú‰»ˆ—
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //XVˆ—
    void Update()override;
    //•`‰æˆ—
    void Draw(Vector2D offset, double rate) const override;
    //I—¹ˆ—
    void Finalize()override;

    //“–‚½‚Á‚½‚Ì‹““®
    void OnHitCollision(GameObject* hit_object)override;
};

