#pragma once
#include <random>
#include "../seed.hpp"
#include "../vec2.hpp"

namespace noiz::detail {
///
/// \brief Generators random normalized Vecs.
///
class Generator {
  public:
	///
	/// \brief Make a random Seed.
	/// \returns A random Seed.
	///
	[[nodiscard]] static auto make_random_seed() -> Seed { return Seed{std::random_device{}()}; }

	///
	/// \brief Construct a Generator instance with a randomized seed.
	///
	explicit Generator() : Generator(make_random_seed()) {}

	///
	/// \brief Construct a Generator instance with a custom seed.
	/// \param seed Seed to initialize generator with .
	///
	explicit Generator(Seed const seed) : m_engine(std::default_random_engine{static_cast<std::uint32_t>(seed)}) {}

	///
	/// \brief Fill the next random unit Vec.
	/// \param out The Vec to assign to.
	///
	template <std::floating_point Type>
	void next(Vec2<Type>& out) {
		auto distribution = std::uniform_real_distribution<Type>{Type{-1}, Type{1}};
		out = Vec2<Type>{.x = distribution(m_engine), .y = distribution(m_engine)}.normalized();
	}

	///
	/// \brief Obtain the next random unit Vec.
	/// \returns The next random unit Vec.
	///
	template <std::floating_point Type>
	auto next() -> Vec2<Type> {
		auto ret = Vec2<Type>{};
		next(ret);
		return ret;
	}

  private:
	std::default_random_engine m_engine{};
};
} // namespace noiz::detail
