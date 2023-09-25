#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>

namespace noiz {
template <std::floating_point Type>
struct Vec2 {
	static constexpr auto epsilon_v = Type(0.001);

	Type x{};
	Type y{};

	[[nodiscard]] constexpr auto sqr_magnitude() const -> Type { return x * x + y * y; }

	[[nodiscard]] auto magnitude() const -> Type { return std::sqrt(sqr_magnitude()); }

	void normalize() {
		auto const mag = magnitude();
		assert(mag > Type(0));
		x /= mag;
		y /= mag;
	}

	[[nodiscard]] auto normalized() const -> Vec2 {
		auto ret = *this;
		ret.normalize();
		return ret;
	}

	[[nodiscard]] auto is_normalized() const -> bool { return std::abs(sqr_magnitude() - Type(1)) < epsilon_v; }

	[[nodiscard]] auto modulo(Vec2<Type> const extent) const -> Vec2<Type> {
		assert(extent.x > Type(0) && extent.y > Type(0));
		return Vec2<Type>{.x = std::fmod(x, extent.x), .y = std::fmod(y, extent.y)};
	}

	[[nodiscard]] constexpr auto fract() const -> Vec2<Type> { return Vec2<Type>{.x = x - std::floor(x), .y = y - std::floor(y)}; }

	[[nodiscard]] constexpr auto fade() const -> Vec2<Type> {
		/* explanation from chatgpt
		The function calculates a smooth transition using a polynomial expression. 
		Here's the breakdown of the expression:

    	t * t * t: This part cubically interpolates t. 
		Cubic interpolation ensures a smooth transition from 0 to 1, 
		where the rate of change is slower at the beginning and end of the interval.

    	(t * (t * 6 - 15) + 10): 
		This part further manipulates the cubic interpolation to achieve a smooth fade effect. 
		The expression (t * 6 - 15) stretches the t values so that they cover a wider range. 
		The + 10 shifts the range so that it starts from 0 and ends at 1.
		*/

		return Vec2<Type>{
			.x = x * x * x * (x * (x * 6 - 15) + 10),
			.y = y * y * y * (y * (y * 6 - 15) + 10)
		};
	}

	constexpr auto operator+=(Vec2 const rhs) -> Vec2& {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	constexpr auto operator-=(Vec2 const rhs) -> Vec2& {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	constexpr auto operator*=(Vec2 const rhs) -> Vec2& {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	constexpr auto operator/=(Vec2 const rhs) -> Vec2& {
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	friend constexpr auto operator+(Vec2 lhs, Vec2 const rhs) -> Vec2 { return lhs += rhs; }
	friend constexpr auto operator-(Vec2 lhs, Vec2 const rhs) -> Vec2 { return lhs -= rhs; }
	friend constexpr auto operator*(Vec2 lhs, Vec2 const rhs) -> Vec2 { return lhs *= rhs; }
	friend constexpr auto operator/(Vec2 lhs, Vec2 const rhs) -> Vec2 { return lhs /= rhs; }

	auto operator==(Vec2 const&) const -> bool = default;
};

template <std::floating_point Type>
constexpr auto dot(Vec2<Type> const lhs, Vec2<Type> const rhs) -> Type {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <std::floating_point Type>
constexpr auto compare(Vec2<Type> const lhs, Vec2<Type> const rhs, Type const epsilon = Type(0.001)) {
	return std::abs(lhs.x - rhs.x) < epsilon && std::abs(lhs.y - rhs.y) < epsilon;
}

template <std::floating_point Type>
constexpr auto lerp(Vec2<Type> const lhs, Vec2<Type> const rhs, Type alpha) -> Vec2<Type> {
	return Vec2<Type>{
		.x = std::lerp(lhs.x, rhs.x, alpha),
		.y = std::lerp(lhs.y, rhs.y, alpha),
	};
}

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
} // namespace noiz
