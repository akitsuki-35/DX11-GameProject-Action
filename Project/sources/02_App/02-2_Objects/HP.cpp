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
#include "Timer.h"
#include <format>

void HP::Initialize()
{
	_mRenderer = AddComponent<TextRenderer>(this);

	// トランスフォームの初期化
	mTransform = Transform(
		{ 500.0f, 60.0f, 0.0f },
		{ 0.0f, 0.0f,  0.0f },
		{ 2.0f, 2.0f,  1.0f }
	);

	// 仮表示用テキスト
	std::string HP = "000000";

	// フォントの設定
	_mRenderer->SetFont("Kaisotai")->
		SetText("HP")
		->SetTextSize(98)->SetShadowColor({0.0f, 0.5f, 1.0f, 1.0f})->SetColor({0.5f, 1.0f, 0.3f, 1.0f})
		->LoadShader("Font");
}

void HP::Finalize()
{
	GameObject::Finalize();
}

void HP::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

void HP::Draw() const
{
	GameObject::Draw();
}