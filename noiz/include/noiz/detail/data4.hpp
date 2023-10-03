#pragma once
#include "generator.hpp"
#include "grid4.hpp"

namespace noiz::detail {
template <std::floating_point Type>
auto make_populated_grid(Index4 const grid_extent, Seed seed = Generator::make_random_seed()) -> Grid4<Type> {
	auto ret = make_grid4<Type>(grid_extent);
	auto generator = Generator{seed};
	for (auto& corner : ret.corners) { generator.next(corner.gradient); }
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_offsets(CornerCell4<Type> const& corner, Vec4<Type> const point) -> Cell4<Type> {

	Cell4<Type> ret;
	for(uint8_t i = 0; i < 16; i++){
		ret.corners[i] = point - corner.corners[i];
	}
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_dot_products(CornerCell4<Type> const& corner, Cell4<Type> const& offset) -> TCell4<Type> {

	TCell4<Type> ret;
	for(int i = 0; i < 16; i++){
		ret.corners[i] = dot(corner.corners[i].gradient, offset.corners[i]);
	}
	return ret;
}

template <std::floating_point Type>
constexpr auto interpolate(Vec4<Type> const point, TCell4<Type> const& dot_products) -> Type {
	auto const cell_interpolated_position = point.fract().fade(); 

	//1st dimension
	auto const below_a = std::lerp(dot_products.corners[0], dot_products.corners[1], cell_interpolated_position.x);
	//2nd dimension
	auto const below_b = std::lerp(dot_products.corners[2], dot_products.corners[3], cell_interpolated_position.x);
	auto const below = std::lerp(below_a, below_b, cell_interpolated_position.y);
	
	//3rd dimension
	auto const above_a = std::lerp(dot_products.corners[4], dot_products.corners[5], cell_interpolated_position.x);
	auto const above_b = std::lerp(dot_products.corners[6], dot_products.corners[7], cell_interpolated_position.x);
	auto const above = std::lerp(above_a, above_b, cell_interpolated_position.y);

	auto const third_lerp_a = std::lerp(below, above, cell_interpolated_position.z);

	//4th dimension
	auto const fourth_below_a = std::lerp(dot_products.corners[8], dot_products.corners[9], cell_interpolated_position.x);
	auto const fourth_below_b = std::lerp(dot_products.corners[10], dot_products.corners[11], cell_interpolated_position.x);
	auto const fourth_below = std::lerp(fourth_below_a, fourth_below_b, cell_interpolated_position.y);
	
	auto const fourth_above_a = std::lerp(dot_products.corners[12], dot_products.corners[13], cell_interpolated_position.x);
	auto const fourth_above_b = std::lerp(dot_products.corners[14], dot_products.corners[15], cell_interpolated_position.x);
	auto const fourth_above = std::lerp(fourth_above_a, fourth_above_b, cell_interpolated_position.y);

	auto const third_lerp_b = std::lerp(fourth_below, fourth_above, cell_interpolated_position.z);



	//i might need to swap the position of below and above, not really sure
	return std::lerp(third_lerp_a, third_lerp_b, cell_interpolated_position.w);
}
} // namespace noiz::detail
