#pragma once
#include <cassert>
#include <cstdint>

#include <vector>

namespace noiz {
struct Index {
	std::vector<int> components;

	[[nodiscard]] constexpr auto modulo(Index const extent) const -> Index {
		for(int i = 0; i < extent.components.size(); i++) {
			assert(extent.components[i] > 0);
		}
		Index ret;
		ret.components.resize(extent.components.size());
		for(int i = 0; i < ret.components.size(); i++){
			ret.components[i] = components[i] % extent.components[i];
		}
		return ret;
	}

	[[nodiscard]] constexpr auto flatten(Index const extent) const -> std::size_t { 
		assert(extent.components.size() == components.size());
		const uint8_t dimension_count = components.size();

		uint64_t ret = components[0];

		for(int i = 1; i < dimension_count; i++){
			uint64_t factor = extent.components[0];
			for(int j = 1; j < i; j++){
				factor *= extent.components[j];
			}
			ret += components[i] * factor;
		}

		return ret;
	}
};

struct CellIndex {
	std::vector<std::size_t> components;

	static /*constexpr*/ auto make(Index index, Index const& grid_extent) -> CellIndex {
		assert((index.components.size() == grid_extent.components.size()) && (index.components.size() > 0));

		index = index.modulo(grid_extent);
		auto const cell_index = index.flatten(grid_extent);
		assert(cell_index >= 0);

		const uint8_t dimension_count = index.components.size();
		uint8_t corner_count = 1;
		std::vector<uint8_t> dimension_break_point;
		dimension_break_point.reserve(dimension_count);
		for(int i = 0; i < dimension_count; i++){
			corner_count *= 2;
			dimension_break_point.emplace_back(corner_count);
		}
		
		auto ret = CellIndex{};
		ret.components.resize(corner_count);

		//1st dimension is implied
		ret.components[0] = static_cast<std::size_t>(cell_index);
		ret.components[1] = ret.components[0] + 1;

		uint8_t current_dimension = 1;
		for(int i = 2; i < corner_count; i++) {
			current_dimension += i == dimension_break_point[current_dimension];

			//const uint64_t& reference_index = dimension_break_point[current_dimension - 1];
			//ret.components[i] = ret.components[reference_index];
			ret.components[i] = ret.components[dimension_break_point[current_dimension - 1]];

			size_t factor = 0;
			for(int j = 0; j < current_dimension; j++){
				factor += grid_extent.components[j];
			}
			ret.components[i] *= factor;
		}
		//std::cout << "cellindex make size : " << ret.components.size() << std::endl;
		return ret;
	}
};
} // namespace noiz
