/*============================================================
*	@file	 : Player.cpp
*	@brief	 : プレイヤー
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/05/19
*	@updated : 2026/08/04
*============================================================*/
#include "Player.h"
#include "Input.h"
#include "ModelRenderer.h"
#include "Animator.h"
#include "Game.h"
#include "AudioPlayer.h"
#include "Camera.h"
#include "Bullet.h"

void Player::Initialize()
{
	mTransform = Transform(
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f }
	);		

	mVelocity = { 0.0f, 0.0f, 0.0f };
	mAccel = { 50.0f, 0.0f, 50.0f };

	// コンポーネント読込
	AddComponent<ModelRenderer>(this)->LoadModel("assets\\models\\Player.obj")->
		LoadShader("Directional");
}

void Player::Finalize()
{
	GameObject::Finalize();
}

void Player::Update(double deltaTime)
{
	float dt = static_cast<float>(deltaTime);

	float g = 30.0f; // 重力加速度
	float r = 5.0f; // 抵抗力

	Vector3 position = mTransform.GetPosition();
	Vector3 rotation = mTransform.GetRotation();
	Vector3 scale = mTransform.GetScale();

	Camera* camera = Game::GetGameObject<Camera>();
	Vector3 forward = camera->GetForward();

	forward.y = 0.0f;
	forward.Normalize();

	mVelocity += forward * 50.0f * dt;

	// キー入力移動処理
	if (Input::GetKeyPress(VK_RIGHT)) {
		rotation.z = std::max(rotation.z - 1.5f * dt, -1.0f);
	}
	else if (Input::GetKeyPress(VK_LEFT)) {
		rotation.z = std::min(rotation.z + 1.5f * dt, 1.0f);
	}
	else {
		if (rotation.z > 0.0f) rotation.z = std::max(rotation.z - 1.5f * dt, 0.0f);
		else if (rotation.z < 0.0f) rotation.z = std::min(rotation.z + 1.5f * dt, 0.0f);
	}

	if (Input::GetKeyPress('S')) {
		mVelocity -= forward * 30.0f * dt;
	}

	float yaw = atan2f(mVelocity.x, mVelocity.z);
	rotation.y = yaw;

	// 重力加速度
	mVelocity.y += -g * dt;

	// 摩擦抵抗
	mVelocity.x += -mVelocity.x * r * dt;
	mVelocity.z += -mVelocity.z * r * dt;

	// 移動処理
	position += mVelocity * dt;

	// 地面との衝突判定
	if (position.y < 0.0f) {
		position.y = 0.0f;
		mVelocity.y = 0.0f;
		mGround = true;
	}

	// 座標のクランプ
	if (position.x < -50.0f) {
		position.x = -50.0f;
	}
	else if (position.x > 50.0f) {
		position.x = 50.0f;
	}

	if (position.z < -50.0f) {
		position.z = -50.0f;
	}
	else if (position.z > 50.0f) {
		position.z = 50.0f;
	}

	// 弾の発射
	if (Input::GetKeyTrigger('J')) {

		Bullet* bullet = Game::AddGameObject<Bullet>();
		bullet->SetPosition(mTransform.GetPosition());
		bullet->SetVelocity(mTransform.GetForward() * 50.0f);
	}

	mTransform.SetPosition(position);
	mTransform.SetRotation(rotation);
	mTransform.SetScale(scale);

	GameObject::Update(deltaTime);
}

void Player::Draw() const
{
	GameObject::Draw(); // 継承元のDrawを呼び出す
}