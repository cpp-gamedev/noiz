#include <noiz/detail/data2.hpp>
#include <test/test.hpp>

namespace {
ADD_TEST(noise_offsets) {
	auto const grid = noiz::detail::make_populated_grid<float>(noiz::Index2{5, 5}); // NOLINT
	auto const expect_equidistant = [&](noiz::Cell2<float> const& offset) {
		EXPECT(compare(offset.left_top, noiz::Vec2f{0.5f, 0.5f}));		 // NOLINT
		EXPECT(compare(offset.right_top, noiz::Vec2f{-0.5f, 0.5f}));	 // NOLINT
		EXPECT(compare(offset.left_bottom, noiz::Vec2f{0.5f, -0.5f}));	 // NOLINT
		EXPECT(compare(offset.right_bottom, noiz::Vec2f{-0.5f, -0.5f})); // NOLINT
	};
	auto point = noiz::Vec2f{0.5f, 0.5f}; // NOLINT
	auto corner = grid.at(noiz::detail::to_index2(point));
	auto offset = noiz::detail::compute_offsets(corner, point.modulo(noiz::detail::to_vec2<float>(grid.grid_extent)));
	expect_equidistant(offset);

	for (int i = 0; i < 10; ++i) { // NOLINT
		point.y += 1.0f;
		for (int j = 0; j < 10; ++j) { // NOLINT
			point.x += 1.0f;
			corner = grid.at(noiz::detail::to_index2(point));
			offset = noiz::detail::compute_offsets(corner, point.modulo(noiz::detail::to_vec2<float>(grid.grid_extent)));
			expect_equidistant(offset);
		}
		point.x = 0.5f; // NOLINT
	}
}
} // namespace
