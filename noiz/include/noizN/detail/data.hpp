#pragma once
#include "generator.hpp"
#include "grid.hpp"

namespace noiz::detail {
template <std::floating_point Type>
auto make_populated_grid(Index const grid_extent, Seed seed = Generator::make_random_seed()) -> Grid<Type> {
	auto ret = make_grid<Type>(grid_extent);
	auto generator = Generator{seed};
	for (auto& corner : ret.corners) { generator.next(corner.gradient); }
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_offsets(CornerCell<Type> const& corner, Vec<Type> const point) -> Cell<Type> {

	Cell<Type> ret;
	ret.components.resize(corner.size());
	
	for(int i = 0; i < ret.components.size(); i++){
		ret.components[i] = point - corner.location.components[i];
	}
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_dot_products(CornerCell<Type> const& corner, Cell<Type> const& offset) -> TCell<Type> {

	Cell<Type> ret;
	ret.components.resize(corner.size());
	
	for(int i = 0; i < ret.components.size(); i++){
		ret.components[i] = dot(corner.gradient.components[i], offset.components[i]);
	}
	return ret;
}

template <std::floating_point Type>
constexpr auto interpolate_assistant(Vec<Type> const point, TCell<Type> const& dot_products, uint8_t dimension_count, Vec<Type> cell_interpolated_position, uint16_t corner_index) -> Type {

	Type ret;
	if(dimension_count > 1) {
		Type ret = (Type)0;

		//at the highest level, corner_index will be passed in as 0
		//in 3 dimensions, i want to add 4
		//2^3 == 8, /2 == 4
		//2 ^ (dimensions - 1)

		auto const value_a = interpolate_assistant(point, dot_products, dimension_count - 1, corner_index);
		uint16_t corner_offset = 2;
		for(int i = 1; i < dimension_count - 1; i++){
			corner_offset *= 2;
		}
		
		auto const value_b = interpolate_assistant(point, dot_products, dimension_count - 1, corner_offset);
		ret = std::lerp(
			value_a,
			value_b,
			cell_interpolated_position[dimension_count - 1]
		);

	}
	else {
		//currently in the first dimension, the final dimension to be calculated
		ret = std::lerp(
				dot_products.corners[corner_index],
				dot_products.corners[corner_index + 1],
				cell_interpolated_position[0];
			);
	}
	return ret;
}


template <std::floating_point Type>
constexpr auto interpolate(Vec3<Type> const point, TCell3<Type> const& dot_products) -> Type {
	auto const cell_interpolated_position = point.fract().fade();
	uint8_t dimension_count = point.components.size();

	return interpolate_assistant(point, dot_products, dimension_count, cell_interpolated_position, 0);
}
} // namespace noiz::detail
