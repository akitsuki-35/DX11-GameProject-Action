/*============================================================
*	@file	 : BackGroundTexture.h
*	@brief	 : 背景テクスチャ
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/10
*	@updated : 2026/09/10
*============================================================*/
#pragma once

#include "GameObject.h"
#include "Vector2.h"

/*------------------------------------------------------------
	前方宣言
------------------------------------------------------------*/
class UIRenderer;

/*============================================================
*	@class	: BackGroundTexture
*	@brief	: 背景テクスチャ
*============================================================*/
class BackGroundTexture : public GameObject
{
private:
	UIRenderer* _mRenderer{ nullptr };

public:
	BackGroundTexture() = default;
	virtual ~BackGroundTexture() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;

	BackGroundTexture* LoadTexture(const char* fileName);
	BackGroundTexture* SetSize(const Vector2& size);
};