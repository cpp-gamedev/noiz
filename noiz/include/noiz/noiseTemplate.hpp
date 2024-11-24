#pragma once
#include "detail/dataTemplate.hpp"

#include <cstddef>
#include <type_traits>

namespace noiz {
using GridExtent2 = Index<2>;
using GridExtent3 = Index<3>;
using GridExtent4 = Index<4>;

template<std::size_t N>
using GridExtent = Index<N>;

namespace Template_Helper{
	template <std::size_t N>
	constexpr auto create_filled_array(int value) -> std::array<int, N> {
		std::array<int, N> ret{};
		std::fill(ret.begin(), ret.end(), std::max(value / (1 << N), 16));
		return ret;
	}
} //Template_Helper namespace

template <std::floating_point Type, std::size_t N>
class Noise {
  public:
  using vec_type = Vec<Type, N>;
  using value_type = Type;

	static constexpr GridExtent<N> grid_extent_v{.component = Template_Helper::create_filled_array<N>(1024)};

	explicit Noise(GridExtent<N> grid_extent = grid_extent_v) : Noise(detail::Generator::make_random_seed(), grid_extent) {}

	explicit Noise(Seed generator_seed, GridExtent<N> grid_extent) : m_grid(detail::make_populated_grid<Type>(grid_extent, generator_seed)) {}

	[[nodiscard]] auto grid_extent() const -> GridExtent<N> { return m_grid.grid_extent; }

	[[nodiscard]] auto at(Vec<Type, N> point) const -> Type {

		point = point.modulo(detail::to_vec<Type>(m_grid.grid_extent));
		auto const corners = m_grid.at(detail::to_index(point));
		auto const offsets = detail::compute_offsets(corners, point);
		auto const dots = detail::compute_dot_products(corners, offsets);
		
		return detail::interpolate(point, dots);
	}

  private:
	detail::Grid<Type, N> m_grid{};
};

using Noise2f = Noise<float, 2>;
using Noise2d = Noise<double, 2>;
using Noise3f = Noise<float, 3>;
using Noise3d = Noise<double, 3>;
using Noise4f = Noise<float, 4>;
using Noise4d = Noise<double, 4>;

template <std::size_t N>
using Noisef = Noise<float, N>;
template <std::size_t N>
using Noised = Noise<double, N>;
} // namespace noiz
