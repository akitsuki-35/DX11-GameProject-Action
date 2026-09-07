/*============================================================
*	@file	 : GameMode.h
*	@brief	 : ゲーム制御用クラス
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/07
*	@updated : 2026/09/07
*============================================================*/
#pragma once

#include "GameObject.h"
#include "AudioPlayer.h"
#include <unordered_map>

/*============================================================
*	@class	: GameMode
*	@brief	: ゲーム制御用クラス
*============================================================*/
class GameMode : public GameObject
{
private:
	// ゲーム内BGM・SE
	static inline std::unordered_map<std::string, AudioPlayer*> _mGameAudios{};

public:
	GameMode() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;

	// オーディオ再生
	static void AudioPlay(std::string key);
};