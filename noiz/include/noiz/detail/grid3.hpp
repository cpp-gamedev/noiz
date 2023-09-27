#pragma once
#include <cassert>
#include <vector>
#include "../cell3.hpp"

namespace noiz::detail {
template <std::floating_point Type>
[[nodiscard]] constexpr auto to_index3(Vec3<Type> const point) -> Index3 {
	return Index3{.x = static_cast<int>(point.x), .y = static_cast<int>(point.y), .z = static_cast<int>(point.z)};
}

template <std::floating_point Type>
[[nodiscard]] constexpr auto to_vec3(Index3 const index) -> Vec3<Type> {
	return Vec3<Type>{.x = static_cast<Type>(index.x), .y = static_cast<Type>(index.y), .z = static_cast<Type>(index.z)};
}

template <std::floating_point Type>
struct Grid3 {
	std::vector<Corner3<Type>> corners{};
	Index3 grid_extent{};

	[[nodiscard]] auto at(CellIndex3 const index) const -> CornerCell3<Type> {
		return CornerCell3<Type>{
			corners.at(index.ltb), corners.at(index.rtb), corners.at(index.lbb), corners.at(index.rbb),
			corners.at(index.lta), corners.at(index.rta), corners.at(index.lba), corners.at(index.rba)
			};
	}

	[[nodiscard]] auto at(Index3 index) const -> CornerCell3<Type> { return at(CellIndex3::make(index, grid_extent)); }
};

template <std::floating_point Type>
[[nodiscard]] auto make_grid3(Index3 grid_extent) -> Grid3<Type> {
	auto const corner_count = (grid_extent.x + 1) * (grid_extent.y + 1) * (grid_extent.z + 1);
	if (corner_count <= 0) { return {}; }

	auto ret = Grid3<Type>{
		.corners = std::vector<Corner3<Type>>(static_cast<std::size_t>(corner_count)),
		.grid_extent = grid_extent,
	};

	for(int depth = 0; depth <= grid_extent.z; ++depth){
		for (int row = 0; row <= grid_extent.y; ++row) {
			for (int col = 0; col <= grid_extent.x; ++col) {
				auto const index3 = Index3{.x = col, .y = row, .z = depth};
				auto const index = static_cast<std::size_t>(index3.flatten(grid_extent));
				ret.corners.at(index).location = to_vec3<Type>(Index3{.x = col, .y = row, .z = depth});
			}
		}
	}

	return ret;
}
} // namespace noiz::detail
