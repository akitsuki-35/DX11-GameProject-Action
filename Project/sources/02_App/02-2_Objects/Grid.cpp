/*============================================================
*	@file	 : Grid.cpp
*	@brief	 : グリッド
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/08/20
*	@updated : 2026/08/20
*============================================================*/
#include "Grid.h"
#include "GridRenderer.h"
#include <d3d11.h>

void Grid::Initialize()
{
	// グリッド描画用レンダラーのセット
	GridRenderer* renderer = AddComponent<GridRenderer>(this);

	// グリッドサイズ設定
	renderer->Set(20, 20, 5.0f);

	// カラー設定
	renderer->SetColor({ 0.2f, 0.4f, 1.0f, 1.0f });

	// 加算合成
	renderer->SetBlendState(Blend::Add);

	// シェーダーセット
	renderer->LoadShader("Unlit");
}

void Grid::Finalize()
{
	GameObject::Finalize();
}

void Grid::Update(double deltaTime)
{
	GameObject::Update(deltaTime);
}

void Grid::Draw() const
{
	GameObject::Draw();
}