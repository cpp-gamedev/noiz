#pragma once
#include "generator.hpp"
#include "grid.hpp"

namespace noiz::detail {
template <std::floating_point Type>
auto make_populated_grid(Index const grid_extent, SeedN seed = GeneratorN::make_random_seed()) -> Grid<Type> {
	auto ret = make_grid<Type>(grid_extent);
	auto generator = GeneratorN{static_cast<uint8_t>(grid_extent.components.size()), seed};
	for (auto& corner : ret.corners) { generator.next(corner.gradient); }
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_offsets(CornerCell<Type> const& corner_cell, Vec<Type> const point) -> Cell_Vec<Type> {
	assert(point.components.size() == corner_cell.corners[0].location.components.size());

	Cell_Vec<Type> ret;
	ret.corners;
	ret.corners.resize(corner_cell.corners.size());
	
	for(int i = 0; i < ret.corners.size(); i++){
		ret.corners[i] = point - corner_cell.corners[i].location;
	}
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_dot_products(CornerCell<Type> const& corner_cell, Cell_Vec<Type> const& offset) -> TCell<Type> {

	TCell<Type> ret;
	ret.corners.resize(corner_cell.corners.size());
	
	for(uint16_t i = 0; i < ret.corners.size(); i++){
	//std::cout << "corner_cell corners size: offset size - " << corner_cell.corners[i].gradient.components.size() << ":" << offset.corners[i].components.size() << std::endl;
		ret.corners[i] = dot(corner_cell.corners[i].gradient, offset.corners[i]);
	}
	return ret;
}

template <std::floating_point Type>
constexpr auto interpolate_assistant(Vec<Type> const point, TCell<Type> const& dot_products, uint8_t dimension_count, Vec<Type> cell_interpolated_position, uint16_t corner_index) -> Type {

	Type ret = (Type)0;
	if(dimension_count > 1) {
		

		//at the highest level, corner_index will be passed in as 0
		//in 3 dimensions, i want to add 4
		//2^3 == 8, /2 == 4
		//2 ^ (dimensions - 1)

		const auto value_a = interpolate_assistant(point, dot_products, dimension_count - 1, cell_interpolated_position, corner_index);
		uint16_t corner_offset = 2;
		for(int i = 1; i < dimension_count - 1; i++){
			corner_offset *= 2;
		}
		
		const auto value_b = interpolate_assistant(point, dot_products, dimension_count - 1, cell_interpolated_position, corner_offset);
		ret = std::lerp(
			value_a,
			value_b,
			cell_interpolated_position.components[dimension_count - 1]
		);

	}
	else {
		//currently in the first dimension, the final dimension to be calculated
		ret = std::lerp(
				dot_products.corners[corner_index],
				dot_products.corners[corner_index + 1],
				cell_interpolated_position.components[0]
			);
	}
	return ret;
}


template <std::floating_point Type>
constexpr auto interpolate(Vec<Type> const point, TCell<Type> const& dot_products) -> Type {
	auto const cell_interpolated_position = point.fract().fade();
	uint8_t dimension_count = point.components.size();

	return interpolate_assistant(point, dot_products, dimension_count, cell_interpolated_position, 0);
}
} // namespace noiz::detail
