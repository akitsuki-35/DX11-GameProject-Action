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

void Score::Initialize()
{
	TextRenderer* renderer = AddComponent<TextRenderer>(this);

	// トランスフォームの初期化
	mTransform = Transform(
		{ 75.0f, 60.0f, 0.0f },
		{ 0.0f, 0.0f,  0.0f },
		{ 1.0f, 1.0f,  1.0f }
	);

	// 仮表示用テキスト
	std::string score = "000000";

	// フォントの設定
	renderer->SetFont("Kaisotai")->
		SetText("Score  " + score)
		->SetTextSize(64.0f)->SetShadowColor({0.0f, 0.5f, 1.0f, 1.0f})->SetColor({0.5f, 1.0f, 0.3f, 1.0f})
		->LoadShader("Font");
}

void Score::Finalize()
{
	GameObject::Finalize();
}

void Score::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

void Score::Draw() const
{
	GameObject::Draw();
}