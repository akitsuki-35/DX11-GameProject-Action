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
#include "Player.h"
#include "Timer.h"
#include "ParticleEmitter.h"
#include "Input.h"

void GameManager::Initialize()
{
	// トランジション処理
	Transition::getInstance().Start(1.0, true);

	_mGameAudios.clear();

	// BGM読み込み・再生
	AudioPlayer* bgm = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Stage.ogg")->SetVolume(0.05f);
	_mGameAudios.emplace("BGM", bgm);

	_mGameAudios["BGM"]->Play();

	// SE読み込み
	AudioPlayer* shot = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Shot.mp3")->SetVolume(0.1f);
	_mGameAudios.emplace("Shot", shot);

	AudioPlayer* hit = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Hit.mp3")->LoadAudio("assets\\audio\\Hit.mp3")->SetVolume(0.1f);
	_mGameAudios.emplace("Hit", hit);

	_mHitStop = AddComponent<Timer>(this);

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
	if (!Transition::getInstance().GetTransitionActive() && Input::GetKeyTrigger(VK_RETURN)) {
		Transition::getInstance().Start(1.0, false);
		mTransitionWait = true;
	}

	if (mTransitionWait && !Transition::getInstance().GetTransitionActive()) {
		mTransitionWait = false;
		SceneManager::getInstance().SceneChange<Result>();
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

void GameManager::stageEffectUpdate()
{
	// プレイヤー座標取得
	auto player = Game::GetGameObject<Player>();
	Vector3 position = player->GetPosition();

	// forwardを反転して後方を取得
	Vector3 back = -player->GetTransform().GetForward();
	position += back * 30.0f;
	_mEffect->SetPosition(position);

	// プレイヤーの後方からエフェクト用パーティクルを発射
	// 後方→前方に向けて発射
	Vector3 velocity = _mEffect->GetDesc().Velocity;
	_mEffect->SetAccel(-back * 100.0f);
}