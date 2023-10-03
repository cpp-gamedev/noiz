#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>

namespace noiz {
template <std::floating_point Type>
struct Vec4 {
	using value_type = Type;
	
	static constexpr auto epsilon_v = Type(0.001);

	Type x{};
	Type y{};
	Type z{};
	Type w{};

	[[nodiscard]] constexpr auto sqr_magnitude() const -> Type { return x * x + y * y + z * z + w * w; }

	[[nodiscard]] auto magnitude() const -> Type { return std::sqrt(sqr_magnitude()); }

	void normalize() {
		auto const mag = magnitude();
		assert(mag > Type(0));

		*this = *this / mag;
	}

	[[nodiscard]] auto normalized() const -> Vec4 {
		auto ret = *this;
		ret.normalize();
		return ret;
	}

	[[nodiscard]] auto is_normalized() const -> bool { return std::abs(sqr_magnitude() - Type(1)) < epsilon_v; }

	[[nodiscard]] auto modulo(Vec4<Type> const extent) const -> Vec4<Type> {
		assert(extent.x > Type(0) && extent.y > Type(0));
		return Vec4<Type>{.x = std::fmod(x, extent.x), .y = std::fmod(y, extent.y), .z = std::fmod(z, extent.z), .w = std::fmod(w, extent.w)};
	}

	[[nodiscard]] constexpr auto fract() const -> Vec4<Type> { return Vec4<Type>{.x = x - std::floor(x), .y = y - std::floor(y), .z = z - std::floor(z), .w = w - std::floor(w)}; }

	[[nodiscard]] constexpr auto fade() const -> Vec4<Type> {
		return Vec4<Type>{
			.x = x * x * x * (x * (x * 6 - 15) + 10),
			.y = y * y * y * (y * (y * 6 - 15) + 10),
			.z = z * z * z * (z * (z * 6 - 15) + 10),
			.w = w * w * w * (w * (w * 6 - 15) + 10)
		};
	}

	constexpr auto operator+=(Vec4 const rhs) -> Vec4& {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	constexpr auto operator-=(Vec4 const rhs) -> Vec4& {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	constexpr auto operator*=(Vec4 const rhs) -> Vec4& {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	constexpr auto operator/=(Vec4 const rhs) -> Vec4& {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	friend constexpr auto operator+(Vec4 lhs, Vec4 const rhs) -> Vec4 { return lhs += rhs; }
	friend constexpr auto operator-(Vec4 lhs, Vec4 const rhs) -> Vec4 { return lhs -= rhs; }
	friend constexpr auto operator*(Vec4 lhs, Vec4 const rhs) -> Vec4 { return lhs *= rhs; }
	friend constexpr auto operator/(Vec4 lhs, Vec4 const rhs) -> Vec4 { return lhs /= rhs; }
	friend constexpr auto operator*(Vec4 lhs, Type const factor) -> Vec4 {return Vec4{.x = lhs.x * factor, .y = lhs.y * factor, .z = lhs.z * factor, .w = lhs.w * factor};}
	friend constexpr auto operator/(Vec4 lhs, Type const divisor) -> Vec4 {return Vec4{.x = lhs.x / divisor, .y = lhs.y / divisor, .z = lhs.z / divisor .w = lhs.w / divisor};}

	auto operator==(Vec4 const&) const -> bool = default;
};

template <std::floating_point Type>
constexpr auto dot(Vec4<Type> const lhs, Vec4<Type> const rhs) -> Type {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z, + lhs.w * lhs.w;
}

template <std::floating_point Type>
constexpr auto compare(Vec4<Type> const lhs, Vec4<Type> const rhs, Type const epsilon = Type(0.001)) {
	return (std::abs(lhs.x - rhs.x) < epsilon) && (std::abs(lhs.y - rhs.y) < epsilon) && (std::abs(lhs.z - rhs.z) < epsilon) && (std::abs(lhs.w - rhs.w) < epsilon);
}

template <std::floating_point Type>
constexpr auto lerp(Vec4<Type> const lhs, Vec4<Type> const rhs, Type alpha) -> Vec4<Type> {
	return Vec4<Type>{
		.x = std::lerp(lhs.x, rhs.x, alpha),
		.y = std::lerp(lhs.y, rhs.y, alpha),
		.z = std::lerp(lhs.z, rhs.z, alpha),
		.w = std::lerp(lhs.w, rhs.w, alpha)
	};
}

using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;
} // namespace noiz
