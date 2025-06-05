#include "Fps.h"

void FpsController::Init(float RefreshRate, int UpdateTime) {
    FrameTime = (int)(1000.0f / RefreshRate); //1�t���[���̎��Ԃ̌v�Z
    UpdateTime = UpdateTime;
    WaitTime = 0;
    LastTime = NowTime = 0;
    Count = 0.0f;
    Fps = 0.0f;
    LastUpdate = 0;
}

void FpsController::Wait() {
    NowTime = GetNowCount();
    WaitTime = FrameTime - (NowTime - LastTime);
    if (WaitTime > 0) { //�҂����Ԃ������
        WaitTimer(WaitTime); //�w�莞�ԑ҂�
    }
    LastTime = GetNowCount();
}

float FpsController::Get() {
    Count += 1.0f;
    if (UpdateTime < (LastTime - LastUpdate)) { //�A�b�v�f�[�g���ԂɂȂ��Ă����
        Fps = Count / (float)(LastTime - LastUpdate) * 1000.0f; //FPS�l�̌v�Z
        LastUpdate = LastTime;
        Count = 0.0f;
    }
    return (Fps);
}
