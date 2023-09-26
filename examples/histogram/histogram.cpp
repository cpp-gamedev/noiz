#include <noiz/noise2.hpp>
#include <charconv>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>

#include <argument_parsing.hpp>

namespace {

struct Config {
	noiz::Seed seed{noiz::detail::Generator::make_random_seed()};
	noiz::GridExtent2 grid_extent{50, 1}; // NOLINT
	int count{100};						  // NOLINT
	float step{0.1f};					  // NOLINT

	// syntax: [count] [step]
	auto parse_args(Args args) -> bool {
		if (!args.next_as(count, "count")) { return false; }
		if (!args.next_as(step, "step")) { return false; }
		if (!args.args.empty()) {
			std::cerr << std::format("unrecognized argument: '{}'\n", args.next());
			return false;
		}
		return true;
	}
};

// contains logic to store data points and draw them
class Histogram {
  public:
	explicit Histogram(std::size_t const levels = 10) : rows(levels) {}

	auto add(float const value) -> Histogram& {
		// compute height relative to number of rows/levels
		auto const height = static_cast<std::size_t>((value + 1.0f) * 0.5f * static_cast<float>(rows.size()));
		// add a new column
		for (std::size_t index = 0; index < rows.size(); ++index) {
			// set output "pixel" value (0 or 1)
			auto const ch = index > height ? symbol : background;
			// append to each row
			rows.at(index) += ch;
		}
		return *this;
	}

	[[nodiscard]] auto build() const -> std::string {
		auto ret = std::string{};
		for (auto const& row : rows) {
			ret += row;
			ret += '\n';
		}
		return ret;
	}

	char symbol{'x'};
	char background{'-'};

  private:
	std::vector<std::string> rows{};
};
} // namespace

auto main(int argc, char** argv) -> int {
	auto config = Config{};
	auto noise = noiz::Noise2f{noiz::Seed{config.seed}, config.grid_extent};

	// skip exe name (argv[0])
	auto const args = Args{std::span{argv, static_cast<std::size_t>(argc)}.subspan(1)};

	// handle --help
	if (!args.args.empty() && args.args.front() == std::string_view{"--help"}) {
		std::cout << std::format("Usage: {} [count(=100)] [step(=0.1)]\n", std::filesystem::path{*argv}.stem().string());
		return EXIT_SUCCESS;
	}

	// parse args, if any
	if (!config.parse_args(args)) { return EXIT_FAILURE; }

	// build histogram
	auto histogram = Histogram{};
	for (int i = 0; i < config.count; ++i) {
		// build point on line (y = 0)
		auto const point = noiz::Vec2f{.x = static_cast<float>(i) * config.step};
		// add noise at point
		histogram.add(noise.at(point));
	}

	// output histogram
	std::cout << histogram.build();
}
