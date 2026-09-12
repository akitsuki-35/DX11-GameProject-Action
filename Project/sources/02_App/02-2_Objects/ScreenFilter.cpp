/*============================================================
*	@file	 : ScreenFilter.cpp
*	@brief	 : 画面フィルター
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/07
*	@updated : 2026/09/07
*============================================================*/
#include "ScreenFilter.h"
#include "UIRenderer.h"
#include "Config.h"

void ScreenFilter::Initialize()
{
	// UIキャンバスのセット
	UIRenderer* renderer = AddComponent<UIRenderer>(this);
	renderer->GetCanvas().CreateCanvas(UIStyle::Pivot::Center);
	
	// 四隅を黒くする
	renderer->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	// 座標とスケールをセット
	mTransform.SetPosition({ Screen::CENTER_X, Screen::CENTER_Y, 0.0f });
	mTransform.SetScale({ Screen::WIDTH, Screen::HEIGHT, 0.0f });

	// テクスチャのセット
	renderer->LoadTexture("assets\\textures\\filter.png")->LoadShader("UI");
}

void ScreenFilter::Finalize()
{
	GameObject::Finalize();
}

void ScreenFilter::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

void ScreenFilter::Draw() const
{
	GameObject::Draw();
}
