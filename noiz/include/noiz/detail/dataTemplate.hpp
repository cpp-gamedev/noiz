#pragma once
#include "generator.hpp"
#include "gridTemplate.hpp"

namespace noiz::detail {
template <std::floating_point Type, std::size_t N>
auto make_populated_grid(Index<N> const grid_extent, Seed seed = Generator::make_random_seed()) -> Grid<Type, N> {
	auto ret = make_grid<Type>(grid_extent);
	auto generator = Generator{seed};
	for (auto& corner : ret.corners) { generator.next(corner.gradient); }
	return ret;
}

template <std::floating_point Type, std::size_t N>
constexpr auto compute_offsets(CornerCell<Type, N> const& corner, Vec<Type, N> const point) -> Cell<Type, N> {

	Cell<Type, N> ret;
	for(uint8_t i = 0; i < ret.corners.size(); i++) {
		ret.corners[i] = point - corner.corners[i].location;
	}
	return ret;
}

template <std::floating_point Type, std::size_t N>
constexpr auto compute_dot_products(CornerCell<Type, N> const& corner, Cell<Type, N> const& offset) -> TCell<Type, N> {

	TCell<Type, N> ret;
	for(int i = 0; i < ret.corners.size(); i++){
		ret.corners[i] = dot(corner.corners[i].gradient, offset.corners[i]);
	}
	return ret;
}

constexpr auto GetLog2(std::size_t input) -> std::size_t {
	std::size_t ret = 0;

	while(input > 1){
		input /= 2;
		ret++;
	}
	return ret;
}

namespace Internal_Helper{
	template<std::floating_point Type, std::size_t N>
	constexpr auto LerpRecursive(Vec<Type, N> const point, TCell<Type, N> const& dot_products, Vec<Type, N> const cell_interp_pos, std::size_t dimension_count, std::size_t corner_index) -> Type {

		if(dimension_count > 0){
			const auto value_a = LerpRecursive(point, dot_products, cell_interp_pos, dimension_count - 1, corner_index);
			uint16_t corner_offset = 2;
			for(std::size_t i = 1; i < dimension_count; i++){
				corner_offset *= 2;
			}

			const auto value_b = LerpRecursive(point, dot_products, cell_interp_pos, dimension_count - 1, corner_offset);
			return std::lerp(
				value_a,
				value_b,
				cell_interp_pos.component[dimension_count]
			);
		}
		// else
		return std::lerp(
			dot_products.corners[corner_index],
			dot_products.corners[corner_index + 1],
			cell_interp_pos.component[0]
		);
	}
}

template <std::floating_point Type, std::size_t N>
constexpr auto interpolate(Vec<Type, N> const point, TCell<Type, N> const& dot_products) -> Type {
	auto const cell_interpolated_position = point.fract().fade();

	return Internal_Helper::LerpRecursive(point, dot_products, cell_interpolated_position, N - 1, 0);
}
} // namespace noiz::detail
