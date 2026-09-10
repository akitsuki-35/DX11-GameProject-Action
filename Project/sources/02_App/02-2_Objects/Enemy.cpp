/*============================================================
*	@file	 : Enemy.cpp
*	@brief	 : エネミー
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/05/19
*	@updated : 2026/08/04
*============================================================*/
#include "Enemy.h"
#include "Timer.h"
#include "GameManager.h"
#include "ModelRenderer.h"

void Enemy::Initialize()
{
	mTransform = Transform(
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f }
	);

	mVelocity = { 0.0f, 0.0f, 0.0f };
	mAccel = { 0.0f, 0.0f, 0.0f };

	// モデル読込
	AddComponent<ModelRenderer>(this)->LoadModel("assets\\models\\Enemy.fbx")->
		LoadTexture("Roughness.jpg", ModelRenderer::TextureType::Roughness)->
		LoadTexture("Metalness.jpg", ModelRenderer::TextureType::Metalness)->
		SetParameter({ 0.2f, 0.8f, 1.0f, 0.0f })->
		LoadShader("PBR");

	// シェイク用タイマーのセット
	_mShakeTimer = AddComponent<Timer>(this);
}

void Enemy::Finalize()
{
	GameObject::Finalize();
}

void Enemy::Update(double deltaTime)
{
	if (GameManager::IsHitStop()) return;

	// シェイク処理
	if (_mShakeTimer->GetEnable()) {
		shakeUpdate();
	}

	GameObject::Update(deltaTime);
}

void Enemy::Draw() const
{
	GameObject::Draw(); // 継承元のDrawを呼び出す
}

void Enemy::Damage()
{
	mLife--;

	if (mLife <= 0) {
		SetDestroy();
	}
}

void Enemy::Shake(float intensity, double shakeTime)
{
	// 揺れの強さをセット
	mShakeIntensity = intensity;

	// タイマーをセット
	_mShakeTimer->Start(shakeTime);
}

void Enemy::shakeUpdate()
{
	// 現在位置を取得
	Vector3 position = mTransform.GetPosition();

	// タイマーの進行度に応じて揺れの強さを算出
	float progress = _mShakeTimer->GetProgress();
	float intensity = mShakeIntensity * progress;
	float angle = static_cast<float>(_mShakeTimer->GetTime()) * 50.0f;
	float shakeX = intensity * cosf(angle);
	float shakeY = intensity * sinf(angle);

	// 座標に揺れを加算
	position.x += shakeX;
	position.y += shakeY;

	// 現在時間が0なら揺れの強さを0にする
	if (_mShakeTimer->IsTimeUp()) {
		mShakeIntensity = 0.0f;
	}

	// 座標を差し戻し
	mTransform.SetPosition(position);
}