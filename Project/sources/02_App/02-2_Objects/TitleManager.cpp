/*============================================================
*	@file	 : TitleManager.cpp
*	@brief	 : タイトルシーン制御用ダミーオブジェクト
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/07
*	@updated : 2026/09/07
*============================================================*/
#include "TitleManager.h"
#include "SceneManager.h"
#include "Transition.h"
#include "Game.h"
#include "Input.h"

void TitleManager::Initialize()
{
	// トランジション処理
	Transition::getInstance().Start(1.0, true);
}

void TitleManager::Finalize()
{
	GameObject::Finalize();
}

void TitleManager::Update(double deltaTime)
{
	// シーン遷移処理
	if (!Transition::getInstance().GetTransitionActive() && Input::GetKeyTrigger(VK_RETURN)) {
		Transition::getInstance().Start(1.0, false);
		mTransitionWait = true;
	}

	if (mTransitionWait && !Transition::getInstance().GetTransitionActive()) {
		mTransitionWait = false;
		SceneManager::getInstance().SceneChange<Game>();
	}

	GameObject::Update(deltaTime);
}

void TitleManager::Draw() const
{
	GameObject::Draw();
}

void TitleManager::AudioPlay(std::string key)
{
	// キーから登録オーディオを検索して再生する
	auto it = _mTitleAudios.find(key);

	if (it != _mTitleAudios.end()) {
		_mTitleAudios[key]->Play();
	}
}