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

template <typename Type>
struct TCell2 {
	Type left_top{};
	Type right_top{};
	Type left_bottom{};
	Type right_bottom{};
};

template <std::floating_point Type>
using Cell2 = TCell2<Vec2<Type>>;

template <std::floating_point Type>
using CornerCell2 = TCell2<Corner2<Type>>;
} // namespace noiz
