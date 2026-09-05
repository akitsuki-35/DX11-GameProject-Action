/*============================================================
*	@file	 : Bullet.cpp
*	@brief	 : 弾
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/06/02
*	@updated : 2026/09/06
*============================================================*/
#include "Bullet.h"
#include "Game.h"
#include "Enemy.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "ModelRenderer.h"
#include "ParticleRenderer.h"

void Bullet::Initialize()
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
	
	// 軌跡用パーティクルのセット
	_mEmitter = Game::AddGameObject<ParticleEmitter>();
	_mEmitter->LoadCSV("assets\\csv\\Bullet.csv");

	// モデル・シェーダー読み込み
	AddComponent<ModelRenderer>(this)->LoadModel("assets\\models\\bullet.obj")->LoadShader("Unlit");
}

void Bullet::Finalize()
{
	GameObject::Finalize();
}

void Bullet::Update(double deltaTime)
{
	// dtをfloatに変換
	float dt = static_cast<float>(deltaTime);

	// 現在の座標を取得
	Vector3 position = mTransform.GetPosition();

	// 弾の直進
	position += mVelocity * dt;

	// オブジェクト背面取得
	Vector3 back = -mTransform.GetForward();

	// オブジェクト背面に向けてパーティクル発射
	_mEmitter->SetPosition(position);
	_mEmitter->SetAccel(back * 10.0f);
	_mEmitter->SetVelocity({ -mVelocity.x * 0.1f, 0.0f, -mVelocity.z * 0.1f });

	// 敵との衝突判定
	auto enemys = Game::GetGameObjects<Enemy>();
	for (auto enemy : enemys) {
		// 距離計算
		Vector3 dir = enemy->GetPosition() - position;
		float length = dir.Length();

		// 距離がオブジェクト半径より小さい
		if (length < 1.0f) {
			// 命中した敵・弾・パーティクルエミッタを削除
			enemy->SetDestroy();
			SetDestroy();
			_mEmitter->SetDestroy();

			// 爆発エフェクト
			Game::AddGameObject<ParticleEmitter>()->LoadCSV("assets\\csv\\exp.csv")->SetLoop(false)->SetPosition({enemy->GetPosition().x,
				enemy->GetPosition().y + 1.0f, enemy->GetPosition().z });

			break;
		}
	}

	// 一定時間経過で弾を削除
	// パーティクルエミッタも同時に削除する
	mLifeTime -= deltaTime;
	if (mLifeTime <= 0.0) {
		SetDestroy();
		_mEmitter->SetDestroy();
	}

	// 座標のセット
	mTransform.SetPosition(position);

	GameObject::Update(deltaTime);
}

void Bullet::Draw() const
{
	GameObject::Draw();
}