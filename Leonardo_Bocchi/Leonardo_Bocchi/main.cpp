#include "DxLib.h"
#include "Utility/UtilityList.h"
#include "Scene/SceneManager.h"
#include "common.h"

#define FRAMERATE 60.0 //フレームレート


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // ウインドウのサイズ

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);


	SceneManager* manager = nullptr;

	//fps制御
	FpsController* FPSC = new FpsController(FRAMERATE, 800);
	ResourceManager* rm = ResourceManager::GetInstance();

	try {
		manager = new SceneManager();

		manager->Initialize();

		InputControl* input = InputControl::GetInstance();

		while (ProcessMessage() != -1)
		{
			//入力情報の更新
			input->Update();

			//画面の初期化
			ClearDrawScreen();

			//画面の更新
			manager->Update();


			FPSC->All();
#ifdef _DEBUG
			FPSC->Disp();
#endif

			if (input->GetKeyUp(KEY_INPUT_ESCAPE))
			{
				break;
			}
			ScreenFlip();
		}

	}
	catch (std::string& error_text)
	{
		OutputDebugString(error_text.c_str());

		return -1;
	}

	//インスタンスの削除
	InputControl::DeleteInstance();
	rm->DeleteInstance();
	ResourceManager::DeleteInstance();

	if (manager != nullptr)
	{
		manager->Finalize();
		delete manager;
	}

	DxLib_End();

	return 0;

}