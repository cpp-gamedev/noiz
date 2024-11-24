#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace noiz {
template <std::floating_point Type, std::size_t N>
struct Vec {
	using value_type = Type;

	static constexpr auto epsilon_v = Type(0.001);
	
	std::array<Type, N> component{};

	[[nodiscard]] constexpr auto sqr_magnitude() const -> Type {
		Type sum = Type(0);
		for (auto const& comp : component) {
			sum += comp * comp;
		}
		return sum;
	}

	[[nodiscard]] auto magnitude() const -> Type { return std::sqrt(sqr_magnitude()); }

	void normalize() {
		auto const mag = magnitude();
		assert(mag > Type(0));

		*this = *this / mag;
	}

	[[nodiscard]] auto normalized() const -> Vec {
		auto ret = *this;
		ret.normalize();
		return ret;
	}

	[[nodiscard]] auto is_normalized() const -> bool { return std::abs(sqr_magnitude() - Type(1)) < epsilon_v; }

	[[nodiscard]] auto modulo(Vec<Type, N> const extent) const -> Vec<Type, N> {
		Vec<Type, N> ret;
		for(std::size_t i = 0; i < N; i++){
			assert(extent.component[i] > Type(0));
			ret.component[i] = std::fmod(component[i], extent.component[i]);
		}
		return ret;
	}

	[[nodiscard]] constexpr auto fract() const -> Vec<Type, N> {
		Vec<Type, N> ret;
		for(std::size_t i = 0; i < N; i++){
			ret.component[i] = component[i] - std::floor(component[i]);
		}
		return ret;
	}

	[[nodiscard]] constexpr auto fade() const -> Vec<Type, N> {
		Vec<Type, N> ret;
		for(std::size_t i = 0; i < N; i++){
			auto const& comp = component[i];
			ret.component[i] = comp * comp * comp * (comp * (comp * Type(6) - Type(15)) + Type(10));
		}
		return ret;
	}

	constexpr auto operator+=(Vec const rhs) -> Vec& {
		for(std::size_t i = 0; i < N; i++){
			component[i] += rhs.component[i];
		}
		return *this;
	}

	constexpr auto operator-=(Vec const rhs) -> Vec& {
		for(std::size_t i = 0; i < N; i++){
			component[i] -= rhs.component[i];
		}
		return *this;
	}

	constexpr auto operator*=(Vec const rhs) -> Vec& {
		for(std::size_t i = 0; i < N; i++){
			component[i] *= rhs.component[i];
		}
		return *this;
	}

	constexpr auto operator/=(Vec const rhs) -> Vec& {
		for(std::size_t i = 0; i < N; i++){
			component[i] /= rhs.component[i];
		}
		return *this;
	}

	friend constexpr auto operator+(Vec lhs, Vec const rhs) -> Vec { return lhs += rhs; }
	friend constexpr auto operator-(Vec lhs, Vec const rhs) -> Vec { return lhs -= rhs; }
	friend constexpr auto operator*(Vec lhs, Vec const rhs) -> Vec { return lhs *= rhs; }
	friend constexpr auto operator/(Vec lhs, Vec const rhs) -> Vec { return lhs /= rhs; }

	//i think this forces the factor/divisor to be the same type as the vec, aka float==float, or double==double,
	//wont allow float*double? not really sure
	friend constexpr auto operator*(Vec lhs, Type const factor) -> Vec {
		Vec ret;
		for(auto& comp : lhs.component){
			comp *= factor;
		}
		return ret;
	}
	friend constexpr auto operator/(Vec lhs, Type const divisor) -> Vec {
		Vec ret;
		for(auto& comp : lhs.component){
			comp /= divisor;
		}
		return ret;
	}

	auto operator==(Vec const&) const -> bool = default;
};

template <std::floating_point Type, std::size_t N>
constexpr auto dot(Vec<Type, N> const lhs, Vec<Type, N> const rhs) -> Type {
	Type dotProd = Type(0);
	for(std::size_t i = 0; i < N; i++){
		dotProd += lhs.component[i] * rhs.component[i];
	}

	return dotProd;
}

template <std::floating_point Type, std::size_t N>
constexpr auto compare(Vec<Type, N> const lhs, Vec<Type, N> const rhs, Type const epsilon = Type(0.001)) {
	for(std::size_t i = 0; i < N; i++){
		if(std::abs(lhs.x - rhs.x) >= epsilon) {
			return false;
		}
	}
	return true;
}

template <std::floating_point Type, std::size_t N>
constexpr auto lerp(Vec<Type, N> const lhs, Vec<Type, N> const rhs, Type alpha) -> Vec<Type, N> {
	Vec<Type, N> ret;
	for(std::size_t i = 0; i < N; i++) {
		ret[i] = std::lerp(lhs.component[i], rhs.component[i], alpha);
	}
	return ret;
}

template<std::floating_point T, typename... Args>
constexpr auto ConstructVector(T first, Args... args) -> Vec<T, sizeof...(Args) + 1>{
	return Vec<T, sizeof...(Args) + 1>{ std::array<T, sizeof...(Args) + 1>{first, args...} };

	/*
	Vec<T, sizeof...(Args) + 1> ret;
	ret.components[0] = first;
	for(std::size_t i = 0; i < (sizeof...(Args)); i++){
		ret.components[i + 1] = args;
	}
	return ret;
	*/
}

using Vec2f = Vec<float, 2>;
using Vec2d = Vec<double, 2>;
using Vec3f = Vec<float, 3>;
using Vec3d = Vec<double, 3>;
using Vec4f = Vec<float, 4>;
using Vec4d = Vec<double, 4>;
} // namespace noiz
