#pragma once
#include "generator.hpp"
#include "grid.hpp"

namespace noiz::detail {
template <std::floating_point Type>
auto make_populated_grid(Index const grid_extent, Seed seed = Generator::make_random_seed()) -> Grid3<Type> {
	auto ret = make_grid<Type>(grid_extent);
	auto generator = Generator{seed};
	for (auto& corner : ret.corners) { generator.next(corner.gradient); }
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_offsets(CornerCell<Type> const& corner, Vec<Type> const point) -> Cell<Type> {
	return Cell3<Type>{
		.left_top_below = point - corner.left_top_below.location,
		.right_top_below = point - corner.right_top_below.location,
		.left_bottom_below = point - corner.left_bottom_below.location,
		.right_bottom_below = point - corner.right_bottom_below.location,
		.left_top_above = point - corner.left_top_above.location,
		.right_top_above = point - corner.right_top_above.location,
		.left_bottom_above = point - corner.left_bottom_above.location,
		.right_bottom_above = point - corner.right_bottom_above.location,
	};
}

template <std::floating_point Type>
constexpr auto compute_dot_products(CornerCell<Type> const& corner, Cell<Type> const& offset) -> TCell<Type> {
	return TCell<Type>{
		.left_top_below = dot(corner.left_top_below.gradient, offset.left_top_below),
		.right_top_below = dot(corner.right_top_below.gradient, offset.right_top_below),
		.left_bottom_below = dot(corner.left_bottom_below.gradient, offset.left_bottom_below),
		.right_bottom_below = dot(corner.right_bottom_below.gradient, offset.right_bottom_below),
		
		.left_top_above = dot(corner.left_top_above.gradient, offset.left_top_above),
		.right_top_above = dot(corner.right_top_above.gradient, offset.right_top_above),
		.left_bottom_above = dot(corner.left_bottom_above.gradient, offset.left_bottom_above),
		.right_bottom_above = dot(corner.right_bottom_above.gradient, offset.right_bottom_above),
	};
}

template <std::floating_point Type>
constexpr auto interpolate(Vec<Type> const point, TCell<Type> const& dot_products) -> Type {
	auto const uvw = point.fract().fade(); 

	auto const below_a = std::lerp(dot_products.left_top_below, dot_products.right_top_below, uvw.x);
	auto const below_b = std::lerp(dot_products.left_bottom_below, dot_products.right_bottom_below, uvw.x);
	auto const below = std::lerp(below_a, below_b, uvw.y);
	
	auto const above_a = std::lerp(dot_products.left_top_above, dot_products.right_top_above, uvw.x);
	auto const above_b = std::lerp(dot_products.left_bottom_above, dot_products.right_bottom_above, uvw.x);
	auto const above = std::lerp(above_a, above_b, uvw.y);

	//i might need to swap the position of below and above, not really sure
	return std::lerp(below, above, uvw.z);
}
} // namespace noiz::detail
