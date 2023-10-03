#include "noise2.hpp"
#include "noise3.hpp"
#include "noise4.hpp"

#pragma once

namespace noiz{
	/*
template <std::floating_point value_type, typename NoiseT = Noise2<value_type>>
class Processor {
public:
using noise_type = NoiseT;
using vec_type = typename NoiseT::vec_type;
*/
template <typename NoiseT>
class Processor {
 public:
	using noise_type = NoiseT;
	using vec_type = typename NoiseT::vec_type;
	using value_type = typename vec_type::value_type;
	explicit Processor(noise_type& noise) : noise{noise}{}

	std::reference_wrapper<noise_type> noise;

	//default values
	uint8_t octave{ 6 };
	value_type step = (value_type)0.1;
	value_type persistence = (value_type)1.5;
	value_type lacunarity = (value_type)2.0;
	value_type frequency = (value_type)1.0;
	value_type amplitude = (value_type)1.0;

	//hybrid multifractal variables
	value_type hmf_H = (value_type)0.25;
	value_type hmf_offset = (value_type)0.7;
protected:
	////hybrid multifractal variable that wont be accessed from outside
	std::vector<value_type> hmf_exponent_array;
public:

	auto raw_noise(vec_type const& point) const -> value_type{
		return noise.get().at(point * step);
	}

	auto basic_processing(vec_type const& point) const -> value_type {
		
		value_type total = value_type(0);
		value_type frequency = this->frequency;
		value_type amplitude = this->amplitude;
		value_type normalizer = (value_type)0;
		for(int i = 0; i < octave; i++){
			total += noise.get().at(point * step * frequency) * amplitude;
			normalizer += amplitude;
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		return total/normalizer;
	}

	auto turbulence_processing(vec_type const& point) const -> value_type {
		value_type amplitude = this->amplitude;
		value_type frequency = this->frequency;
		
		value_type sum = 0.f;
		value_type normalizer = 0.f;
		for (uint8_t i = 1; i < octave; i++) {
			sum += noise.get().at(point * (step * frequency)) * amplitude;
			normalizer += amplitude;
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		return sum;
	}

	auto billowy_processing(vec_type const& point) const -> value_type {
		return std::abs(noise.get().at(point * step));
	}

	auto rigid_processing(vec_type const& point) const -> value_type {
		return 1.f - std::abs(noise.get().at(point * step));
	}

	auto hybrid_multi_fractal_processing(vec_type const& point) -> const value_type {
		auto temp_point = point;

		value_type frequency, result, signal, weight;
		if (hmf_exponent_array.size() != octave) {
			// precompute and store spectral weights
			//recalculates if the octave count has changed

			hmf_exponent_array.resize(octave);
			frequency = 1.0;
			for (uint16_t i = 0; i < octave; i++) {
				// compute weight for each frequency
				hmf_exponent_array[i] = std::pow(frequency, -hmf_H);
				frequency *= lacunarity;
			}
		}
		// get first octave of function
		result = (noise.get().at(temp_point * step) + hmf_offset) * hmf_exponent_array[0];
		weight = result;
		// increase frequency
		temp_point = temp_point * lacunarity;
		// spectral construction inner loop, where the fractal is built
		for (int i = 1; i < octave; i++) {
			// prevent divergence
			if (weight > 1.0) weight = 1.0;
			// get next frequency
			signal = (noise.get().at(temp_point * step) + hmf_offset) * hmf_exponent_array[i];
			result += weight * signal;
			// this is why hmf_H must specify a high fractal dimension
			weight *= signal;
			temp_point = temp_point * lacunarity;
		}

		return result;
	}

};
using Processor2f = Processor<Noise2f>;
using Processor2d = Processor<Noise2d>;

using Processor3f = Processor<Noise3f>;
using Processor3d = Processor<Noise3d>;

using Processor4f = Processor<Noise4f>;
using Processor4d = Processor<Noise4d>;

} //namespace noiz