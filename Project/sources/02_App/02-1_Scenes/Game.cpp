/*============================================================
*	@file	 : Game.cpp
*	@brief	 : ゲームシーン
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/03/28
*	@updated : 2026/08/04
*============================================================*/
#include "Game.h"
#include "SceneManager.h"
#include "Input.h"
#include "Camera.h"
#include "Transition.h"

#include "GameMode.h"

#include "Grid.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Sky.h"

#include "ParticleEmitter.h"
#include "Result.h"

#include "ScreenFilter.h"
#include "Score.h"

#include "DeviceManager.h"
#include "D3D11Config.h"

void Game::Initialize()
{
	// トランジション処理
	Transition::getInstance().Start(1.0, true);

	// 配列を初期化
	_mGameObjects.clear();

	// カメラ
	AddGameObject<Camera>();

	// スカイドーム
	AddGameObject<Sky>();

	// グリッド（フィールド）
	AddGameObject<Grid>()->SetPosition({ 0.0f, 0.0f, 0.0f });

	// オブジェクト
	AddGameObject<Player>();
	AddGameObject<Enemy>()->SetPosition({ 5.0f, 0.0f, 5.0f });
	AddGameObject<Enemy>()->SetPosition({ -5.0f, 0.0f, 5.0f });
	AddGameObject<Enemy>()->SetPosition({ 0.0f, 0.0f, 5.0f });

	// ステージ上のエフェクト
	_mEffect = AddGameObject<ParticleEmitter>()->LoadCSV("assets\\csv\\Effect.csv");

	// 2Dオブジェクト
	AddGameObject<ScreenFilter>();
	AddGameObject<Score>();

	// ゲームモード（制御用ダミーオブジェクト）
	AddGameObject<GameMode>();
}

void Game::Finalize()
{
	Scene::Finalize();
}

void Game::Update(double deltaTime)
{
	Scene::Update(deltaTime);
	
	// ステージエフェクト更新
	stageEffectUpdate();
	
	if (Input::GetKeyTrigger(VK_RETURN)) {
		SceneManager::getInstance().SceneChange<Result>();
	}
}

void Game::Draw() const
{
	Scene::Draw();
}

void Game::stageEffectUpdate()
{
	// プレイヤー座標取得
	auto player = GetGameObject<Player>();
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