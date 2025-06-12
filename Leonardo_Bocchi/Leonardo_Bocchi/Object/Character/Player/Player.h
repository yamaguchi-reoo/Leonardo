#pragma once
#include "../CharaBase.h"
#include "../../../Utility/InputControl.h"
#include "HealParticle.h"
#include "../../Gimmick/GoalParticle.h"

#include <map>
#include <vector>

//�v���C���[�̍s����Ԃ�\���񋓌^
enum class ActionState { IDLE, WALK, JUMP, DAMAGE };

//�ړ������̍\����
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

    //�v���C���[�̌����i�ړ������j���Ǘ�
    enum class MoveDirection { NONE, LEFT, RIGHT };

    ActionState action_state = ActionState::JUMP;   // ���݂̍s�����
    MoveDirection move = MoveDirection::NONE;       // ���݂̈ړ�����


    bool is_invincible;
    int invincible_timer;

    bool is_goal;

    std::map<ActionState,std::vector<int>> animation_data;

    // �e�s����Ԃ̃A�j���[�V�����̃t���[����
	std::map<ActionState, int> animation_frame_count;

	int animation_frame = 0; //���݂̃A�j���[�V�����t���[��

    //�񕜃p�[�e�B�N���̊Ǘ��z��
    std::vector<HealParticle> heal_particles;


	bool is_teleport = false; // �e���|�[�g�����ǂ���
	int teleport_timer = 0; // �e���|�[�g�̃^�C�}�[
    std::vector<GoalParticle> teleport_particles;

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

    // �_���[�W���󂯂��ۂ̏���
    void ApplyDamage();

    // �v���C���[�摜�̓ǂݍ��ݏ���
    void LoadPlayerImage();

    // ���G��Ԃ̃G�t�F�N�g�`��
    void InvincibleEffect(Vector2D offset);

    // �񕜃p�[�e�B�N���̍X�V����
    void UpdateHealParticle(HealParticle& particle);

    // �񕜃p�[�e�B�N���̕`�揈��
    void DrawHealParticle(const HealParticle& particle, Vector2D offset);

    // �v���C���[���S�[�������ۂ̏���
    void PlayerToGoal();

    // �v���C���[�̃e���|�[�g�����J�n
    void PlayerTeleport();

    // �e���|�[�g�����̍X�V�i�^�C�}�[�Ǘ��Ȃǁj
    void UpdateTeleport();

    // �e���|�[�g�G�t�F�N�g�̕`��
    void DrawTeleport(Vector2D offset);
};

