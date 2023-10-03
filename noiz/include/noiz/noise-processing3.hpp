#pragma once
#include "noise3.hpp"

namespace noiz{
template <std::floating_point Type>
class Noise_Processor3 {
public:
	explicit Noise_Processor3(noiz::Noise3<Type>& noise) : noise{noise}{}

	std::reference_wrapper<Noise3<Type>> noise;

	//default values
	uint8_t octave{ 6 };
	Type step = (Type)0.1;
	Type persistence = (Type)1.5;
	Type lacunarity = (Type)2.0;
	Type frequency = (Type)1.0;
	Type amplitude = (Type)1.0;

	//hybrid multifractal variables
	Type hmf_H = (Type)0.25;
	Type hmf_offset = (Type)0.7;
	std::vector<Type> hmf_exponent_array;

	auto raw_noise(noiz::Vec3<Type> const& point) const -> Type{
		return noise.get().at(point * step);
	}

	auto basic_processing(noiz::Vec3<Type> const& point) const -> Type {
		
		Type total = Type(0);
		Type frequency = this->frequency;
		Type amplitude = this->amplitude;
		Type normalizer = (Type)0;
		for(int i = 0; i < octave; i++){
			total += noise.get().at(point * step * frequency) * amplitude;
			normalizer += amplitude;
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		return total/normalizer;
	}

	auto turbulence_processing(noiz::Vec3<Type> const& point) const -> Type {
		Type amplitude = this->amplitude;
		Type frequency = this->frequency;
		
		Type sum = 0.f;
		Type normalizer = 0.f;
		for (int i = 0; i < octave; i++) {
			sum += noise.get().at(point * (step * frequency)) * amplitude;
			normalizer += amplitude;
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		return sum;
	}

	auto billowy_processing(noiz::Vec3<Type> const& point) const -> Type {
		return std::abs(noise.get().at(point * step));
	}

	auto rigid_processing(noiz::Vec3<Type> const& point) const -> Type {
		return 1.f - std::abs(noise.get().at(point * step));
	}

	auto hybrid_multi_fractal_processing(noiz::Vec3<Type> const& point) -> Type {
		noiz::Vec3<Type> temp_point = point;

		Type frequency, result, signal, weight;
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
		for (uint8_t i = 1; i < octave; i++) {
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
using Noise_Processor3f = Noise_Processor3<float>;
using Noise_Processor3d = Noise_Processor3<double>;
} //namespace noiz