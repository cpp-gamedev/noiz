#include <noiz/vec2.hpp>
#include <test/test.hpp>

namespace test {
namespace {
static_assert(noiz::Vec2f{} == noiz::Vec2f{});
static_assert(noiz::Vec2f{1.0f, 2.0f} + noiz::Vec2f{3.0f, 4.0f} == noiz::Vec2f{4.0f, 6.0f});  // NOLINT
static_assert(noiz::Vec2f{3.0f, 4.0f} - noiz::Vec2f{2.0f, 1.0f} == noiz::Vec2f{1.0f, 3.0f});  // NOLINT
static_assert(noiz::Vec2f{2.0f, 3.0f} * noiz::Vec2f{4.0f, 5.0f} == noiz::Vec2f{8.0f, 15.0f}); // NOLINT
static_assert(noiz::Vec2f{4.0f, 9.0f} / noiz::Vec2f{2.0f, 3.0f} == noiz::Vec2f{2.0f, 3.0f});  // NOLINT

static_assert(noiz::dot(noiz::Vec2f{2.0f, 3.0f}, noiz::Vec2f{4.0f, 5.0f}) == 23.0f); // NOLINT
static_assert(noiz::Vec2f{2.0f, 3.0f}.sqr_magnitude() == 13.0f);					 // NOLINT

static_assert(noiz::Vec2f{1.0f, 0.0f}.is_normalized());

void vec2_magnitude() {
	auto const vec = noiz::Vec2f{3.0f, 4.0f}; // NOLINT
	EXPECT(vec.magnitude() == 5.0f);		  // NOLINT
}

void vec2_normalize() {
	auto vec = noiz::Vec2f{2.0f, 3.0f}; // NOLINT
	EXPECT(!vec.is_normalized());

	vec.normalize();
	EXPECT(vec.is_normalized());
}

auto run() -> bool {
	ADD_TEST(vec2_magnitude);
	ADD_TEST(vec2_normalize);

	return run_tests();
}
} // namespace
} // namespace test

auto main() -> int {
	if (!test::run()) { return EXIT_FAILURE; }
}
