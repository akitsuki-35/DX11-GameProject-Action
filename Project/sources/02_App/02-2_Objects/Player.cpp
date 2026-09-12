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
#include "GameManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "Input.h"
#include "ModelRenderer.h"
#include "Timer.h"

void Player::Initialize()
{
	// トランスフォームの初期化
	mTransform = Transform(
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f }
	);

	// 移動量と加速度の初期化
	mVelocity = { 0.0f, 0.0f, 0.0f };
	mAccel = { 0.0f, 0.0f, 0.0f };

	// モデル・シェーダー読み込み
	AddComponent<ModelRenderer>(this)->LoadModel("assets\\models\\Player.fbx")->
		LoadTexture("Roughness.jpg", ModelRenderer::TextureType::Roughness)->
		LoadTexture("Metalness.jpg", ModelRenderer::TextureType::Metalness)->
		SetParameter({ 0.2f, 0.8f, 1.0f, 0.0f })->
		LoadShader("PBR");

	_mShotInterval = AddComponent<Timer>(this);
}

void Player::Finalize()
{
	GameObject::Finalize();
}

void Player::Update(double deltaTime)
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
	else if (Input::GetKeyPress('B')) {

	}

	// カメラ方向にプレイヤーを向ける
	float yaw = atan2f(mVelocity.x, mVelocity.z);
	rotation.y = yaw;

	// 摩擦抵抗
	mVelocity.x += -mVelocity.x * r * dt;
	mVelocity.z += -mVelocity.z * r * dt;

	// 移動処理
	position += mVelocity * dt;

	// 敵との衝突処理
	GameManager::EnemyCollision(*this, position, dt);

	// 座標クランプ
	GameManager::ClampPosition(position);

	// 弾の発射
	if(!_mShotInterval->GetEnable() && !GameManager::IsTransition())
	if (Input::GetKeyPress(VK_SPACE)) {
		GameManager::AudioPlay("Shot");
		Bullet* bullet = Game::AddGameObject<Bullet>();
		bullet->SetPosition(mTransform.GetPosition());
		bullet->SetVelocity(forward * 100.0f);

		// インターバルのセット
		_mShotInterval->Start(0.2);
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