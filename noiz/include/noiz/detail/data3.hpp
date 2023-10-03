#pragma once
#include "generator.hpp"
#include "grid3.hpp"

namespace noiz::detail {
template <std::floating_point Type>
auto make_populated_grid(Index3 const grid_extent, Seed seed = Generator::make_random_seed()) -> Grid3<Type> {
	auto ret = make_grid3<Type>(grid_extent);
	auto generator = Generator{seed};
	for (auto& corner : ret.corners) { generator.next(corner.gradient); }
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_offsets(CornerCell3<Type> const& corner, Vec3<Type> const point) -> Cell3<Type> {
	return Cell3<Type>{
		.left_top_front = point - corner.left_top_front.location,
		.right_top_front = point - corner.right_top_front.location,
		.left_bottom_front = point - corner.left_bottom_front.location,
		.right_bottom_front = point - corner.right_bottom_front.location,
		.left_top_back = point - corner.left_top_back.location,
		.right_top_back = point - corner.right_top_back.location,
		.left_bottom_back = point - corner.left_bottom_back.location,
		.right_bottom_back = point - corner.right_bottom_back.location,
	};
}

template <std::floating_point Type>
constexpr auto compute_dot_products(CornerCell3<Type> const& corner, Cell3<Type> const& offset) -> TCell3<Type> {
	return TCell3<Type>{
		.left_top_front = dot(corner.left_top_front.gradient, offset.left_top_front),
		.right_top_front = dot(corner.right_top_front.gradient, offset.right_top_front),
		.left_bottom_front = dot(corner.left_bottom_front.gradient, offset.left_bottom_front),
		.right_bottom_front = dot(corner.right_bottom_front.gradient, offset.right_bottom_front),
		
		.left_top_back = dot(corner.left_top_back.gradient, offset.left_top_back),
		.right_top_back = dot(corner.right_top_back.gradient, offset.right_top_back),
		.left_bottom_back = dot(corner.left_bottom_back.gradient, offset.left_bottom_back),
		.right_bottom_back = dot(corner.right_bottom_back.gradient, offset.right_bottom_back),
	};
}

template <std::floating_point Type>
constexpr auto interpolate(Vec3<Type> const point, TCell3<Type> const& dot_products) -> Type {
	auto const uvw = point.fract().fade(); 

	auto const below_a = std::lerp(dot_products.left_top_front, dot_products.right_top_front, uvw.x);
	auto const below_b = std::lerp(dot_products.left_bottom_front, dot_products.right_bottom_front, uvw.x);
	auto const below = std::lerp(below_a, below_b, uvw.y);
	
	auto const above_a = std::lerp(dot_products.left_top_back, dot_products.right_top_back, uvw.x);
	auto const above_b = std::lerp(dot_products.left_bottom_back, dot_products.right_bottom_back, uvw.x);
	auto const above = std::lerp(above_a, above_b, uvw.y);

	//i might need to swap the position of below and above, not really sure
	return std::lerp(below, above, uvw.z);
}
} // namespace noiz::detail
