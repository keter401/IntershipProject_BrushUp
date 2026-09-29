#pragma once

#include <cmath>

class DWVector2
{
public:
	float x, y;

	// コンストラクタ
	DWVector2() { zero(); }
	DWVector2(const DWVector2 &a) : x(a.x), y(a.y) {}
	DWVector2(float nx, float ny) : x(nx), y(ny) {}

// 標準的なオブジェクトの保守
	// 代入
	DWVector2& operator =(const DWVector2& a);

	// 等しさのチェック
	bool operator ==(const DWVector2& a) const;
	bool operator !=(const DWVector2& a) const;

// ベクトル操作
	// ベクトルを０に設定する
	void zero();
	
	// 単項式のマイナスは、反転したベクトルを返す
	DWVector2 operator -() const;

	// 二項式の + と - はベクトルを加算し、減算する
	DWVector2 operator +(const DWVector2& a) const;
	DWVector2 operator -(const DWVector2& a) const;

	// スカラーによる乗算と除算
	DWVector2 operator *(float a) const;
	DWVector2 operator /(float a) const;

	// 組み合わせ代入演算
	DWVector2& operator +=(const DWVector2& a);
	DWVector2& operator -=(const DWVector2& a);
	DWVector2& operator *=(float a);
	DWVector2& operator /=(float a);

	// ベクトルを正規化する
	void normalize();
	
	// 長さ
	float length() const;

	// ２つの点の距離を計算する
	// (ヘッダで inline 宣言し cpp で定義すると、他の翻訳単位から呼んだ瞬間にリンクエラーになる)
	static float distance(const DWVector2& a, const DWVector2& b);

	// 内積
	static float dot(const DWVector2& a, const DWVector2& b);

	// 外積 (2D ではスカラー。正なら b は a の反時計回り側)
	static float cross(const DWVector2& a, const DWVector2& b);
};