/*============================================================
*	@file	 : GameMode.cpp
*	@brief	 : ゲーム制御用ダミーオブジェクト
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/07
*	@updated : 2026/09/07
*============================================================*/
#include "GameMode.h"

void GameMode::Initialize()
{
	// BGM読み込み・再生
	AudioPlayer* bgm = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Stage.ogg")->SetVolume(0.05f);
	_mGameAudios.emplace("BGM", bgm);

	_mGameAudios["BGM"]->Play();

	// SE読み込み
	AudioPlayer* shot = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Shot.mp3")->SetVolume(0.1f);
	_mGameAudios.emplace("Shot", shot);

	AudioPlayer* hit = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Hit.mp3")->LoadAudio("assets\\audio\\Hit.mp3")->SetVolume(0.1f);
	_mGameAudios.emplace("Hit", hit);
}

void GameMode::Finalize()
{
	GameObject::Finalize();
}

void GameMode::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

void GameMode::Draw() const
{
	GameObject::Draw();
}

void GameMode::AudioPlay(std::string key)
{
	// キーから登録オーディオを検索して再生する
	auto it = _mGameAudios.find(key);

	if (it != _mGameAudios.end()) {
		_mGameAudios[key]->Play();
	}
}