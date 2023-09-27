#pragma once
#include <span>
#include "index.hpp"
#include "vec.hpp"

namespace noiz {
template <std::floating_point Type>
struct Corner3 {
	Vec<Type> location{};
	Vec<Type> gradient{};
};

template <typename Type>
struct TCell {
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
using Cell = TCell<Vec<Type>>;

template <std::floating_point Type>
using CornerCell = TCell<Corner<Type>>;
} // namespace noiz
