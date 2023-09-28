#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>

namespace noiz {
template <std::floating_point Type>
struct Vec3 {
	static constexpr auto epsilon_v = Type(0.001);

	Type x{};
	Type y{};
	Type z{};

	[[nodiscard]] constexpr auto sqr_magnitude() const -> Type { return x * x + y * y + z * z; }

	[[nodiscard]] auto magnitude() const -> Type { return std::sqrt(sqr_magnitude()); }

	void normalize() {
		auto const mag = magnitude();
		assert(mag > Type(0));

		*this = *this / mag;
	}

	[[nodiscard]] auto normalized() const -> Vec3 {
		auto ret = *this;
		ret.normalize();
		return ret;
	}

	[[nodiscard]] auto is_normalized() const -> bool { return std::abs(sqr_magnitude() - Type(1)) < epsilon_v; }

	[[nodiscard]] auto modulo(Vec3<Type> const extent) const -> Vec3<Type> {
		assert(extent.x > Type(0) && extent.y > Type(0));
		return Vec3<Type>{.x = std::fmod(x, extent.x), .y = std::fmod(y, extent.y), .z = std::fmod(z, extent.z)};
	}

	[[nodiscard]] constexpr auto fract() const -> Vec3<Type> { return Vec3<Type>{.x = x - std::floor(x), .y = y - std::floor(y), .z = z - std::floor(z)}; }

	[[nodiscard]] constexpr auto fade() const -> Vec3<Type> {
		return Vec3<Type>{
			.x = x * x * x * (x * (x * 6 - 15) + 10),
			.y = y * y * y * (y * (y * 6 - 15) + 10),
			.z = z * z * z * (z * (z * 6 - 15) + 10)
		};
	}

	constexpr auto operator+=(Vec3 const rhs) -> Vec3& {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	constexpr auto operator-=(Vec3 const rhs) -> Vec3& {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	constexpr auto operator*=(Vec3 const rhs) -> Vec3& {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	constexpr auto operator/=(Vec3 const rhs) -> Vec3& {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	friend constexpr auto operator+(Vec3 lhs, Vec3 const rhs) -> Vec3 { return lhs += rhs; }
	friend constexpr auto operator-(Vec3 lhs, Vec3 const rhs) -> Vec3 { return lhs -= rhs; }
	friend constexpr auto operator*(Vec3 lhs, Vec3 const rhs) -> Vec3 { return lhs *= rhs; }
	friend constexpr auto operator/(Vec3 lhs, Vec3 const rhs) -> Vec3 { return lhs /= rhs; }
	friend constexpr auto operator*(Vec3 lhs, Type const factor) -> Vec3 {return Vec3{.x = lhs.x * factor, .y = lhs.y * factor, .z = lhs.z * factor};}
	friend constexpr auto operator/(Vec3 lhs, Type const divisor) -> Vec3 {return Vec3{.x = lhs.x / divisor, .y = lhs.y / divisor, .z = lhs.z / divisor};}

	auto operator==(Vec3 const&) const -> bool = default;
};

template <std::floating_point Type>
constexpr auto dot(Vec3<Type> const lhs, Vec3<Type> const rhs) -> Type {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <std::floating_point Type>
constexpr auto compare(Vec3<Type> const lhs, Vec3<Type> const rhs, Type const epsilon = Type(0.001)) {
	return (std::abs(lhs.x - rhs.x) < epsilon) && (std::abs(lhs.y - rhs.y) < epsilon) && (std::abs(lhs.z - rhs.z) < epsilon);
}

template <std::floating_point Type>
constexpr auto lerp(Vec3<Type> const lhs, Vec3<Type> const rhs, Type alpha) -> Vec3<Type> {
	return Vec3<Type>{
		.x = std::lerp(lhs.x, rhs.x, alpha),
		.y = std::lerp(lhs.y, rhs.y, alpha),
		.z = std::lerp(lhs.z, rhs.z, alpha)
	};
}

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
} // namespace noiz
