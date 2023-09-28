#pragma once
#include <cassert>
#include <vector>
#include "../cell.hpp"

namespace noiz::detail {
template <std::floating_point Type>
[[nodiscard]] constexpr auto to_index(Vec<Type> const point) -> Index {
	Index ret;
	ret.components.resize(point.components.size());
	for(int i = 0; i < ret.components.size(); i++){
		ret.components[i] = static_cast<int>(point.components[i]);
	}
	return ret;
}

template <std::floating_point Type>
[[nodiscard]] constexpr auto to_vec(Index const index) -> Vec<Type> {
	Vec<Type> returnVec;
	returnVec.reserve(index.size());
	for(int i = 0; i < index.size(); i++){
		returnVec.emplace_back(static_cast<Type>(index[0]));
	}
	//return Vec<Type>{.x = static_cast<Type>(index.x), .y = static_cast<Type>(index.y), .z = static_cast<Type>(index.z)};
}

template <std::floating_point Type>
struct Grid {
	std::vector<Corner<Type>> corners{};
	Index grid_extent{};

	[[nodiscard]] auto at(CellIndex const index) const -> CornerCell<Type> {
		CornerCell<Type> ret;
		
		return CornerCell<Type>{
			corners.at(index.ltb), corners.at(index.rtb), corners.at(index.lbb), corners.at(index.rbb),
			corners.at(index.lta), corners.at(index.rta), corners.at(index.lba), corners.at(index.rba)
			};
	}

	[[nodiscard]] auto at(Index index) const -> CornerCell<Type> { return at(CellIndex::make(index, grid_extent)); }
};

template <std::floating_point Type>
[[nodiscard]] auto make_grid_support(std::vector<int> component_position, int dimension_position, Index const& grid_extent, uint8_t const& dimension_count) -> bool {
	if(dimension_position >= dimension_count){

	}
	for(int i = 0; i < grid_extent.components[dimension_position]; i++){

	}
}

template <std::floating_point Type>
[[nodiscard]] auto make_grid(Index grid_extent, uint8_t dimension_count) -> Grid<Type> {
	auto const corner_count = (grid_extent.x + 1) * (grid_extent.y + 1) * (grid_extent.z + 1);
	if (corner_count <= 0) { return {}; }

	auto ret = Grid<Type>{
		.corners = std::vector<Corner<Type>>(static_cast<std::size_t>(corner_count)),
		.grid_extent = grid_extent,
	};

	Index index;
	auto const grid_dimensions = grid_extent.components.size();
	index.components.resize(grid_dimensions, 0);

	auto back_dimension_grid_extent = grid_extent.components[grid_dimensions - 1] + 1;
	auto& back_component_position = index.components[grid_dimensions - 1];

	auto& first_dimension_position = cindex.components[0];
	auto first_dimension_grid_extent = (grid_extent.components[0] + 1);

	do{
		first_dimension_position++;
		for(uint8_t i = 0; i < (dimension_count - 1); i++){
			index.components[i + 1] += index.components[i] == (grid_extent.components[i] + 1);
			index.components[i] %= grid_extent.components[i] + 1;
		}
		auto const flat_index = static_cast<std::size_t>(index.flatten(grid_extent));
		ret.corners.at(index).location = to_vec<Type>(index);
	}
	while(back_component_position < back_dimension_grid_extent);


	for(int depth = 0; depth <= grid_extent.z; ++depth) {
		for (int row = 0; row <= grid_extent.y; ++row) {
			for (int col = 0; col <= grid_extent.x; ++col) {
				Index index;
				index.reserve(dimension_count);
				for(int i = 0; i < dimension_count; i++){

				}
				 = Index{.x = col, .y = row, .z = depth};
				auto const index = static_cast<std::size_t>(index.flatten(grid_extent));
				ret.corners.at(index).location = to_vec3<Type>(Index{.x = col, .y = row, .z = depth});
			}
		}
	}

	return ret;
}
} // namespace noiz::detail
