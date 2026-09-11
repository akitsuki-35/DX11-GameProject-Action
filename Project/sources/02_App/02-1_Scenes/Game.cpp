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
#include "Camera.h"

#include "GameManager.h"

#include "Grid.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Sky.h"

#include "ParticleEmitter.h"
#include "Result.h"

#include "ScreenFilter.h"
#include "Score.h"
#include "HP.h"

#include "DeviceManager.h"
#include "D3D11Config.h"

void Game::Initialize()
{
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

	// 2Dオブジェクト
	AddGameObject<ScreenFilter>();
	AddGameObject<Score>();
	AddGameObject<HP>();

	// マネージャー（制御用ダミーオブジェクト）
	AddGameObject<GameManager>();
}

void Game::Finalize()
{
	Scene::Finalize();
}

void Game::Update(double deltaTime)
{
	Scene::Update(deltaTime);
}

void Game::Draw() const
{
	Scene::Draw();
}