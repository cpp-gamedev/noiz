#pragma once
#include "detail/data4.hpp"

namespace noiz {
using GridExtent4 = Index4;

template <std::floating_point Type>
class Noise4 {
  public:
  using vec_type = Vec4<Type>;
  using value_type = Type;

	static constexpr GridExtent4 grid_extent_v{64, 64, 64, 64};

	explicit Noise4(GridExtent4 grid_extent = grid_extent_v) : Noise4(detail::Generator::make_random_seed(), grid_extent) {}

	explicit Noise4(Seed generator_seed, GridExtent4 grid_extent) : m_grid(detail::make_populated_grid<Type>(grid_extent, generator_seed)) {}

	[[nodiscard]] auto grid_extent() const -> GridExtent4 { return m_grid.grid_extent; }

	[[nodiscard]] auto at(Vec4<Type> point) const -> Type {
		point = point.modulo(detail::to_vec4<Type>(m_grid.grid_extent));
		auto const corners = m_grid.at(detail::to_index4(point));
		auto const offsets = detail::compute_offsets(corners, point);
		auto const dots = detail::compute_dot_products(corners, offsets);
		
		return detail::interpolate(point, dots);
	}

  private:
	detail::Grid4<Type> m_grid{};
};

using Noise4f = Noise4<float>;
using Noise4d = Noise4<double>;
} // namespace noiz
