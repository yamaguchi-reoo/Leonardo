#include "SceneBase.h"
#include "../common.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	this->Finalize();
}

void SceneBase::Initialize()
{
}

eSceneType SceneBase::Update()
{
	for (GameObject* obj : objects)
	{
		//if (camera_location.x + obj->GetBoxSize().x && camera_location.x <= 800) {
		obj->Update();
		//}
	}


	//削除フラグが立っているオブジェクトを削除
	for (auto it = objects.begin(); it != objects.end();)
	{
		//フラグが立っているのなら
		if ((*it)->GetIsDelete()) {
			//削除処理
			DeleteObject(*it);
			//削除フラグが立っているオブジェクトを削除
			it = objects.begin();
		}
		else
		{
			++it;
		}
	}

	// 二重ループで衝突判定
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = i + 1; j < objects.size(); j++)
		{
			if (objects[i]->CheckBoxCollision(objects[j]))
			{
				objects[i]->OnHitCollision(objects[j]);
				objects[j]->OnHitCollision(objects[i]);

				//ゴールに到達した場合
				if (objects[i]->GetObjectType() == PLAYER && objects[j]->GetObjectType() == GOAL)
				{
					clear_count++;
					//ステージ再生成		
					SetStageReload(true);
				}
			}

		}
	}

	return GetNowSceneType();
}

void SceneBase::Draw() const
{
	for (GameObject* obj : objects)
	{
		Vector2D draw_location = obj->GetLocation() - camera_location;
		obj->Draw(draw_location, 1.0);
	}
}

void SceneBase::Finalize()
{
	//オブジェクトが空の場合は処理をスキップ
	if (objects.empty())
	{
		return;
	}

	for (GameObject* obj : objects)
	{
		obj->Finalize();

		delete obj;
	}

	objects.clear();
}

void SceneBase::DeleteObject(GameObject* obj)
{
	if (obj == nullptr) return;

	auto it = std::find(objects.begin(), objects.end(), obj);

	if (it != objects.end()) {
		//見つかった場合、オブジェクトを削除
		(*it)->Finalize(); //終了処理
		delete* it;         //メモリ解放
		objects.erase(it);  //リストから削除
	}
}

