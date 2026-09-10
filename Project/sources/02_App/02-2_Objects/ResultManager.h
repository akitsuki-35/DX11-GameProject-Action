/*============================================================
*	@file	 : ResultManager.h
*	@brief	 : リザルトシーン制御用クラス
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
*	@class	: ResultManager
*	@brief	: リザルトシーン制御用クラス
*============================================================*/
class ResultManager : public GameObject
{
private:
	// タイトルBGM・SE
	static inline std::unordered_map<std::string, AudioPlayer*> _mResultAudios{};

	// トランジション中フラグ
	static inline bool mTransitionWait{ false };

private:
	// コピー禁止
	ResultManager(const ResultManager&) = delete;
	ResultManager& operator=(const ResultManager&) = delete;
	ResultManager(ResultManager&&) = delete;
	ResultManager& operator=(ResultManager&&) = delete;

public:
	ResultManager() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;

	// オーディオ再生
	static void AudioPlay(std::string key);
};