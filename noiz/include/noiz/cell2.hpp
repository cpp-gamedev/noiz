#pragma once
#include <span>
#include "index2.hpp"
#include "vec2.hpp"

namespace noiz {
template <std::floating_point Type>
struct Corner2 {
	Vec2<Type> location{};
	Vec2<Type> gradient{};
};

template <std::floating_point Type>
struct Cell2 {
	Corner2<Type> left_top{};
	Corner2<Type> right_top{};
	Corner2<Type> left_bottom{};
	Corner2<Type> right_bottom{};
};
} // namespace noiz
