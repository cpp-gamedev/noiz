#pragma once
#include "noizN/detail/data.hpp"

namespace noiz {
using GridExtent = Index;

template <std::floating_point Type>
class Noise3 {
  public:
	static constexpr GridExtent grid_extent_v{256, 256, 256};

	explicit Noise(uint8_t dimensions, GridExtent3 grid_extent = grid_extent_v) : Noise3(detail::Generator::make_random_seed(), grid_extent) {}

	explicit Noise(uint8_t dimensions, Seed generator_seed, GridExtent3 grid_extent) : m_grid(detail::make_populated_grid<Type>(grid_extent, generator_seed)) {}

	[[nodiscard]] auto grid_extent() const -> GridExtent { return m_grid.grid_extent; }

	[[nodiscard]] auto at(Vec<Type> point) const -> Type {
		point = point.modulo(detail::to_vec<Type>(m_grid.grid_extent));
		auto const corners = m_grid.at(detail::to_index(point));
		auto const offsets = detail::compute_offsets(corners, point);
		auto const dots = detail::compute_dot_products(corners, offsets);
		
		return detail::interpolate(point, dots);
	}

  private:
	detail::Grid<Type> m_grid{};
};

using Noisef = Noise<float>;
using Noised = Noise<double>;
} // namespace noiz
