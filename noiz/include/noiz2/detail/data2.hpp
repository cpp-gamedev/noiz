#pragma once
#include "generator.hpp"
#include "grid2.hpp"

namespace noiz::detail {
template <std::floating_point Type>
auto make_populated_grid(Index2 const grid_extent, Seed seed = Generator::make_random_seed()) -> Grid2<Type> {
	auto ret = make_grid2<Type>(grid_extent);
	auto generator = Generator{seed};
	for (auto& corner : ret.corners) { generator.next(corner.gradient); }
	return ret;
}

template <std::floating_point Type>
constexpr auto compute_offsets(CornerCell2<Type> const& corner, Vec2<Type> const point) -> Cell2<Type> {
	return Cell2<Type>{
		.left_top = point - corner.left_top.location,
		.right_top = point - corner.right_top.location,
		.left_bottom = point - corner.left_bottom.location,
		.right_bottom = point - corner.right_bottom.location,
	};
}

template <std::floating_point Type>
constexpr auto compute_dot_products(CornerCell2<Type> const& corner, Cell2<Type> const& offset) -> TCell2<Type> {
	return TCell2<Type>{
		.left_top = dot(corner.left_top.gradient, offset.left_top),
		.right_top = dot(corner.right_top.gradient, offset.right_top),
		.left_bottom = dot(corner.left_bottom.gradient, offset.left_bottom),
		.right_bottom = dot(corner.right_bottom.gradient, offset.right_bottom),
	};
}

template <std::floating_point Type>
constexpr auto interpolate(Vec2<Type> const point, TCell2<Type> const& dot_products) -> Type {
	auto const uv = point.fract().fade(); 
	auto const a = std::lerp(dot_products.left_top, dot_products.right_top, uv.x);
	auto const b = std::lerp(dot_products.left_bottom, dot_products.right_bottom, uv.x);
	return std::lerp(a, b, uv.y);
}
} // namespace noiz::detail
