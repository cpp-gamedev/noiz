#pragma once
#include <cassert>
#include <cstdint>

namespace noiz {
struct Index4 {
	int x{};
	int y{};
	int z{};
	int w{};

	[[nodiscard]] constexpr auto modulo(Index4 const extent) const -> Index4 {
		assert(extent.x > 0 && extent.y > 0 && extent.z > 0 && extent.w > 0);
		return Index4{.x = x % extent.x, .y = y % extent.y, .z = z % extent.z, .w = w % extent.w};
	}

	[[nodiscard]] constexpr auto flatten(Index4 const extent) const -> int64_t { 
		return w * ((extent.z + 1) * (extent.y + 1) * (extent.x + 1)) 
			+ z * ((extent.x + 1) * (extent.y + 1))
			+ y * (extent.x + 1)
			+ x;
	}
};

struct CellIndex4 {

	//16 is 2^4
	std::array<std::size_t, 16> positions; //NOLINT

	static constexpr auto make(Index4 index, Index4 const grid_extent) -> CellIndex4 {
		index = index.modulo(grid_extent);
		auto const cell_index = index.flatten(grid_extent);
		assert(cell_index >= 0);
		auto ret = CellIndex4{};


		//1st dimension
		ret.positions[0] = static_cast<std::size_t>(cell_index);
		ret.positions[1] = ret.positions[0] + 1;

		//2nd dimension
		for(int i = 2; i < 4; i++){
			ret.positions.at(i) = ret.positions.at(i - 2) + static_cast<std::size_t>(grid_extent.x + 1);
		}

		//3rd dimension
		const auto dimension_offset3 = static_cast<std::size_t>((grid_extent.x + 1) * (grid_extent.y + 1));
		for(int i = 4; i < 8; i++){
			ret.positions.at(i) = ret.positions.at(i - 4) + dimension_offset3;
		}
		
		//4th dimension
		const auto dimension_offset4 = dimension_offset3 * static_cast<std::size_t>(grid_extent.z + 1);
		for(int i = 8; i < 16; i++){
			ret.positions.at(i) = ret.positions.at(i - 8) + dimension_offset4;
		}

		return ret;
	}
};
} // namespace noiz
