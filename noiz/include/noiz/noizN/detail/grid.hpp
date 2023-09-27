#pragma once
#include <cassert>
#include <vector>
#include "../cell.hpp"

namespace noiz::detail {
template <std::floating_point Type>
[[nodiscard]] constexpr auto to_index(Vec3<Type> const point) -> Index {
	return Index{.x = static_cast<int>(point.x), .y = static_cast<int>(point.y), .z = static_cast<int>(point.z)};
}

template <std::floating_point Type>
[[nodiscard]] constexpr auto to_vec(Index const index) -> Vec3<Type> {
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

	std::vector<int> component_position;
	auto const grid_dimensions = grid_extent.components.size();
	component_position.resize(grid_dimensions, 0);


	bool finished = false;
	for(uint8_t dimension_position = 0; dimension_position < dimension_count;){

	}

	auto back_dimension_grid_extent = grid_extent.components.back() + 1;
	auto& back_component_position = component_position[grid_dimensions - 1];

	auto& first_dimension_position = component_position[0];
	auto first_dimension_grid_extent = (grid_extent.components[0] + 1);

	while(back_component_position < back_dimension_grid_extent){
		first_dimension_position++;
		for(uint8_t i = 0; i < (dimension_count - 1); i++){
			component_position[]
			if(first_dimension_position == first_dimension_grid_extent){
				component_position[1]++;
				component_Position[dimension_position] = 0;
				dimension_position++;
			}
		}

	}


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
