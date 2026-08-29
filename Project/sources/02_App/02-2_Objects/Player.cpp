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

#include "Tree.h"
#include "Box.h"

void Player::Initialize()
{
	mTransform = Transform(
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.5f, 1.5f, 1.5f }
	);		

	mVelocity = { 0.0f, 0.0f, 0.0f };
	mAccel = { 50.0f, 0.0f, 50.0f };

	// コンポーネント読込
	AddComponent<ModelRenderer>(this)->LoadModel("assets\\models\\Player.fbx")->
		LoadShader("Directional");
	AddComponent<Animator>(this)->Set("Take 001");

	mSE = AddComponent<AudioPlayer>(this)->LoadAudio("assets\\audio\\wan.wav");
}

void Player::Finalize()
{
	GameObject::Finalize();
}

void Player::Update(double deltaTime)
{
	float dt = static_cast<float>(deltaTime);

	float j = 15.0f; // ジャンプ力
	float g = 30.0f; // 重力加速度
	float r = 5.0f; // 抵抗力

	Vector3 position = mTransform.GetPosition();
	Vector3 rotation = mTransform.GetRotation();
	Vector3 scale = mTransform.GetScale();

	Vector3 oldPosition = mTransform.GetPosition(); // プレイヤー移動前座標

	Camera* camera = Game::GetGameObject<Camera>();
	Vector3 forward = camera->GetForward();
	Vector3 right = camera->GetRight();

	forward.y = 0.0f;
	forward.Normalize();

	right.y = 0.0f;
	right.Normalize();

	// キー入力移動処理
	if (Input::GetKeyPress('D')) {
		mVelocity += right * 50.0f * dt;
	}
	if (Input::GetKeyPress('A')) {
		mVelocity -= right * 50.0f * dt;
	}
	if (Input::GetKeyPress('W')) {
		mVelocity += forward * 50.0f * dt;
	}
	if (Input::GetKeyPress('S')) {
		mVelocity -= forward * 50.0f * dt;
	}

	float yaw = atan2f(mVelocity.x, mVelocity.z);
	yaw += DirectX::XM_PI;
	rotation.y = yaw;

	// ジャンプ
	if (mGround) {
		if (Input::GetKeyTrigger('K')) {
			mVelocity.y += j; // 撃力

			//スケールアニメーション
			mTransform.SetScale({ 0.75f, 2.0f, 0.75f });
			scale.y *= 2.0f;
			scale.x *= 0.75f;
			scale.z *= 0.75f;

			mSE->Play();
		}
	}

	// スケールを元に戻す
	scale.x += (1.5f - scale.x) * 0.1f;
	scale.y += (1.5f - scale.y) * 0.1f;
	scale.z += (1.5f - scale.z) * 0.1f;

	// 重力加速度
	mVelocity.y += -g * dt;

	// 摩擦抵抗
	mVelocity.x += -mVelocity.x * r * dt;
	mVelocity.z += -mVelocity.z * r * dt;

	// 移動処理
	position += mVelocity * dt;

	bool oldGround = mGround;
	mGround = false;

	// 地面との衝突判定
	if (position.y < 0.0f) {
		position.y = 0.0f;
		mVelocity.y = 0.0f;
		mGround = true;
	}

	if (!oldGround && mGround) {
		// スケールアニメーション
		scale.y *= 0.5f;
		scale.x *= 1.5f;
		scale.z *= 1.5f;
	}

	// スケールを元に戻す
	scale.x += (1.5f - scale.x) * 0.1f;
	scale.y += (1.5f - scale.y) * 0.1f;
	scale.z += (1.5f - scale.z) * 0.1f;

	// 弾の発射
	if (Input::GetKeyTrigger('J')) {

		Bullet* bullet = Game::AddGameObject<Bullet>();
		bullet->SetPosition(mTransform.GetPosition());
		bullet->SetVelocity(mTransform.GetForward() * 50.0f);
	}
	
	// 移動アニメーション
	if (mGround) {
		mMoveAnimation += mVelocity.Length() * dt;
		scale.y += sinf(mMoveAnimation * 3.0f) * 0.03f;
	}

	mTransform.SetPosition(position);
	mTransform.SetRotation(rotation);
	mTransform.SetScale(scale);

	GameObject::Update(deltaTime);
	GetComponent<Animator>()->Update(deltaTime);
}

void Player::Draw() const
{
	GameObject::Draw(); // 継承元のDrawを呼び出す
}