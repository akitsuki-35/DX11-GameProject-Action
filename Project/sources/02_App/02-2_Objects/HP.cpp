/*============================================================
*	@file	 : HP.cpp
*	@brief	 : スコア表示
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/06/30
*	@updated : 2026/08/12
*============================================================*/
#include "HP.h"
#include "TextRenderer.h"
#include "FontLoader.h"
#include "Game.h"
#include "Player.h"
#include <format>

void HP::Initialize()
{
	_mHPRenderer = AddComponent<TextRenderer>(this);

	// トランスフォームの初期化
	mTransform = Transform(
		{ 100.0f, 900.0f, 0.0f },
		{ 0.0f, 0.0f,  0.0f },
		{ 2.0f, 2.0f,  1.0f }
	);

	// フォントの設定
	_mHPRenderer->SetFont("Kaisotai")
		->SetTextSize(100)->SetShadowColor({0.0f, 0.5f, 1.0f, 1.0f})->SetColor({ 0.5f, 1.0f, 0.3f, 1.0f })
		->LoadShader("Font");

	_mTextRenderer = AddComponent<TextRenderer>(this);

	// フォントの設定
	_mTextRenderer->SetFont("Kaisotai")->SetText("%")->SetOffset({ 140.0f, 30.0f })
		->SetTextSize(60)->SetShadowColor({ 0.0f, 0.5f, 1.0f, 1.0f })->SetColor({ 0.5f, 1.0f, 0.3f, 1.0f })
		->LoadShader("Font");
}

void HP::Finalize()
{
	GameObject::Finalize();
}

void HP::Update(double deltaTime)
{
	std::string hp = std::format("{:0}", Game::GetGameObject<Player>()->GetHP());

	_mHPRenderer->SetText(hp);

	GameObject::Update(deltaTime);
}

void HP::Draw() const
{
	GameObject::Draw();
}