#pragma once
#include <span>
#include "index3.hpp"
#include "vec3.hpp"

namespace noiz {
template <std::floating_point Type>
struct Corner3 {
	Vec3<Type> location{};
	Vec3<Type> gradient{};
};

template <typename Type>
struct TCell3 {
	Type left_top_below{};
	Type right_top_below{};
	Type left_bottom_below{};
	Type right_bottom_below{};
	
	Type left_top_above{};
	Type right_top_above{};
	Type left_bottom_above{};
	Type right_bottom_above{};
};

template <std::floating_point Type>
using Cell3 = TCell3<Vec3<Type>>;

template <std::floating_point Type>
using CornerCell3 = TCell3<Corner3<Type>>;
} // namespace noiz
