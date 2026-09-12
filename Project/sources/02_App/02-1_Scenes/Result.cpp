/*============================================================
*	@file	 : Result.cpp
*	@brief	 : リザルトシーン
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/03/29
*	@updated : 2026/08/15
*============================================================*/
#include "Result.h"
#include "ResultManager.h"
#include "BackGroundTexture.h"
#include "Timer.h"

void Result::Initialize()
{
	_mGameObjects.clear();

	AddGameObject<BackGroundTexture>()->LoadTexture("assets\\textures\\result.png");

	// マネージャー（制御用ダミーオブジェクト）
	AddGameObject<ResultManager>();
}

void Result::Finalize()
{
	Scene::Finalize();
}

void Result::Update(double deltaTime)
{
	Scene::Update(deltaTime);
}

void Result::Draw() const
{
	Scene::Draw();
}