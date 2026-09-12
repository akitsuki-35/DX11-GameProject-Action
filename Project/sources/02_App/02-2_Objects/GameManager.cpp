/*============================================================
*	@file	 : GameManager.cpp
*	@brief	 : ゲーム制御用ダミーオブジェクト
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/07
*	@updated : 2026/09/07
*============================================================*/
#include "GameManager.h"
#include "SceneManager.h"
#include "Transition.h"
#include "Game.h"
#include "Result.h"
#include "Camera.h"
#include "Timer.h"
#include "ParticleEmitter.h"
#include "Input.h"

void GameManager::Initialize()
{
	// トランジション処理
	Transition::getInstance().Start(0.5, true);

	_mGameAudios.clear();

	// BGM読み込み・再生
	mBGMVolume = 0.05f;
	AudioPlayer* bgm = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Stage.ogg")->SetVolume(mBGMVolume);
	_mGameAudios.emplace("BGM", bgm);

	_mGameAudios["BGM"]->Play();

	// SE読み込み
	AudioPlayer* shot = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Shot.mp3")->SetVolume(0.1f);
	_mGameAudios.emplace("Shot", shot);

	AudioPlayer* hit = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Hit.mp3")->LoadAudio("assets\\audio\\Hit.mp3")->SetVolume(0.1f);
	_mGameAudios.emplace("Hit", hit);

	// タイマー
	_mHitStop = AddComponent<Timer>(this);
	_mSceneChangeTimer = AddComponent<Timer>(this);

	// ステージ上のエフェクト
	_mEffect = Game::AddGameObject<ParticleEmitter>()->LoadCSV("assets\\csv\\Effect.csv");
}

void GameManager::Finalize()
{
	GameObject::Finalize();
}

void GameManager::Update(double deltaTime)
{
	// ステージエフェクト更新
	stageEffectUpdate();

	// シーン遷移処理
	// 1.遷移条件を満たしたら遷移までのウェイトタイマーをセット
	if (mEnemyCount == 0 && !mTransitionWait && !_mSceneChangeTimer->GetEnable()) {
		_mSceneChangeTimer->Start(1.5);
	}

	// 2.ウェイトタイマーが時間切れならフェードアウト処理に移行
	if (_mSceneChangeTimer->GetEnable()) {
		if (_mSceneChangeTimer->IsTimeUp()) {
			Transition::getInstance().Start(2.5, false);
			mTransitionWait = true;
		}
	}

	// 3.フェードアウトが完了したらシーン遷移
	if (mTransitionWait && !Transition::getInstance().GetTransitionActive()) {
		mTransitionWait = false;
		GameManager::SetSlow(false);
		SceneManager::getInstance().SceneChange<Result>();
	}

	// BGMのフェードアウト処理
	if (mTransitionWait) {
		float volume = mBGMVolume * Transition::getInstance().GetTransitionProgress();
		_mGameAudios["BGM"]->SetVolume(volume);
	}

	// ヒットストップが終了していたら、シーン側に終了を伝える
	if (_mHitStop->IsTimeUp()) {
		Game::SetHitStop(false);
	}

	GameObject::Update(deltaTime);
}

void GameManager::Draw() const
{
	GameObject::Draw();
}

void GameManager::AudioPlay(std::string key)
{
	// キーから登録オーディオを検索して再生する
	auto it = _mGameAudios.find(key);

	if (it != _mGameAudios.end()) {
		_mGameAudios[key]->Play();
	}
}

void GameManager::SetHitStop(double time)
{
	_mHitStop->Start(time);

	// シーン側にヒットストップ状態を伝える
	Game::SetHitStop(true);
}

bool GameManager::IsHitStop()
{
	return _mHitStop->GetEnable();
}

void GameManager::SetSlow(bool isSlow)
{
	// シーン側にスローモーション状態を伝える
	Game::SetSlow(isSlow);
}

void GameManager::SceneChangeWait(double time)
{
	_mSceneChangeTimer->Start(time);
}

void GameManager::stageEffectUpdate()
{
	// カメラ座標取得
	auto camera = Game::GetGameObject<Camera>();
	Vector3 position = camera->GetPosition();

	// forwardを反転して後方を取得
	Vector3 back = -camera->GetTransform().GetForward();
	position += back * 30.0f;
	_mEffect->SetPosition(position);

	// プレイヤーの後方からエフェクト用パーティクルを発射
	// 後方→前方に向けて発射
	Vector3 velocity = _mEffect->GetDesc().Velocity;
	_mEffect->SetAccel(-back * 100.0f);
}