#pragma once
#include <cassert>
#include <vector>
#include "../cellTemplate.hpp"

namespace noiz::detail {
template <std::floating_point Type, std::size_t N>
[[nodiscard]] constexpr auto to_index(Vec<Type, N> const point) -> Index<N> {
	Index<N> ret;
	for(std::size_t i = 0; i < N; i++){
		ret.component[i] = static_cast<int>(point.component[i]);
	}
	return ret;
}

template <std::floating_point Type, std::size_t N>
[[nodiscard]] constexpr auto to_vec(Index<N> const index) -> Vec<Type, N> {
	Vec<Type, N> ret;
	for(std::size_t i = 0; i < N; i++){
		ret.component[i] = static_cast<Type>(index.component[i]);
	}
	return ret;
}

template <std::floating_point Type, std::size_t N>
struct Grid {
	std::vector<CornerData<Type, N>> corners{};
	Index<N> grid_extent{};

	[[nodiscard]] auto at(CellIndex<N> const index) const -> CornerCell<Type, N> {
		CornerCell<Type, N> ret;
		for(std::size_t i = 0; i < (1 << N); i++){
			ret.corners[i] = corners.at(index.positions.at(i));
		}
		return ret;
	}

	[[nodiscard]] auto at(Index<N> index) const -> CornerCell<Type, N> { return at(CellIndex<N>::make(index, grid_extent)); }
};

template <std::floating_point Type, std::size_t N>
[[nodiscard]] auto make_grid(Index<N> grid_extent) -> Grid<Type, N> {

	auto corner_count = grid_extent.component[0] + 1;
	for(std::size_t i = 1; i < N; i++){
		corner_count *= grid_extent.component[i] + 1;
	}
	if (corner_count <= 0) { return Grid<Type, N>{}; }

	auto ret = Grid<Type, N>{
		.corners = std::vector<CornerData<Type, N>>(static_cast<std::size_t>(corner_count)),
		.grid_extent = grid_extent,
	};

	Index<N> index;
	for(std::size_t i = 0; i < N; i++){
		index.component[i] = 0;
	}
	int64_t flattenedIndex;
	while(index.component[N - 1] <= grid_extent.component[N - 1]){

		flattenedIndex = static_cast<std::size_t>(index.flatten(grid_extent)); 
		ret.corners.at(flattenedIndex).location = to_vec<Type>(index);
		index.component[0]++;
		for(std::size_t i = 1; i < N; i++){
			if(index.component[i - 1] == grid_extent.component[i - 1]){
				index.component[i - 1] = 0;
				index.component[i]++;
			}
		}
	}
	return ret;
}
} // namespace noiz::detail
