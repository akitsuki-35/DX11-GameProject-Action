/*============================================================
*	@file	 : Score.h
*	@brief	 : スコア表示
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/06/30
*	@updated : 2026/08/12
*============================================================*/
#pragma once

#include "GameObject.h"

/*------------------------------------------------------------
	前方宣言
------------------------------------------------------------*/
class Timer;
class TextRenderer;

/*============================================================
*	@class	: Score
*	@brief	: スコア
*============================================================*/
class Score : public GameObject
{
private:
	int mScore{};
	TextRenderer* _mTextRenderer{ nullptr };
	TextRenderer* _mScoreRenderer{ nullptr };

public:
	Score() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;
	void Draw() const override;

	void AddScore(int add) { mScore += add; }
};