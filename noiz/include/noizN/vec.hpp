#pragma once
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>

#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>

namespace noiz {
template <std::floating_point Type>
struct Vec {
	static constexpr auto epsilon_v = Type(0.001);
	std::vector<Type> components;

	[[nodiscard]] constexpr auto sqr_magnitude() const -> Type {
		Type sqr_mag = (Type)0;
		for(int i = 0; i < components.size(); i++){
			sqr_mag += components[i] * components[i];
		}
		return sqr_mag;
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

	[[nodiscard]] auto modulo(Vec<Type> const extent) const -> Vec<Type> {
		for(int i = 0; i < extent.components.size(); i++){
			assert(extent.components[i] > (Type)0);
		}
		Vec<Type> returnVec;
		returnVec.components.resize(components.size(), (Type)0);
		for(uint8_t i = 0; i < components.size(); i++){
			assert(extent.components[i] > (Type)0);
			returnVec.components[i] = std::fmod(components[i], extent.components[i]);
		}
		return returnVec; 
	}

	[[nodiscard]] constexpr auto fract() const -> Vec<Type> { 
		Vec<Type> returnVec;
		returnVec.components.resize(components.size(), (Type)0);
		for(int i = 0; i < components.size(); i++){
			returnVec.components[i] = components[i] - std::floor(components[i]);
		}
		return returnVec; 
		}

	[[nodiscard]] constexpr auto fade() const -> Vec<Type> {
		Vec<Type> returnVec;
		returnVec.components.resize(components.size(), (Type)0);
		for(int i = 0; i < components.size(); i++){
			auto& c = components[i];
			returnVec.components[i] = c * c * c * (c * (c * (Type)6 - (Type)15) + (Type)10);
		}
		return returnVec; 
	}

	constexpr auto operator+=(Vec const rhs) -> Vec& {
		assert(components.size() == rhs.components.size());
		for(int i = 0; i < components.size(); i++){
			components[i] += rhs.components[i];
		}
		return *this;
	}

	constexpr auto operator-=(Vec const rhs) -> Vec& {
		assert(components.size() == rhs.components.size());
		for(int i = 0; i < components.size(); i++){
			components[i] -= rhs.components[i];
		}
		return *this;
	}

	constexpr auto operator*=(Vec const rhs) -> Vec& {
		assert(components.size() == rhs.components.size());
		for(int i = 0; i < components.size(); i++){
			components[i] *= rhs.components[i];
		}
		return *this;
	}

	constexpr auto operator/=(Vec const rhs) -> Vec& {
		assert(components.size() == rhs.components.size());
		for(int i = 0; i < components.size(); i++){
			components[i] /= rhs.components[i];
		}
		return *this;
	}

	friend constexpr auto operator+(Vec lhs, Vec const rhs) -> Vec { return lhs += rhs; }
	friend constexpr auto operator-(Vec lhs, Vec const rhs) -> Vec { return lhs -= rhs; }
	friend constexpr auto operator*(Vec lhs, Vec const rhs) -> Vec { return lhs *= rhs; }
	friend constexpr auto operator/(Vec lhs, Vec const rhs) -> Vec { return lhs /= rhs; }

	friend constexpr auto operator*(Vec lhs, Type const factor) -> Vec {
		Vec<Type> returnVec;
		returnVec.components.resize(lhs.components.size(), (Type)0);
		for(int i = 0; i < lhs.components.size(); i++){
			returnVec[i] = lhs.components[i] * factor;
		}
		return returnVec;
	}
	friend constexpr auto operator/(Vec lhs, Type const divisor) -> Vec {
		Vec<Type> returnVec;
		returnVec.components.resize(lhs.components.size(), (Type)0);
		for(int i = 0; i < lhs.components.size(); i++){
			returnVec[i] = lhs.components[i] / divisor;
		}
		return returnVec;
	}

	auto operator==(Vec const& rhs) const -> bool {
        if (components.size() != rhs.components.size()) {
            return false;
        }
        return std::all_of(components.begin(), components.end(),
                           [&rhs](const Type& element) { return std::find(rhs.components.begin(), rhs.components.end(), element) != rhs.components.end(); });
    
	}
};

template <std::floating_point Type>
constexpr auto dot(Vec<Type> const& lhs, Vec<Type> const& rhs) -> Type {
	assert(lhs.components.size() == rhs.components.size());

	Type dot_value = (Type)0;

	for(int i = 0; i < lhs.components.size(); i++){
		dot_value += lhs.components[i] * rhs.components[i];
	}
	

	return dot_value;
}

template <std::floating_point Type>
constexpr auto compare(Vec<Type> const lhs, Vec<Type> const rhs, Type const epsilon = Type(0.001)) {
	if(lhs.components.size() != rhs.components.size()){
		return false;
	}
	return std::all_of(lhs.components.begin(), lhs.components.end(),
						[&rhs](const Type& element, Type const epsilon) { return (std::find(rhs.components.begin(), rhs.components.end(), element) - rhs.components.end()) < epsilon; });
}

template <std::floating_point Type>
constexpr auto lerp(Vec<Type> const lhs, Vec<Type> const rhs, Type alpha) -> Vec<Type> {
	assert(lhs.components.size() == rhs.components.size());

	Vec<Type> return_vec;
	return_vec.reserve(lhs.components.size());

	std::transform(lhs.components.begin(), lhs.components.end(), rhs.components.begin(),
				std::back_inserter(return_vec), [alpha](Type lhs_value, Type rhs_value) {
					return std::lerp(lhs_value, rhs_value, alpha);
				});

	return return_vec;
}

using Vecf = Vec<float>;
using Vecd = Vec<double>;
} // namespace noiz
