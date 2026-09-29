#include "Framework\vector2.h"

// 標準的なオブジェクトの保守
	// 代入
DWVector2& DWVector2::operator =(const DWVector2& a)
{
	x = a.x;
	y = a.y;
	return *this;
}

// 等しさのチェック
bool  DWVector2::operator ==(const DWVector2& a) const
{
	return x == a.x && y == a.y;
}
bool  DWVector2::operator !=(const DWVector2& a) const
{
	return x != a.x || y != a.y;
}

// ベクトル操作
	// ベクトルを０に設定する
void  DWVector2::zero()
{
	x = y = 0.0f;
}

// 単項式のマイナスは、反転したベクトルを返す
DWVector2 DWVector2::operator -() const 
{
	return DWVector2(-x, -y);
}

// 二項式の + と - はベクトルを加算し、減算する
DWVector2  DWVector2::operator +(const DWVector2& a) const
{
	return DWVector2(x + a.x, y + a.y);
}
DWVector2  DWVector2::operator -(const DWVector2& a) const
{
	return DWVector2(x - a.x, y - a.y);
}

// スカラーによる乗算と除算
DWVector2 DWVector2::operator *(float a) const
{
	return DWVector2(x * a, y * a);
}
DWVector2 DWVector2::operator /(float a) const
{
	float oneOverA = 1.0f / a;
	return DWVector2(x * oneOverA, y * oneOverA);
}

// 組み合わせ代入演算
DWVector2& DWVector2::operator +=(const DWVector2& a)
{
	x += a.x;
	y += a.y;
	return *this;
}
DWVector2& DWVector2::operator -=(const DWVector2& a)
{
	x -= a.x;
	y -= a.y;
	return *this;
}
DWVector2& DWVector2::operator *=(float a)
{
	x *= a;
	y *= a;
	return *this;
}
DWVector2& DWVector2::operator /=(float a)
{
	float oneOverA = 1.0f / a;
	x *= oneOverA;
	y *= oneOverA;
	return *this;
}

// ベクトルを正規化する
void DWVector2::normalize() {
	float magSq = x * x + y * y;
	if (magSq > 0.0f) {	// 0除算をチェックする
		float oneOverMag = 1.0f / std::sqrt(magSq);
		x *= oneOverMag;
		y *= oneOverMag;
	}
}

// 長さ
float DWVector2::length() const {
	return std::sqrt(x * x + y * y);
}

// ２つの点の距離を計算する
float DWVector2::distance(const DWVector2& a, const DWVector2& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return std::sqrt(dx * dx + dy * dy);
}

// 内積
float DWVector2::dot(const DWVector2& a, const DWVector2& b)
{
	return a.x * b.x + a.y * b.y;
}

// 外積
float DWVector2::cross(const DWVector2& a, const DWVector2& b)
{
	return a.x * b.y - a.y * b.x;
}