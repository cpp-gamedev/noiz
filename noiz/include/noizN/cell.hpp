#pragma once
#include <span>
#include "index.hpp"
#include "vec.hpp"

namespace noiz {
template <std::floating_point Type>
struct Corner {
	Vec<Type> location{};
	Vec<Type> gradient{};
};

template <typename Vec_Type>
struct TCell {
	std::vector<Vec_Type> corners;
};

template <std::floating_point Type>
using Cell = TCell<Vec<Type>>;

template <std::floating_point Type>
using CornerCell = TCell<Corner<Type>>;
} // namespace noiz
