/*============================================================
*	@file	 : Title.h
*	@brief	 : タイトルシーン
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/03/29
*	@updated : 2026/08/15
*============================================================*/
#pragma once

#include "Scene.h"

/*============================================================
*	@class	: Title
*	@brief	: タイトルシーン（Sceneを継承）
*============================================================*/
class Title : public Scene
{
public:
	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;
};