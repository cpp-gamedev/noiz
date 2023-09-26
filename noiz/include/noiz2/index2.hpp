#pragma once
#include <cassert>

namespace noiz {
struct Index2 {
	int x{};
	int y{};

	[[nodiscard]] constexpr auto modulo(Index2 const extent) const -> Index2 {
		assert(extent.x > 0 && extent.y > 0);
		return Index2{.x = x % extent.x, .y = y % extent.y};
	}

	[[nodiscard]] constexpr auto flatten(Index2 const extent) const -> int { return y * (extent.x + 1) + x; }
};

struct CellIndex2 {
	std::size_t lt{};
	std::size_t rt{};
	std::size_t lb{};
	std::size_t rb{};

	static constexpr auto make(Index2 index, Index2 const grid_extent) -> CellIndex2 {
		index = index.modulo(grid_extent);
		auto const cell_index = index.flatten(grid_extent);
		assert(cell_index >= 0);
		auto ret = CellIndex2{};
		ret.lt = static_cast<std::size_t>(cell_index);
		ret.rt = ret.lt + 1;
		ret.lb = ret.lt + static_cast<std::size_t>(grid_extent.x + 1);
		ret.rb = ret.rt + static_cast<std::size_t>(grid_extent.x + 1);
		return ret;
	}
};
} // namespace noiz
