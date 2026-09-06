/*============================================================
*	@file	 : Player.cpp
*	@brief	 : プレイヤー
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/05/19
*	@updated : 2026/09/06
*============================================================*/
#include "Player.h"
#include "Game.h"
#include "Camera.h"
#include "Bullet.h"
#include "Input.h"
#include "ModelRenderer.h"
#include "AudioPlayer.h"

void Player::Initialize()
{
	// トランスフォームの初期化
	mTransform = Transform(
		{ 0.0f, 0.0f, -50.0f },
		{ 0.0f, 0.0f,  0.0f },
		{ 1.0f, 1.0f,  1.0f }
	);		

	// 移動量と加速度の初期化
	mVelocity = { 0.0f, 0.0f, 0.0f };
	mAccel = { 0.0f, 0.0f, 0.0f };

	// モデル・シェーダー読み込み
	AddComponent<ModelRenderer>(this)->LoadModel("assets\\models\\Player.fbx")->
		LoadShader("Directional");
}

void Player::Finalize()
{
	GameObject::Finalize();
}

void Player::Update(double deltaTime)
{
	// dtをfloatに変換
	float dt = static_cast<float>(deltaTime);

	// 抵抗力
	float r = 5.0f;

	// 現在の座標と回転を取得
	Vector3 position = mTransform.GetPosition();
	Vector3 rotation = mTransform.GetRotation();

	// オブジェクト正面を取得
	Camera* camera = Game::GetGameObject<Camera>();
	Vector3 forward = camera->GetForward();

	// yの値は使わないので0に
	forward.y = 0.0f;
	forward.Normalize();

	// 自動で正面に前進
	mVelocity += forward * 100.0f * dt;

	// キー入力で旋回
	if (Input::GetKeyPress(VK_RIGHT)) {
		rotation.z = std::max(rotation.z - 2.0f * dt, -1.0f);
	}
	else if (Input::GetKeyPress(VK_LEFT)) {
		rotation.z = std::min(rotation.z + 2.0f * dt, 1.0f);
	}
	else {
		// 入力がない時は傾きを戻す
		if (rotation.z > 0.0f) rotation.z = std::max(rotation.z - 2.0f * dt, 0.0f);
		else if (rotation.z < 0.0f) rotation.z = std::min(rotation.z + 2.0f * dt, 0.0f);
	}

	// 下キーでスピードダウン
	if (Input::GetKeyPress(VK_DOWN)) {
		mVelocity -= forward * 30.0f * dt;
	}

	// カメラ方向にプレイヤーを向ける
	float yaw = atan2f(mVelocity.x, mVelocity.z);
	rotation.y = yaw;

	// 摩擦抵抗
	mVelocity.x += -mVelocity.x * r * dt;
	mVelocity.z += -mVelocity.z * r * dt;

	// 移動処理
	position += mVelocity * dt;

	// 座標のクランプ（グリッドの外には出られないようにする）
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
	if (Input::GetKeyTrigger(VK_SPACE)) {

		Bullet* bullet = Game::AddGameObject<Bullet>();
		bullet->SetPosition(mTransform.GetPosition());
		bullet->SetVelocity(mTransform.GetForward() * 100.0f);
	}

	// 座標と回転をセット
	mTransform.SetPosition(position);
	mTransform.SetRotation(rotation);

	GameObject::Update(deltaTime);
}

void Player::Draw() const
{
	GameObject::Draw();
}