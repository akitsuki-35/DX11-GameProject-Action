/*============================================================
*	@file	 : GameManager.h
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

/*------------------------------------------------------------
	前方宣言
------------------------------------------------------------*/
class Timer;
class ParticleEmitter;

/*============================================================
*	@class	: GameManager
*	@brief	: ゲーム制御用クラス
*============================================================*/
class GameManager : public GameObject
{
private:
	// ゲーム内BGM・SE
	static inline std::unordered_map<std::string, AudioPlayer*> _mGameAudios{};
	
	// エフェクト
	ParticleEmitter* _mEffect{ nullptr };

	// ヒットストップ
	static inline Timer* _mHitStop{ nullptr };

	// トランジション中フラグ
	static inline bool mTransitionWait{ false };

private:
	// コピー禁止
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator=(GameManager&&) = delete;

public:
	GameManager() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;

	// オーディオ再生
	static void AudioPlay(std::string key);

	// ヒットストップ
	static void SetHitStop(double time);
	static bool IsHitStop();

private:
	// ステージエフェクト更新
	void stageEffectUpdate();
};