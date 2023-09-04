#include <noiz/detail/grid2.hpp>
#include <test/test.hpp>

namespace test {
namespace {
void grid_make() {
	auto grid = noiz::detail::make_grid2<float>({3, 3});
	ASSERT(grid.corners.size() == 16);
}

void grid_cell_loc() {
	auto grid_extent = noiz::Index2{3, 3};
	auto loc = noiz::CellIndex2::make({0, 0}, grid_extent);
	EXPECT(loc.lt == 0 && loc.rt == 1 && loc.lb == 4 && loc.rb == 5);
	loc = noiz::CellIndex2::make({1, 0}, grid_extent);
	EXPECT(loc.lt == 1 && loc.rt == 2 && loc.lb == 5 && loc.rb == 6);
	loc = noiz::CellIndex2::make({2, 0}, grid_extent);
	EXPECT(loc.lt == 2 && loc.rt == 3 && loc.lb == 6 && loc.rb == 7);
	loc = noiz::CellIndex2::make({0, 1}, grid_extent);
	EXPECT(loc.lt == 4 && loc.rt == 5 && loc.lb == 8 && loc.rb == 9);
	loc = noiz::CellIndex2::make({1, 1}, grid_extent);
	EXPECT(loc.lt == 5 && loc.rt == 6 && loc.lb == 9 && loc.rb == 10);
	loc = noiz::CellIndex2::make({2, 1}, grid_extent);
	EXPECT(loc.lt == 6 && loc.rt == 7 && loc.lb == 10 && loc.rb == 11);
	loc = noiz::CellIndex2::make({0, 2}, grid_extent);
	EXPECT(loc.lt == 8 && loc.rt == 9 && loc.lb == 12 && loc.rb == 13);
	loc = noiz::CellIndex2::make({1, 2}, grid_extent);
	EXPECT(loc.lt == 9 && loc.rt == 10 && loc.lb == 13 && loc.rb == 14);
	loc = noiz::CellIndex2::make({2, 2}, grid_extent);
	EXPECT(loc.lt == 10 && loc.rt == 11 && loc.lb == 14 && loc.rb == 15);
	loc = noiz::CellIndex2::make({3, 3}, grid_extent);
	EXPECT(loc.lt == 0 && loc.rt == 1 && loc.lb == 4 && loc.rb == 5);

	grid_extent = noiz::Index2{4, 4};
	loc = noiz::CellIndex2::make({0, 0}, grid_extent);
	EXPECT(loc.lt == 0 && loc.rt == 1 && loc.lb == 5 && loc.rb == 6);
	loc = noiz::CellIndex2::make({1, 1}, grid_extent);
	EXPECT(loc.lt == 6 && loc.rt == 7 && loc.lb == 11 && loc.rb == 12);
}

void grid_index() {
	static constexpr auto corners_v = std::array<noiz::Vec2f, 4>{
		noiz::Vec2f{1.0f, 2.0f},
		noiz::Vec2f{3.0f, 4.0f},
		noiz::Vec2f{5.0f, 6.0f},
		noiz::Vec2f{7.0f, 8.0f},
	};

	auto grid = noiz::detail::make_grid2<float>({3, 3});
	grid.corners[5].gradient = corners_v[0];  // NOLINT
	grid.corners[6].gradient = corners_v[1];  // NOLINT
	grid.corners[9].gradient = corners_v[2];  // NOLINT
	grid.corners[10].gradient = corners_v[3]; // NOLINT

	auto const cell = grid.at(noiz::Index2{1, 1});
	EXPECT(compare(cell.left_top.gradient, corners_v[0]));
	EXPECT(compare(cell.right_top.gradient, corners_v[1]));
	EXPECT(compare(cell.left_bottom.gradient, corners_v[2]));
	EXPECT(compare(cell.right_bottom.gradient, corners_v[3]));

	EXPECT(compare(cell.left_top.location, noiz::Vec2f{1, 1}));
	EXPECT(compare(cell.right_top.location, noiz::Vec2f{2, 1}));
	EXPECT(compare(cell.left_bottom.location, noiz::Vec2f{1, 2}));
	EXPECT(compare(cell.right_bottom.location, noiz::Vec2f{2, 2}));
}

auto run() -> bool {
	ADD_TEST(grid_make);
	ADD_TEST(grid_cell_loc);
	ADD_TEST(grid_index);

	return run_tests();
}
} // namespace
} // namespace test

auto main() -> int {
	if (!test::run()) { return EXIT_FAILURE; }
}
