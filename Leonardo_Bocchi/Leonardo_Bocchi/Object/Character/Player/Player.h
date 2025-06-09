#pragma once
#include "../CharaBase.h"
#include <vector>
#include "../../../Utility/InputControl.h"
#include <map>

enum class ActionState { IDLE, WALK, JUMP, DAMAGE };

struct PlayerMoveRecord : public MoveRecord
{
    ActionState action_state;
};

class Player :
    public CharaBase
{
private:
    //std::vector<int> animation_data;//�A�j���[�V�����f�[�^

	std::vector<PlayerMoveRecord> move_history; //�ړ�����

    int jump_time = 0;

    //�_���[�W���󂯂Ă��鎞��
    int damage_timer;

    //��ԊǗ�
    //enum class ActionState { IDLE, WALK, JUMP, DAMAGE };
    enum class MoveDirection { NONE, LEFT, RIGHT };

    ActionState action_state = ActionState::IDLE;
    MoveDirection move = MoveDirection::NONE;


    bool is_invincible;
    int invincible_timer;


    std::map<ActionState,std::vector<int>> animation_data;
	std::map<ActionState, int> animation_frame_count;
	int animation_frame = 0; //���݂̃A�j���[�V�����t���[��


public:
    Player();
    ~Player();

    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) override;
    //�I��������
    void Finalize()override;

public:

    void ApplyDeceleration();
    void ConstrainVelocity();
    

    void HandleInput();

    //�A�j���[�V�����Ǘ�
    void AnimationControl();
    //�����������̋���
    void OnHitCollision(GameObject* hit_object)override;

	//�ړ�������ۑ�
	void SaveMoveHistory();

	//�ړ��������擾
    const std::vector<PlayerMoveRecord>& GetMoveHistory() const {
        return move_history;
    }

    void ApplyDamage();

    void LoadPlayerImage();

	void InvincibleEffect(Vector2D offset);

    void DrawEllipseAA(float cx, float cy, float rx, float ry, int num_segments, int color, bool fill, int line_thickness = 1);
};

