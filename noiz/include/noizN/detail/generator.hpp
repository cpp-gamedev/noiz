#pragma once
#include <random>
#include "../seed.hpp"
#include "../vec.hpp"

namespace noiz::detail {
///
/// \brief Generators random normalized Vecs.
///
class GeneratorN {
  public:
	///
	/// \brief Make a random Seed.
	/// \returns A random Seed.
	///
	[[nodiscard]] static auto make_random_seed() -> SeedN { return SeedN{std::random_device{}()}; }

	///
	/// \brief Construct a Generator instance with a randomized seed.
	///
	explicit GeneratorN(uint8_t dimension_count) : GeneratorN(dimension_count, make_random_seed()) {}

	///
	/// \brief Construct a Generator instance with a custom seed.
	/// \param seed Seed to initialize generator with .
	///
	explicit GeneratorN(uint8_t dimension_count, SeedN const seed) : m_engine(std::default_random_engine{static_cast<std::uint32_t>(seed)}), dimension_count{dimension_count} {}

	///
	/// \brief Fill the next random unit Vec.
	/// \param out The Vec to assign to.
	///
	template <std::floating_point Type>
	void next(Vec<Type>& out) {
		if(out.components.size() == 0){
			out.components.resize(dimension_count);
		}

		auto distribution = std::uniform_real_distribution<Type>{Type{-1}, Type{1}};
		for(int i = 0; i < out.components.size(); i++){
			out.components[i] = distribution(m_engine);
		}
	}

	///
	/// \brief Obtain the next random unit Vec.
	/// \returns The next random unit Vec.
	///
	template <std::floating_point Type>
	auto nextN() -> Vec<Type> {
		auto ret = Vec<Type>{};
		ret.components.resize(dimension_count);
		next(ret);
		return ret;
	}


  private:
  	uint8_t dimension_count;
	std::default_random_engine m_engine{};
};
} // namespace noiz::detail
