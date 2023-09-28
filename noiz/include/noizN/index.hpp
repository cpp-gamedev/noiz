#pragma once
#include <cassert>
#include <cstdint>

namespace noiz {
struct Index {
	std::vector<int> components;

	[[nodiscard]] constexpr auto modulo(Index const extent) const -> Index {
		for(int i = 0; i < extent.components.size(); i++) {
			assert(extent.component[i] > 0);
		}
		Index ret;
		ret.components.resize(extent.components.size());
		for(int i = 0; i < ret.components.size(); i++){
			ret.components[i] = components[i] % extent.components[i];
		}
	}

	[[nodiscard]] constexpr auto flatten(Index const extent) const -> int64_t { 
		assert(extent.components.size() == components.size());
		int64_t ret = components.back();
		const uint8_t dimension_count = components.size();
		


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
