#include "DxLib.h"
#include "SceneManager.h"
#include "GameScene/GameMainScene.h"
#include "GameScene/TitleScene.h"
#include "GameScene/ResultScene.h"


SceneManager::SceneManager() :current_scene(nullptr)
{
}

SceneManager::~SceneManager()
{
	this->Finalize();
}

void SceneManager::Initialize()
{
	ChangeScene(eSceneType::TITLE);
}

void SceneManager::Update()
{
	eSceneType next_scene_type = current_scene->Update();

	current_scene->Draw();

	if (next_scene_type != current_scene->GetNowSceneType())
	{
		ChangeScene(next_scene_type);
	}
}

void SceneManager::Finalize()
{
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

void SceneManager::Draw() const
{
}

void SceneManager::ChangeScene(eSceneType type)
{
	SceneBase* new_scene = CreateScene(type);

	//エラーチェック
	if (new_scene == nullptr)
	{
		throw("\n新しいシーンの生成が出来ませんでした\n");
	}

	//現在のシーンの終了処理
	if (current_scene != nullptr)
	{
		new_scene->SetClearCount(current_scene->GetClearCount());

		current_scene->Finalize();
		delete current_scene;
	}

	//新しいシーンの初期化
	new_scene->Initialize();
	current_scene = new_scene;
}

SceneBase* SceneManager::CreateScene(eSceneType type)
{

	switch (type)
	{
	case eSceneType::TITLE:
		return dynamic_cast<SceneBase*>(new TitleScene());
	case eSceneType::GAME_MAIN:
		return dynamic_cast<SceneBase*>(new GameMainScene());
	case eSceneType::RESULT:
		return dynamic_cast<SceneBase*>(new ResultScene());
	default:
		return nullptr;
	}
}

