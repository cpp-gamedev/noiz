#pragma once
#include <cassert>
#include <cstdint>

namespace noiz {
struct Index3 {
	int x{};
	int y{};
	int z{};

	[[nodiscard]] constexpr auto modulo(Index3 const extent) const -> Index3 {
		assert(extent.x > 0 && extent.y > 0);
		return Index3{.x = x % extent.x, .y = y % extent.y, .z = z % extent.z};
	}

	[[nodiscard]] constexpr auto flatten(Index3 const extent) const -> int64_t { 
		return z * ((extent.x + 1) * (extent.y + 1)) + y * (extent.x + 1) + x;
	}
};

struct CellIndex3 {
	std::size_t ltb{};
	std::size_t rtb{};
	std::size_t lbb{};
	std::size_t rbb{};

	std::size_t lta{};
	std::size_t rta{};
	std::size_t lba{};
	std::size_t rba{};

	static constexpr auto make(Index3 index, Index3 const grid_extent) -> CellIndex3 {
		index = index.modulo(grid_extent);
		auto const cell_index = index.flatten(grid_extent);
		assert(cell_index >= 0);
		auto ret = CellIndex3{};
		ret.ltb = static_cast<std::size_t>(cell_index);
		ret.rtb = ret.ltb + 1;
		ret.lbb = ret.ltb + static_cast<std::size_t>(grid_extent.x + 1);
		ret.rbb = ret.rtb + static_cast<std::size_t>(grid_extent.x + 1);

		ret.lta = ret.ltb + static_cast<std::size_t>((grid_extent.x + 1) * (grid_extent.y + 1));
		ret.rta = ret.rtb + static_cast<std::size_t>((grid_extent.x + 1) * (grid_extent.y + 1));
		ret.lba = ret.lbb + static_cast<std::size_t>((grid_extent.x + 1) * (grid_extent.y + 1));
		ret.rba = ret.rbb + static_cast<std::size_t>((grid_extent.x + 1) * (grid_extent.y + 1));
		return ret;
	}
};
} // namespace noiz
