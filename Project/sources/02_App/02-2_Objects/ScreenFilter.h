/*============================================================
*	@file	 : ScreenFilter.h
*	@brief	 : 画面フィルター
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/09/07
*	@updated : 2026/09/07
*============================================================*/
#pragma once

#include "GameObject.h"

/*============================================================
*	@class	: ScreenFilter
*	@brief	: 画面フィルター
*============================================================*/
class ScreenFilter : public GameObject
{
public:
	ScreenFilter() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;
};