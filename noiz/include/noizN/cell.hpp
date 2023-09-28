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

template <typename Type>
struct TCell {
	std::vector<Type> corners;
};

template <std::floating_point Type>
using Cell_Vec = TCell<Vec<Type>>;

template <std::floating_point Type>
using CornerCell = TCell<Corner<Type>>;
} // namespace noiz