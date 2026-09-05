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

	mTransform.SetPosition({ 0.0f, 0.0f, 0.0f });

	renderer->SetFont("Kaisotai")->
		SetText("あのイーハトーヴォのすきとおった風、夏でも底に冷たさをもつ青いそら、うつくしい森で飾られたモリーオ市、郊外のぎらぎらひかる草の波")
		->SetTextSize(48.0f)->SetShadowColor({0.0f, 0.5f, 1.0f, 1.0f})->SetColor({0.5f, 1.0f, 0.3f, 1.0f})
		->LoadShader("Font");

	//UIRenderer* renderer = AddComponent<UIRenderer>(this);

	//renderer->GetCanvas().CreateCanvas(UIStyle::Pivot::LeftTop);

	//mTransform.SetPosition({ 0.0f, 0.0f, 0.0f });
	//mTransform.SetScale({ 1920.0f, 1080.0f, 0.0f });

	//renderer->LoadTexture("assets\\textures\\filter.png")->LoadShader("UI");

	mValue = 0;
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