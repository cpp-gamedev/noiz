#pragma once
#include <random>
#include "../seed.hpp"
#include "../vecTemplate.hpp"

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
	template <std::floating_point Type, std::size_t N>
	void next(Vec<Type, N>& out) {
		auto distribution = std::uniform_real_distribution<Type>{Type{-1}, Type{1}};
		for(std::size_t i = 0; i < N; i++){
			out.component[i] = distribution(m_engine);
		}
		out.normalize();
	}


	///
	/// \brief Obtain the next random unit Vec.
	/// \returns The next random unit Vec.
	///
	template <std::floating_point Type, std::size_t N>
	auto next() -> Vec<Type, N> {
		auto ret = Vec<Type, N>{};
		next(ret);
		return ret;
	}

  private:
	std::default_random_engine m_engine{};
};
} // namespace noiz::detail
