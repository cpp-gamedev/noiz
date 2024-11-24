#pragma once
#include <array>
#include "indexTemplate.hpp"
#include "vecTemplate.hpp"

namespace noiz {
template <std::floating_point Type, std::size_t N>
struct CornerData {
	Vec<Type, N> location{};
	Vec<Type, N> gradient{};
};

template <typename Type, std::size_t N>
struct TCell {
	std::array<Type, (1 << N)> corners{};
};

template <std::floating_point Type>
using Cell2 = TCell<Vec<Type, 2>, 2>;
template <std::floating_point Type>
using Cell3 = TCell<Vec<Type, 3>, 3>;
template <std::floating_point Type>
using Cell4 = TCell<Vec<Type, 4>, 4>;
template <std::floating_point Type, std::size_t N>
using Cell = TCell<Vec<Type, N>, N>;

template <std::floating_point Type, std::size_t N>
using CornerCell = TCell<CornerData<Type, N>, N>;
} // namespace noiz
