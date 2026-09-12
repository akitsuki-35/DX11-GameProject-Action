/*============================================================
*	@file	 : Score.cpp
*	@brief	 : スコア表示
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/06/30
*	@updated : 2026/08/12
*============================================================*/
#include "Score.h"
#include "TextRenderer.h"
#include "FontLoader.h"
#include "Timer.h"
#include <format>

void Score::Initialize()
{
	_mTextRenderer = AddComponent<TextRenderer>(this);

	// トランスフォームの初期化
	mTransform = Transform(
		{ 75.0f, 60.0f, 0.0f },
		{ 0.0f, 0.0f,  0.0f },
		{ 1.0f, 1.0f,  1.0f }
	);

	// フォントの設定
	_mTextRenderer->SetFont("Kaisotai")->SetText("score")
		->SetTextSize(54)->SetShadowColor({0.0f, 0.5f, 1.0f, 1.0f})->SetColor({0.5f, 1.0f, 0.3f, 1.0f})
		->LoadShader("Font");

	_mScoreRenderer = AddComponent<TextRenderer>(this);

	// フォントの設定
	_mScoreRenderer->SetFont("Kaisotai")->SetOffset({ 150.0f, -10.0f })
		->SetTextSize(64)->SetShadowColor({ 0.0f, 0.5f, 1.0f, 1.0f })->SetColor({ 0.5f, 1.0f, 0.3f, 1.0f })
		->LoadShader("Font");
}

void Score::Finalize()
{
	GameObject::Finalize();
}

void Score::Update(double deltaTime)
{
	std::string score = std::format("{:06}", mScore);
	_mScoreRenderer->SetText(score);

	GameObject::Update(deltaTime);
}

void Score::Draw() const
{
	GameObject::Draw();
}