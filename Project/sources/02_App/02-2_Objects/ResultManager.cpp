/*============================================================
*	@file	 : ResultManager.cpp
*	@brief	 : リザルトシーン制御用ダミーオブジェクト
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/07
*	@updated : 2026/09/07
*============================================================*/
#include "ResultManager.h"
#include "SceneManager.h"
#include "Transition.h"
#include "Title.h"
#include "Input.h"

void ResultManager::Initialize()
{
	// トランジション処理
	Transition::getInstance().Start(0.5, true);
}

void ResultManager::Finalize()
{
	GameObject::Finalize();
}

void ResultManager::Update(double deltaTime)
{
	// シーン遷移処理
	if (!Transition::getInstance().GetTransitionActive() && Input::GetKeyTrigger(VK_RETURN)) {
		Transition::getInstance().Start(0.5, false);
		mTransitionWait = true;
	}

	if (mTransitionWait && !Transition::getInstance().GetTransitionActive()) {
		mTransitionWait = false;
		SceneManager::getInstance().SceneChange<Title>();
	}

	GameObject::Update(deltaTime);
}

void ResultManager::Draw() const
{
	GameObject::Draw();
}

void ResultManager::AudioPlay(std::string key)
{
	// キーから登録オーディオを検索して再生する
	auto it = _mResultAudios.find(key);

	if (it != _mResultAudios.end()) {
		_mResultAudios[key]->Play();
	}
}