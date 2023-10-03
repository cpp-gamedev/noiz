#pragma once
#include <cassert>
#include <cstdint>

namespace noiz {
struct Index3 {
	int x{};
	int y{};
	int z{};

	[[nodiscard]] constexpr auto modulo(Index3 const extent) const -> Index3 {
		assert(extent.x > 0 && extent.y > 0 && extent.z > 0);
		return Index3{.x = x % extent.x, .y = y % extent.y, .z = z % extent.z};
	}

	[[nodiscard]] constexpr auto flatten(Index3 const extent) const -> int64_t { 
		return z * ((extent.x + 1) * (extent.y + 1)) + y * (extent.x + 1) + x;
	}
};

struct CellIndex3 {
	std::size_t ltf{};
	std::size_t rtf{};
	std::size_t lbf{};
	std::size_t rbf{};

	std::size_t ltb{};
	std::size_t rtb{};
	std::size_t lbb{};
	std::size_t rbb{};

	static constexpr auto make(Index3 index, Index3 const grid_extent) -> CellIndex3 {
		index = index.modulo(grid_extent);
		auto const cell_index = index.flatten(grid_extent);
		assert(cell_index >= 0);
		auto ret = CellIndex3{};
		ret.ltf = static_cast<std::size_t>(cell_index);
		ret.rtf = ret.ltf + 1;
		ret.lbf = ret.ltf + static_cast<std::size_t>(grid_extent.x + 1);
		ret.rbf = ret.rtf + static_cast<std::size_t>(grid_extent.x + 1);

		const auto dimension_offset3 = static_cast<std::size_t>((grid_extent.x + 1) * (grid_extent.y + 1));
		ret.ltb = ret.ltf + dimension_offset3;
		ret.rtb = ret.rtf + dimension_offset3;
		ret.lbb = ret.lbf + dimension_offset3;
		ret.rbb = ret.rbf + dimension_offset3;
		return ret;
	}
};
} // namespace noiz
