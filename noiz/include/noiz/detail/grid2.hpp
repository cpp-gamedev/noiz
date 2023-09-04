#pragma once
#include <cassert>
#include <vector>
#include "../cell2.hpp"

namespace noiz::detail {
template <std::floating_point Type>
[[nodiscard]] constexpr auto to_index2(Vec2<Type> const point) -> Index2 {
	return Index2{.x = static_cast<int>(point.x), .y = static_cast<int>(point.y)};
}

template <std::floating_point Type>
[[nodiscard]] constexpr auto to_vec2(Index2 const index) -> Vec2<Type> {
	return Vec2<Type>{.x = static_cast<Type>(index.x), .y = static_cast<Type>(index.y)};
}

template <std::floating_point Type>
struct Grid2 {
	std::vector<Corner2<Type>> corners{};
	Index2 grid_size{};

	[[nodiscard]] auto at(CellIndex2 const index) const -> Cell2<Type> {
		return Cell2<Type>{corners.at(index.lt), corners.at(index.rt), corners.at(index.lb), corners.at(index.rb)};
	}

	[[nodiscard]] auto at(Index2 index) const -> Cell2<Type> { return at(CellIndex2::make(index, grid_size)); }
};

template <std::floating_point Type>
[[nodiscard]] auto make_grid2(Index2 grid_size) -> Grid2<Type> {
	auto const corner_count = (grid_size.x + 1) * (grid_size.y + 1);
	if (corner_count <= 0) { return {}; }

	auto ret = Grid2<Type>{
		.corners = std::vector<Corner2<Type>>(static_cast<std::size_t>(corner_count)),
		.grid_size = grid_size,
	};

	for (int row = 0; row <= grid_size.y; ++row) {
		for (int col = 0; col <= grid_size.x; ++col) {
			auto const index2 = Index2{.x = col, .y = row};
			auto const index = static_cast<std::size_t>(index2.flatten(grid_size));
			ret.corners.at(index).location = to_vec2<Type>(Index2{.x = col, .y = row});
		}
	}

	return ret;
}
} // namespace noiz::detail
