#include <noiz2/noise2.hpp>

#include <charconv>
#include <format>
#include <iostream>

namespace {
template <typename Type>
concept NumberT = std::integral<Type> || std::floating_point<Type>;

template <NumberT Type>
auto parse_as(Type& out, std::string_view const value) -> bool {
	if (value.empty()) { return false; }

	auto const* end = value.data() + value.size();
	auto const [ptr, ec] = std::from_chars(value.data(), end, out);

	return ec == std::errc{} && ptr == end;
}

struct Args {
	std::span<char const* const> args{};

	[[nodiscard]] constexpr auto next() -> std::string_view {
		if (args.empty()) { return {}; }
		auto const* ret = args.front();
		args = args.subspan(1);
		return ret;
	}

	template <NumberT Type>
	auto next_as(Type& out, std::string_view const key) -> bool {
		auto value = next();
		if (value.empty()) { return true; }

		if (!parse_as(out, value)) {
			std::cerr << std::format("invalid argument: '{}' for '{}'\n", value, key);
			return false;
		}

		return true;
	}
};
} // namespace

//the config function is implemented per-example, currently have 2 implementations