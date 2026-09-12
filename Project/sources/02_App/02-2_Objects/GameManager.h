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
class GameObject;

/*============================================================
*	@class	: GameManager
*	@brief	: ゲーム制御用クラス
*============================================================*/
class GameManager : public GameObject
{
private:
	// ウェーブ数
	static inline int mWave{ 0 };

	// 敵の数
	static inline int mEnemyCount{ 0 };

	// ゲーム内BGM・SE
	static inline std::unordered_map<std::string, AudioPlayer*> _mGameAudios{};
	
	// BGM音量
	static inline float mBGMVolume{};
	
	// エフェクト
	static inline ParticleEmitter* _mEffect{ nullptr };

	// ヒットストップ
	static inline Timer* _mHitStop{ nullptr };

	// トランジション中フラグ
	static inline bool mTransitionWait{ false };

	// 敵全滅～シーン遷移までのタイマー
	static inline Timer* _mSceneChangeTimer{ nullptr };

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

	// 簡易エネミー当たり判定
	static void EnemyCollision(GameObject& other, Vector3& position, float dt);

	// 座標クランプ
	static void ClampPosition(Vector3& position);

	// ウェーブ数取得
	static int GetWave() { return mWave; }

	// エネミーカウント関連
	static int GetEnemyCount() { return mEnemyCount; }
	static void AddEnemy() { mEnemyCount++; }
	static void ReduceEnemy() { mEnemyCount--; }

	// オーディオ再生
	static void AudioPlay(std::string key);

	// ヒットストップ
	static void SetHitStop(double time);
	static bool IsHitStop();

	// スローモーション
	static void SetSlow(bool isSlow);

	// シーン遷移中かを判定
	static bool IsTransition();

private:
	void enemySpawn();

	// ステージエフェクト更新
	void stageEffectUpdate();
};