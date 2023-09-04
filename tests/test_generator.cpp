#include <noiz/detail/generator.hpp>
#include <test/test.hpp>

namespace test {
namespace {
void gen_same_seed() {
	static constexpr auto seed_v = noiz::Seed{42};
	static constexpr std::size_t count_v{5};

	auto const generate = [] {
		auto ret = std::array<noiz::Vec2f, count_v>{};
		auto generator = noiz::detail::Generator{seed_v};
		for (auto& vec : ret) { generator.next(vec); }
		return ret;
	};

	auto const lhs = generate();
	auto const rhs = generate();

	for (std::size_t i = 0; i < count_v; ++i) {
		EXPECT(lhs.at(i) == rhs.at(i));
		EXPECT(lhs.at(i).is_normalized());
	}
}

auto run() -> bool {
	ADD_TEST(gen_same_seed);

	return run_tests();
}
} // namespace
} // namespace test

auto main() -> int {
	if (!test::run()) { return EXIT_FAILURE; }
}
