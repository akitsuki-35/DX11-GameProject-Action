/*============================================================
*	@file	 : ParticleEmitter.cpp
*	@brief	 : パーティクルエミッタ
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/06/18
*	@updated : 2026/06/18
*============================================================*/
#include "ParticleEmitter.h"
#include "ParticleRenderer.h"
#include "ParticleBox.h"
#include "MeshTypes.h"
#include "GameMode.h"
#include "Timer.h"

using namespace MeshType;
using namespace DirectX;

void ParticleEmitter::Initialize()
{
	ParticleRenderer* renderer = AddComponent<ParticleRenderer>(this)->SetEmitter(this);

	renderer->LoadTexture("assets\\textures\\particle.png")->LoadShader("Unlit")
		->SetBlendState(Blend::Add)->SetLayer(Layer::Alpha);

	// 構造体初期化
	mParticles.resize(PARTICLE_MAX);
	for (int i = 0; i < PARTICLE_MAX; i++) {
		mParticles[i].mEnable = false;
	}

	_mType = std::make_unique<ParticleType::Box>(this);
}

void ParticleEmitter::Finalize()
{
	GameObject::Finalize();
}

void ParticleEmitter::Update(double deltaTime)
{
	if (GameMode::IsHitStop()) return;

	_mType->Update(deltaTime);

	mCurrentInterval -= deltaTime;

	if (mCurrentInterval <= 0.0) {
		_mType->Emission(mDesc);
		mCurrentInterval = mMaxInterval;
	}

	if (!mLoop) {
		alphaUpdate();

		if (_mEmitterLife->IsTimeUp()) {
			SetDestroy();
		}
	}

	GameObject::Update(deltaTime);
}

void ParticleEmitter::Draw() const
{
	GameObject::Draw();
}

ParticleEmitter* ParticleEmitter::LoadCSV(const char* filePath)
{
	auto newType = _mType->LoadCSV(filePath);

	if (newType) {
		this->SetType(std::move(newType));
	}
	return this;
}

ParticleEmitter* ParticleEmitter::SetEmitterLife(double lifeTime)
{
	// エミッタの寿命（再生時間）をセット
	_mEmitterLife = AddComponent<Timer>(this);
	_mEmitterLife->Start(lifeTime);

	// ループしない設定にする
	mLoop = false;

	return this;
}

void ParticleEmitter::alphaUpdate()
{
	// エミッタ寿命に応じて透明度を変更
	auto renderer = GetComponent<ParticleRenderer>();
	float alpha = _mEmitterLife->GetProgress();

	// メインカラー透明度更新
	DirectX::XMFLOAT4 color = renderer->mColor;
	color.w = alpha;
	renderer->mColor = color;

	// サブカラー透明度更新
	color = renderer->mSubColor;
	color.w = alpha;
	renderer->mSubColor = color;
}