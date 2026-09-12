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
#include "Enemy.h"
#include "ParticleEmitter.h"
#include "Timer.h"

using namespace DirectX;

void GameManager::Initialize()
{
	// トランジション処理
	Transition::getInstance().Start(0.5, true);

	_mGameAudios.clear();

	// ウェーブ数初期化
	mWave = 1;

	// BGM読み込み・再生
	mBGMVolume = 0.05f;
	AudioPlayer* bgm = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Stage.ogg")->SetVolume(mBGMVolume);
	_mGameAudios.emplace("BGM", bgm);

	_mGameAudios["BGM"]->Play();

	// SE読み込み
	AudioPlayer* shot = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Shot.mp3")->SetVolume(0.1f);
	_mGameAudios.emplace("Shot", shot);

	AudioPlayer* hit = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Hit.mp3")->SetVolume(0.25f);
	_mGameAudios.emplace("Hit", hit);

	AudioPlayer* destroy = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\Destroy.mp3")->SetVolume(0.15f);
	_mGameAudios.emplace("Destroy", destroy);

	// タイマー
	_mHitStop = AddComponent<Timer>(this);
	_mSceneChangeTimer = AddComponent<Timer>(this);

	// ステージ上のエフェクト
	_mEffect = Game::AddGameObject<ParticleEmitter>()->LoadCSV("assets\\csv\\Effect.csv");

	enemySpawn();
}

void GameManager::Finalize()
{
	GameObject::Finalize();
}

void GameManager::Update(double deltaTime)
{
	// ステージエフェクト更新
	stageEffectUpdate();

	// ウェーブカウント増加と敵配置
	if (mEnemyCount == 0 && _mHitStop->IsTimeUp()) {
		mWave++;
		enemySpawn();
	}

	// シーン遷移処理
	// 1.遷移条件を満たしたら遷移までのウェイトタイマーをセット
	if (mWave == 6 && !mTransitionWait && !_mSceneChangeTimer->GetEnable()) {
		GameManager::SetSlow(true);
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

void GameManager::EnemyCollision(GameObject& other, Vector3& position, float dt)
{
	auto enemies = Game::GetGameObjects<Enemy>();
	for (auto enemy : enemies) {
		if (enemy == dynamic_cast<Enemy*>(&other)) continue;

		// 相手との距離を計算
		Vector3 otherDir = position - enemy->GetPosition();
		float distance = otherDir.Length();

		// 半径の合計
		float min = 4.0f;

		if (distance < min && distance > 0.0f) {
			otherDir.Normalize();

			// めり込んでいる距離を計算
			float overlap = min - distance;

			// 押し出し
			position += otherDir * overlap * 5.0f * dt;
		}
	}
}

void GameManager::ClampPosition(Vector3& position)
{
	// 座標のクランプ（グリッドの外には出られないようにする）
	if (position.x < -50.0f) {
		position.x = -50.0f;
	}
	else if (position.x > 50.0f) {
		position.x = 50.0f;
	}

	if (position.z < -50.0f) {
		position.z = -50.0f;
	}
	else if (position.z > 50.0f) {
		position.z = 50.0f;
	}

	// y座標を0に補正
	if (position.y != 0.0f) {
		position.y = 0.0f;
	}
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

bool GameManager::IsTransition()
{
	return mTransitionWait || _mSceneChangeTimer->GetEnable();
}

void GameManager::enemySpawn()
{
	switch (mWave)
	{
	case 1:
		Game::AddGameObject<Enemy>()->SetPosition({ -25.0f, 0.0f, 45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(135.0f), 0.0f});
		Game::AddGameObject<Enemy>()->SetPosition({ 25.0f, 0.0f, 45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(-135.0f), 0.0f });
		break;

	case 2:
		Game::AddGameObject<Enemy>()->SetPosition({ -30.0f, 0.0f, 30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(135.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ -40.0f, 0.0f, 45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(135.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ -45.0f, 0.0f, 25.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(135.0f), 0.0f });
		break;

	case 3:
		Game::AddGameObject<Enemy>()->SetPosition({ 0.0f, 0.0f, 30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(180.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ -25.0f, 0.0f, 45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(180.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ 25.0f, 0.0f, 45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(180.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ 0.0f, 0.0f, -45.0f }).
			SetRotation({ 0.0f, 0.0f, 0.0f });
		break;

	case 4:
		Game::AddGameObject<Enemy>()->SetPosition({ -30.0f, 0.0f, -30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(45.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ -45.0f, 0.0f, -45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(45.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ 30.0f, 0.0f, 30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(-135.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ 45.0f, 0.0f, 45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(-135.0f), 0.0f });
		break;

	case 5:
		Game::AddGameObject<Enemy>()->SetPosition({ 0.0f, 0.0f, 45.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(180.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ -45.0f, 0.0f, 30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(135.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ 45.0f, 0.0f, 30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(-135.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ -30.0f, 0.0f, -30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(45.0f), 0.0f });
		Game::AddGameObject<Enemy>()->SetPosition({ 30.0f, 0.0f, -30.0f }).
			SetRotation({ 0.0f, XMConvertToRadians(-45.0f), 0.0f });
		break;

	default:
		break;
	}
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