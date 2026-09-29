#pragma once

#include <cmath>

class DWVector2
{
public:
	float x, y;

	// コンストラクタ (constexpr にして static constexpr な定数メンバに使えるようにする)
	constexpr DWVector2() : x(0.0f), y(0.0f) {}
	constexpr DWVector2(float nx, float ny) : x(nx), y(ny) {}

	// コピー/代入はメンバごとの既定動作で十分
	DWVector2(const DWVector2&) = default;
	DWVector2& operator =(const DWVector2&) = default;

// 標準的なオブジェクトの保守
	// 等しさのチェック
	constexpr bool operator ==(const DWVector2& a) const { return x == a.x && y == a.y; }
	constexpr bool operator !=(const DWVector2& a) const { return x != a.x || y != a.y; }

// ベクトル操作
	// ベクトルを０に設定する
	void zero() { x = y = 0.0f; }
	
	// 単項式のマイナスは、反転したベクトルを返す
	constexpr DWVector2 operator -() const { return DWVector2(-x, -y); }

	// 二項式の + と - はベクトルを加算し、減算する
	constexpr DWVector2 operator +(const DWVector2& a) const { return DWVector2(x + a.x, y + a.y); }
	constexpr DWVector2 operator -(const DWVector2& a) const { return DWVector2(x - a.x, y - a.y); }

	// スカラーによる乗算と除算
	constexpr DWVector2 operator *(float a) const { return DWVector2(x * a, y * a); }
	constexpr DWVector2 operator /(float a) const { return DWVector2(x / a, y / a); }

	// 組み合わせ代入演算
	DWVector2& operator +=(const DWVector2& a) { x += a.x; y += a.y; return *this; }
	DWVector2& operator -=(const DWVector2& a) { x -= a.x; y -= a.y; return *this; }
	DWVector2& operator *=(float a) { x *= a; y *= a; return *this; }
	DWVector2& operator /=(float a) { x /= a; y /= a; return *this; }

	// ベクトルを正規化する
	void normalize();
	
	// 長さ
	float length() const;

	// ２つの点の距離を計算する
	static float distance(const DWVector2& a, const DWVector2& b);

	// 内積
	static constexpr float dot(const DWVector2& a, const DWVector2& b) { return a.x * b.x + a.y * b.y; }

	// 外積 (2D ではスカラー。正なら b は a の反時計回り側)
	static constexpr float cross(const DWVector2& a, const DWVector2& b) { return a.x * b.y - a.y * b.x; }
};

// スカラー * ベクトル
constexpr DWVector2 operator *(float a, const DWVector2& v) { return v * a; }
