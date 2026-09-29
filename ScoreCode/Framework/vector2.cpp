#include "Framework\vector2.h"

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
