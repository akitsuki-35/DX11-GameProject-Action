/*============================================================
*	@file	 : Game.cpp
*	@brief	 : ゲームシーン
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/03/28
*	@updated : 2026/08/04
*============================================================*/
#include "Game.h"

// マネージャー
#include "GameManager.h"

// オブジェクト群
#include "Camera.h"
#include "Grid.h"
#include "Player.h"
#include "Bullet.h"
#include "Sky.h"
#include "ScreenFilter.h"
#include "Score.h"
#include "HP.h"

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
	if (mSlow) {
		deltaTime *= 0.5;
	}

	Scene::Update(deltaTime);
}

void Game::Draw() const
{
	Scene::Draw();
}