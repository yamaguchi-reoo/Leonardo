#include "InputControl.h"

//�ÓI�����o�ϐ���`
InputControl* InputControl::instance = nullptr; //�N���X�̃C���X�^���X�̃|�C���^

//���͐���N���X�̃C���X�^���X�擾���鏈��
InputControl* InputControl::GetInstance()
{
	//�C���X�^���X��������΁A��������
	if (instance == nullptr)
	{
		instance = new InputControl();
	}

	//�������g�Ń|�C���^��ԋp����
	return instance;
}

//���͐���N���X�̃C���X�^���X�폜���鏈��
void InputControl::DeleteInstance()
{
	//�C���X�^���X�����݂��Ă���΁A�폜����
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void InputControl::Update()
{
	//�O��̃L�[�̍X�V
	memcpy(old_key, now_key, (sizeof(char) * D_KEYCODE_MAX));

	//���݂̃L�[��Ԃ̍X�V
	GetHitKeyStateAll(now_key);

	//�O��̃L�[�̍X�V
	memcpy(old_mouse, now_mouse, (sizeof(char) * MOUSE_MAX));

	//���݂̃L�[��Ԃ̍X�V
	for (int i = 0; i < MOUSE_MAX; i++)
	{
		if (GetMouseInput() == i)
		{
			now_mouse[i] = 1;
		}
		else
		{
			now_mouse[i] = 0;
		}
	}
	GetMousePoint(&cursor.x, &cursor.y);


	//�O��̃{�^���̍X�V
	memcpy(old_button, now_button, (sizeof(char) * BUTTON_MAX));
	//���݂̃R���g���[���[�̃{�^���̏�Ԃ��擾
	if (GetJoypadXInputState(DX_INPUT_KEY_PAD1, &input) == 0)
	{
		for (int i = 0; i < BUTTON_MAX; i++)
		{
			now_button[i] = input.Buttons[i];
		}
	}
}

bool InputControl::GetKey(int key_code)const
{
	//�L�[��������Ă��邩���m�F���鏈��
	return CheckKeyCodeRange(key_code) && ((now_key[key_code] == TRUE) && (old_key[key_code] == TRUE));
}

bool InputControl::GetKeyDown(int key_code)const
{
	//�L�[�������ꂽ�u�Ԃ��m�F���鏈��
	return CheckKeyCodeRange(key_code) && ((now_key[key_code] == TRUE) && (old_key[key_code] == FALSE));
}

bool InputControl::GetKeyUp(int key_code)const
{
	//�L�[�������ꂽ�u�Ԃ��m�F���鏈��
	return CheckKeyCodeRange(key_code) && ((now_key[key_code] == FALSE) && (old_key[key_code] == TRUE));
}

bool InputControl::GetMouse(int mouse)
{
	//�{�^����������Ă��邩���m�F���鏈��
	return (now_mouse[mouse] == TRUE) && (old_mouse[mouse] == TRUE);
}

bool InputControl::GetMouseDown(int mouse)
{
	//�{�^���������ꂽ�u�Ԃ��m�F���鏈��
	return (now_mouse[mouse] == TRUE) && (old_mouse[mouse] == FALSE);
}

bool InputControl::GetMouseUp(int mouse)
{
	//�{�^���������ꂽ�u�Ԃ��m�F���鏈��
	return (now_mouse[mouse] == FALSE) && (old_mouse[mouse] == TRUE);
}

Cursor InputControl::GetMouseCursor()
{
	//�J�[�\����Ԃ�
	return cursor;
}

bool InputControl::GetButton(int button) const
{
	//�{�^����������Ă��邩���m�F���鏈��
	return ((now_button[button] == TRUE) && (old_button[button] == TRUE));
}

bool InputControl::GetButtonDown(int button) const
{
	//�{�^���������ꂽ�u�Ԃ��m�F���鏈��
	return ((now_button[button] == TRUE) && (old_button[button] == FALSE));
}

bool InputControl::GetButtonUp(int button) const
{
	//�{�^���������ꂽ�u�Ԃ��m�F���鏈��
	return ((now_button[button] == FALSE) && (old_button[button] == TRUE));
}

float InputControl::LeftStickTilt(short l_stick)
{
	// ���X�e�B�b�N�̌X��������Ԃ�
	if (l_stick == STICK_X)
	{
		//X���̌X�����擾
		return static_cast<float>(input.ThumbLX) / STICK_TILT_MAX;
	}
	else if (l_stick == STICK_Y)
	{
		//Y���̌X�����擾
		return static_cast<float>(input.ThumbLY) / STICK_TILT_MAX;
	}
	return 0.0f;
}

float InputControl::RightStickTilt(short r_stick)
{
	// �E�X�e�B�b�N�̌X��������Ԃ�
	if (r_stick == STICK_X)
	{
		//X���̌X�����擾
		return static_cast<float>(input.ThumbRX) / STICK_TILT_MAX;
	}
	else if (r_stick == STICK_Y)
	{
		//Y���̌X�����擾
		return static_cast<float>(input.ThumbRY) / STICK_TILT_MAX;
	}
	return 0.0f;
}

bool InputControl::CheckKeyCodeRange(int key_code)
{
	//�L�[�R�[�h�͈͓̔����m�F���鏈��
	return (0 <= key_code && key_code < D_KEYCODE_MAX);
}