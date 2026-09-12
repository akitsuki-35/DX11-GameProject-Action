/*============================================================
*	@file	 : Enemy.cpp
*	@brief	 : エネミー
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/05/19
*	@updated : 2026/08/04
*============================================================*/
#include "Enemy.h"
#include "Game.h"
#include "Player.h"
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

	// エネミーカウントを増加
	GameManager::AddEnemy();
}

void Enemy::Finalize()
{
	GameObject::Finalize();
}

void Enemy::Update(double deltaTime)
{
	if (GameManager::IsHitStop()) return;

	// dtをfloatに変換
	float dt = static_cast<float>(deltaTime);

	// 抵抗力
	float r = 5.0f;

	// 現在の座標と回転を取得
	Vector3 position = mTransform.GetPosition();
	Vector3 rotation = mTransform.GetRotation();

	// オブジェクト正面を取得
	Vector3 forward = mTransform.GetForward();

	// yの値は使わないので0に
	forward.y = 0.0f;
	forward.Normalize();

	// プレイヤーオブジェクトと座標を取得
	auto player = Game::GetGameObject<Player>();
	Vector3 playerPosition = player->GetPosition();

	// 自身からプレイヤーへの距離を取得
	Vector3 dir = playerPosition - position;
	float length = dir.Length();

	float angleY = atan2f(dir.x, dir.z);

	// 正規化
	if (length > 0.0f) {
		dir.Normalize();
	}

	// プレイヤー方向へ移動
	mVelocity += forward * 50.0f * dt;

	float angle = angleY - rotation.y;
	float PI = 3.14159265f;

	while (angle > PI) angle -= PI * 2.0f;
	while (angle < -PI) angle += PI * 2.0f;

	// 旋回
	if (angle > 0.05f) {
		rotation.y += 2.0f * dt;
		rotation.z = std::max(rotation.z - 2.0f * dt, -1.0f);
	}
	else if (angle < -0.05f) {
		rotation.y -= 2.0f * dt;
		rotation.z = std::min(rotation.z + 2.0f * dt, 1.0f);
	}
	else {
		// 傾きを戻す
		if (rotation.z > 0.0f) rotation.z = std::max(rotation.z - 2.0f * dt, 0.0f);
		else if (rotation.z < 0.0f) rotation.z = std::min(rotation.z + 2.0f * dt, 0.0f);
	}

	// 摩擦抵抗
	mVelocity.x += -mVelocity.x * r * dt;
	mVelocity.z += -mVelocity.z * r * dt;

	// 移動処理
	position += mVelocity * dt;

	// 敵同士の衝突処理
	GameManager::EnemyCollision(*this, position, dt);

	// シェイク処理
	if (_mShakeTimer->GetEnable()) {
		shakeUpdate(position);
	}

	// 座標クランプ
	GameManager::ClampPosition(position);
	
	// 座標と回転をセット
	mTransform.SetPosition(position);
	mTransform.SetRotation(rotation);

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

//Enemy* Enemy::PlayerLook()
//{
//	float angleY = 0.0f;
//
//	auto player = Game::GetGameObject<Player>();
//	Vector3 playerPosition = player->GetPosition();
//
//	if (fabsf(playerPosition.x) < 0.001f && fabsf(playerPosition.z) < 0.001f) {
//		Vector3 position = mTransform.GetPosition();
//		
//		if (position.x < 0.0f && position.z < 0.0f) {
//			angleY = atan2f(position.x, position.z);
//		}
//		else {
//			angleY = atan2f(-position.x, -position.z);
//		}
//	}
//	else {
//		Vector3 dir = playerPosition - mTransform.GetPosition();
//		angleY = atan2f(dir.x, dir.z);
//	}
//
//	mTransform.SetRotation({ 0.0f, angleY, 0.0f });
//
//	return this;
//}

void Enemy::shakeUpdate(Vector3& position)
{
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
}