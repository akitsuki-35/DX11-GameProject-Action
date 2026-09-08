/*============================================================
*	@file	 : Camera.h
*	@brief	 : カメラオブジェクト
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/04/26
*	@updated : 2026/08/04
*============================================================*/
#pragma once

#include "GameObject.h"
#include <DirectXMath.h>

/*------------------------------------------------------------
	前方宣言
------------------------------------------------------------*/
class Timer;

/*============================================================
*	@class	: Camera
*	@brief	: カメラ
*============================================================*/
class Camera : public GameObject
{
protected:
	Vector3 mTarget{}; // 注視点
	DirectX::XMMATRIX mViewMatrix{}; // ビュー行列

	Timer* _mShakeTimer{ nullptr }; // カメラシェイク用タイマー
	float mShakeIntensity{}; // シェイク強度
	
	// シェイク方向
	bool mShakeX{ true };
	bool mShakeY{ true };

public:
	virtual ~Camera() = default;
	void Initialize() override;
	void Finalize() override;
	void Update(double deltaTime) override;

	DirectX::XMMATRIX GetViewMatrix() const { return mViewMatrix; }

	Vector3 GetForward() const;

	Vector3 GetRight() const;

	// カメラ行列のセット
	void SetMatrix() const;

	void Shake(float intensity, double shakeTime = 1.0);

private:
	void shakeUpdate();
};