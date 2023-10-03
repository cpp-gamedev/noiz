#pragma once
#include <span>
#include "index4.hpp"
#include "vec4.hpp"

namespace noiz {
template <std::floating_point Type>
struct Corner4 {
	Vec4<Type> location{};
	Vec4<Type> gradient{};
};

template <typename Type>
struct TCell4 {
	//16 is 2^4
	std::array<Type, 16> corners;
};

template <std::floating_point Type>
using Cell4 = TCell4<Vec4<Type>>;

template <std::floating_point Type>
using CornerCell4 = TCell4<Corner4<Type>>;
} // namespace noiz
