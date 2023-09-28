#pragma once
#include "detail/data.hpp"

namespace noiz {
using GridExtent = Index;

template <std::floating_point Type>
class Noise {
  public:
	static constexpr GridExtent grid_extent_v{.components{256, 256}};


	explicit Noise(GridExtent grid_extent = grid_extent_v) : Noise(dimension_count, detail::Generator::make_random_seed(), grid_extent) {}

	explicit Noise(SeedN generator_seed, GridExtent grid_extent) : m_grid(detail::make_populated_grid<Type>(grid_extent, generator_seed)), dimension_count{grid_extent.components.size()} {}

	[[nodiscard]] auto grid_extent() const -> GridExtent { return m_grid.grid_extent; }

	[[nodiscard]] auto at(Vec<Type> point) const -> Type {
		point = point.modulo(detail::to_vec<Type>(m_grid.grid_extent));
		auto const corners = m_grid.at(detail::to_index(point));
		auto const offsets = detail::compute_offsets(corners, point);
		auto const dots = detail::compute_dot_products(corners, offsets);
		
		return detail::interpolate(point, dots);
	}

	uint8_t get_dimensions(){
		return dimension_count;
	}
  private:
	detail::Grid<Type> m_grid{};
	uint8_t dimensions; //constant??? const uint8_t giving me warnings
};

using Noisef = Noise<float>;
using Noised = Noise<double>;
} // namespace noiz
