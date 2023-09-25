#pragma once
#include "detail/data2.hpp"

namespace noiz {
using GridExtent2 = Index2;

template <std::floating_point Type>
class Noise2 {
  public:
	static constexpr GridExtent2 grid_extent_v{256, 256};

	explicit Noise2(GridExtent2 grid_extent = grid_extent_v) : Noise2(detail::Generator::make_random_seed(), grid_extent) {}

	explicit Noise2(Seed generator_seed, GridExtent2 grid_extent) : m_grid(detail::make_populated_grid<Type>(grid_extent, generator_seed)) {}

	[[nodiscard]] auto grid_extent() const -> GridExtent2 { return m_grid.grid_extent; }

	[[nodiscard]] auto at(Vec2<Type> point) const -> Type {
		point = point.modulo(detail::to_vec2<Type>(m_grid.grid_extent));
		auto const corners = m_grid.at(detail::to_index2(point));
		auto const offsets = detail::compute_offsets(corners, point);
		auto const dots = detail::compute_dot_products(corners, offsets);
		
		return detail::interpolate(point, dots);
	}

	[[nodiscard]] auto at_with_octave(Vec2<Type> const& point, int const& octaves, Type const& persistence, Type const& lacunarity) const -> Type {
		Type total = Type(0);
		Type frequency = (Type)2;
		Type amplitude = (Type)1;
		Type normalizer = (Type)0;
		for(int i = 0; i < octaves; i++){
			total += at(Vec2<Type>{.x = point.x * frequency, .y = point.y * frequency}) * amplitude;
			normalizer += amplitude;
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		return total/normalizer;
	}

  private:
	detail::Grid2<Type> m_grid{};
};

using Noise2f = Noise2<float>;
using Noise2d = Noise2<double>;
} // namespace noiz
