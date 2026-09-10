/*============================================================
*	@file	 : TitleManager.h
*	@brief	 : タイトルシーン制御用クラス
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
*	@class	: TitleManager
*	@brief	: タイトルシーン制御用クラス
*============================================================*/
class TitleManager : public GameObject
{
private:
	// タイトルBGM・SE
	static inline std::unordered_map<std::string, AudioPlayer*> _mTitleAudios{};

	// トランジション中フラグ
	static inline bool mTransitionWait{ false };

private:
	// コピー禁止
	TitleManager(const TitleManager&) = delete;
	TitleManager& operator=(const TitleManager&) = delete;
	TitleManager(TitleManager&&) = delete;
	TitleManager& operator=(TitleManager&&) = delete;

public:
	TitleManager() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;

	// オーディオ再生
	static void AudioPlay(std::string key);
};