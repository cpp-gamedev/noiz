#pragma once
#include "detail/data3.hpp"

namespace noiz {
using GridExtent3 = Index3;

template <std::floating_point Type>
class Noise3 {
  public:
	static constexpr GridExtent3 grid_extent_v{256, 256, 256};

	explicit Noise3(GridExtent3 grid_extent = grid_extent_v) : Noise3(detail::Generator::make_random_seed(), grid_extent) {}

	explicit Noise3(Seed generator_seed, GridExtent3 grid_extent) : m_grid(detail::make_populated_grid<Type>(grid_extent, generator_seed)) {}

	[[nodiscard]] auto grid_extent() const -> GridExtent3 { return m_grid.grid_extent; }

	[[nodiscard]] auto at(Vec3<Type> point) const -> Type {
		point = point.modulo(detail::to_vec3<Type>(m_grid.grid_extent));
		auto const corners = m_grid.at(detail::to_index3(point));
		auto const offsets = detail::compute_offsets(corners, point);
		auto const dots = detail::compute_dot_products(corners, offsets);
		
		return detail::interpolate(point, dots);
	}

  private:
	detail::Grid3<Type> m_grid{};
};

using Noise3f = Noise3<float>;
using Noise3d = Noise3<double>;
} // namespace noiz
