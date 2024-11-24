#pragma once
#include <cassert>
#include <cstdint>
#include <array>
#include <cmath>
#include <type_traits>

namespace noiz {
template<std::size_t N>
struct Index {
	std::array<int, N> component{};

	[[nodiscard]] constexpr auto modulo(Index const extent) const -> Index {

		Index ret;
		for(std::size_t i = 0; i < N; i++){
			assert(extent.component[i] > 0);
			ret.component[i] = component[i] % extent.component[i];
		}
		return ret;
	}

	[[nodiscard]] constexpr auto flatten(Index const extent) const -> int64_t { 

		int64_t ret = component[0];
		for (std::size_t i = 1; i < N; i++) {
			int64_t partial = component[i];
			for (std::size_t j = 0; j < i; j++) { 
				partial *= extent.component[j] + 1;
			}
			ret += partial;
		}
		return ret;
	}
};

constexpr auto Pow2(std::size_t input){
	if(input == 0){
		return 0;
	}
	return 1 << input;
}

template<std::size_t N>
struct CellIndex {

	//16 is 2^4
	std::array<std::size_t, (1 << N)> positions{};

	template<std::size_t N>
	static constexpr auto make(Index<N> index, Index<N> const grid_extent) -> CellIndex {
		index = index.modulo(grid_extent);
		auto const cell_index = index.flatten(grid_extent);
		assert(cell_index >= 0);
		auto ret = CellIndex<N>{};
		//1st dimension is kind of a special case
		ret.positions[0] = static_cast<std::size_t>(cell_index);
		ret.positions[1] = ret.positions[0] + 1;
	

		for(std::size_t i = 1; i < N; i++){
			const int currentPow2 = Pow2(i);
			const int nextPow2 = Pow2(i + 1);

			std::size_t dimensionOffset = grid_extent.component[0] + 1;
			for(int k = 1; k < i; k++) {
				dimensionOffset *= grid_extent.component[i] + 1;
			}

			for(std::size_t j = currentPow2; j < nextPow2; j++){
				ret.positions[j] = ret.positions[j - currentPow2] + dimensionOffset;
			}
		}
		return ret;
	}
};
} // namespace noiz
