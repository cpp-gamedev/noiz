#pragma once
#include <cassert>
#include <vector>
#include "../cell4.hpp"

namespace noiz::detail {
template <std::floating_point Type>
[[nodiscard]] constexpr auto to_index4(Vec4<Type> const point) -> Index4 {
	return Index4{.x = static_cast<int>(point.x), .y = static_cast<int>(point.y), .z = static_cast<int>(point.z), .w = static_cast<int>(point.w)};
}

template <std::floating_point Type>
[[nodiscard]] constexpr auto to_vec4(Index4 const index) -> Vec4<Type> {
	return Vec4<Type>{.x = static_cast<Type>(index.x), .y = static_cast<Type>(index.y), .z = static_cast<Type>(index.z), .w = static_cast<Type>(index.w)};
}

template <std::floating_point Type>
struct Grid4 {
	std::vector<Corner4<Type>> corners{};
	Index4 grid_extent{};

	[[nodiscard]] auto at(CellIndex4 const index) const -> CornerCell4<Type> {
		return CornerCell4<Type>{
			corners.at(index.at(0)), corners.at(index.at(1)), corners.at(index.at(2), corners.at(index.at(3)),
			corners.at(index.at(4)), corners.at(index.at(5)), corners.at(index.at(6)), corners.at(index.at(7)),
			corners.at(index.at(8)), corners.at(index.at(9)), corners.at(index.at(10), corners.at(index.at(11)),
			corners.at(index.at(12)), corners.at(index.at(13)), corners.at(index.at(14)), corners.at(index.at(15))
		};
	}

	[[nodiscard]] auto at(Index4 index) const -> CornerCell4<Type> { return at(CellIndex4::make(index, grid_extent)); }
};

template <std::floating_point Type>
[[nodiscard]] auto make_grid4(Index4 grid_extent) -> Grid4<Type> {
	auto const corner_count = (grid_extent.x + 1) * (grid_extent.y + 1) * (grid_extent.z + 1) * (grid_extent.w + 1);
	if (corner_count <= 0) { return {}; }

	auto ret = Grid4<Type>{
		.corners = std::vector<Corner4<Type>>(static_cast<std::size_t>(corner_count)),
		.grid_extent = grid_extent,
	};

	for(int fourth = 0; fourth <= grid_extent.w; ++fourth){ //fourth dimension???
		for(int depth = 0; depth <= grid_extent.z; ++depth) {
			for (int row = 0; row <= grid_extent.y; ++row) {
				for (int col = 0; col <= grid_extent.x; ++col) {
					auto const index4 = Index4{.x = col, .y = row, .z = depth, .w = fourth};
					auto const index = static_cast<std::size_t>(index4.flatten(grid_extent));
					ret.corners.at(index).location = to_vec4<Type>(Index4{.x = col, .y = row, .z = depth, .w = fourth});
				}
			}
		}
	}
	return ret;
}
} // namespace noiz::detail
