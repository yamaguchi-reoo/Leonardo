#pragma once
#include "DxLib.h"

#define D_KEYCODE_MAX (256)     //�L�[�R�[�h�̍ő吔�i256�L�[�j
#define MOUSE_MAX (256)         //�}�E�X�{�^���̍ő吔

#define BUTTON_MAX 16           //�R���g���[���[�{�^���̍ő吔
#define STICK_X 1               //�X�e�B�b�N��X��
#define STICK_Y 2               //�X�e�B�b�N��Y��
#define STICK_TILT_MAX 32767.0f //�X�e�B�b�N�̍ő�X���l�iXInput�d�l�j

//�}�E�X�J�[�\���̍��W�������\����
struct Cursor
{
    int x; //�}�E�X�J�[�\����X���W
    int y; //�}�E�X�J�[�\����Y���W
};

//�L�[�{�[�h�E�}�E�X�E�R���g���[���̓��͊Ǘ��N���X�i�V���O���g���j
class InputControl
{
private:
    //�N���X�̗B��̃C���X�^���X�i�V���O���g���p�^�[���j
    static InputControl* instance;

    //���݂̃L�[���͏��
    char now_key[D_KEYCODE_MAX];
    //�O��̃L�[���͏��
    char old_key[D_KEYCODE_MAX];

    //���݂̃}�E�X���͏��
    char now_mouse[MOUSE_MAX];
    //�O��̃}�E�X���͏��
    char old_mouse[MOUSE_MAX];

    //�}�E�X�J�[�\���̍��W
    Cursor cursor;

    //���݂̃R���g���[���[�{�^�����͏��
    char now_button[BUTTON_MAX];
    //�O��̃R���g���[���[�{�^�����͏��
    char old_button[BUTTON_MAX];

    //XInput�f�o�C�X�̓��͏��
    XINPUT_STATE input;

private:
    //�R���X�g���N�^�iprivate�ɂ��邱�ƂŊO������̃C���X�^���X������h���j
    InputControl() = default;

    //�R�s�[�K�[�h�i�R�s�[�֎~�j
    InputControl(InputControl& v) = delete;
    InputControl& operator = (const InputControl& v) = delete;

public:
    /// @brief �f�X�g���N�^�i�V���O���g���Ȃ̂Ńf�t�H���g�j
    ~InputControl() = default;

public:
    //InputControl �̃C���X�^���X
    static InputControl* GetInstance();

    //�C���X�^���X�̍폜�i�V���O���g���̔j�������j
    static void DeleteInstance();

public:
    //���͏�Ԃ��X�V����
    void Update();

    //�L�[�{�[�h���͏���
    //�L�[��������Ă����
    bool GetKey(int key_code) const;

    //�L�[�������ꂽ�u��
    bool GetKeyDown(int key_code) const;

    //�L�[�������ꂽ�u��
    bool GetKeyUp(int key_code) const;

    // �}�E�X���͏���
    //�}�E�X�{�^����������Ă����
    bool GetMouse(int mouse);

    //�}�E�X�{�^���������ꂽ�u��
    bool GetMouseDown(int mouse);

    //�}�E�X�{�^���������ꂽ�u��
    bool GetMouseUp(int mouse);

    //�}�E�X�J�[�\���̍��W���擾
    Cursor GetMouseCursor();

    //�R���g���[�����͏���
    //�R���g���[���̃{�^����������Ă����
    bool GetButton(int button) const;

    //�R���g���[���̃{�^���������ꂽ�u��
    bool GetButtonDown(int button) const;

    //�R���g���[���̃{�^���������ꂽ�u��
    bool GetButtonUp(int button) const;

    //���X�e�B�b�N�̌X�����擾
    float LeftStickTilt(short l_stick);

    //�E�X�e�B�b�N�̌X�����擾
    float RightStickTilt(short r_stick);

private:
    //�L�[�R�[�h���L���Ȕ͈͂ɂ��邩���m�F
    static bool CheckKeyCodeRange(int key_code);
};


