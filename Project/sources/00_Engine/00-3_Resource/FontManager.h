/*============================================================
*	@file	 : FontManager.h
*	@brief	 : フォント管理
*
* 　@author  : @akitsuki-35（https://github.com/akitsuki-35）
* 　@date	 : 2026/08/11
*	@updated : 2026/08/11
*============================================================*/
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <wrl/client.h>

/*------------------------------------------------------------
	前方宣言
------------------------------------------------------------*/
class Texture;
struct Font;
struct IDWriteFactory;

/*------------------------------------------------------------
	文字テクスチャデータ
------------------------------------------------------------*/
struct Glyph
{
	std::shared_ptr<Texture> Texture{ nullptr };

	int BearingX{ 0 };   // 左側の余白
	int BearingY{ 0 };   // 上側の余白
	int Advance{ 0 };   // 次の文字までの移動量
};

// 探索用キー
struct GlyphKey
{
	Font* Font{};
	uint32_t Codepoint{};
	int Size{};

	bool operator==(const GlyphKey& o) const {
		return Font == o.Font && Codepoint == o.Codepoint && Size == o.Size;
	}
};

// ハッシュ化
template <>
struct std::hash<GlyphKey> {
	size_t operator()(const GlyphKey& k) const {

		size_t h1 = std::hash<const Font*>()(k.Font);
		size_t h2 = std::hash<uint32_t>()(k.Codepoint);
		size_t h3 = std::hash<int>()(k.Size);

		size_t seed = h1;
		seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

/*============================================================
*	@class	: FontManager
*	@brief	: テクスチャのロード・管理
*============================================================*/
class FontManager final
{
	friend class FontLoader;

/*--------------------------------------------------
	Singleton用
----------------------------------------------------*/
public:
	static FontManager& getInstance() {
		static FontManager  instance;
		return instance;
	}

private:
	FontManager() = default;
	FontManager(const FontManager&) = delete;

	FontManager& operator=(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;

	FontManager& operator=(FontManager&&) = delete;
	~FontManager() {};

/*--------------------------------------------------
	メンバ変数・メンバ関数
----------------------------------------------------*/
private:
	// フォントコンテナ
	std::unordered_map<std::string, std::unique_ptr<Font>> mFonts{};

	// Glyphキャッシュ
	std::unordered_map<GlyphKey, std::unique_ptr<Glyph>> mAtlas{};

	// DirectWriteファクトリ
	Microsoft::WRL::ComPtr<IDWriteFactory> _mFactory{ nullptr };

public:
	// ファクトリ取得
	void Initialize(IDWriteFactory* factory) { _mFactory = factory; }

	// フォント取得
	Font* GetFont(const std::string& keyName);
	
	// 文字テクスチャ取得
	Glyph* GetGlyph(const GlyphKey& key);

	// フォント登録
	Font* Register(const std::string& keyName, const char* fontPath);

	// クリア
	void Clear();

private:
	// 文字テクスチャ生成
	bool generateGlyph(Glyph& glyph, const GlyphKey& key);
};

namespace FontSet {
	inline void initialize() {
		FontManager::getInstance().Register("MPLUS_Regular", "assets\\fonts\\MPLUS1-Regular.ttf");
		FontManager::getInstance().Register("MPLUS_Bold", "assets\\fonts\\MPLUS1-Bold.ttf");
		FontManager::getInstance().Register("LogoTypeGothic", "assets\\fonts\\07LogoTypeGothic7.ttf");
		FontManager::getInstance().Register("GenEiLateMin", "assets\\fonts\\GenEiLateMinN_v2.ttf");
		FontManager::getInstance().Register("Kaisotai", "assets\\fonts\\Kaisotai-Next-UP-B.ttf");
	}
}