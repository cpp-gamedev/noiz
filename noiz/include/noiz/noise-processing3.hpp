#pragma once
#include "noise3.hpp"


namespace noiz{
//static functions, maybe make members of noise2? not really sure
template <std::floating_point Type>
class Noise_Processor3 {
public:
	explicit Noise_Processor3(noiz::Noise3<Type>& noise) : noise{noise}{}
	//if this is multi-threaded, accessing these variables directly could lead to strange behavior
	//i.e. a handful of threads processing noise, then another comes in and changes octave??
	//not sure if thats a use case worth covering?
	noiz::Noise3<Type>& noise;

	//default values
	uint8_t octave{ 6 };
	Type step = (Type)0.1;
	Type persistence = (Type)1.5;
	Type lacunarity = (Type)2.0;
	Type frequency = (Type)1.0;
	Type amplitude = (Type)1.0;

	//hybrid multifractal variables, i have no idea what H is or id expand it to be mroe descriptive
	Type hbf_H = (Type)0.25;
	Type hbf_offset = (Type)0.7;
	std::vector<Type> hbf_exponent_array;

	auto raw_noise(noiz::Vec3<Type> const& point) -> Type{
		//redundant
		return noise.at(point * step);
	}

	auto basic_processing(noiz::Vec3<Type> const& point) -> Type {
		
		Type total = Type(0);
		Type frequency = (Type)2;
		Type amplitude = (Type)1;
		Type normalizer = (Type)0;
		for(int i = 0; i < octave; i++){
			total += noise.at(point * step * frequency) * amplitude;
			normalizer += amplitude;
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		return total/normalizer;
	}

	auto turbulence_processing(noiz::Vec3<Type> const& point) -> Type {
		Type amplitude = this->amplitude;
		Type frequency = this->frequency;
		
			Type sum = 0.f;
			Type normalizer = 0.f;
			for (int i = 0; i < octave; i++) {
				sum += noise.at(point * (step * frequency)) * amplitude;
				normalizer += amplitude;
				amplitude *= persistence;
				frequency *= lacunarity;
			}
			//normally return sum;
			//but im doing some additional adjustment in this function
			return sum;
	}

	auto billowy_processing(noiz::Vec3<Type> const& point) -> Type {
		return std::abs(noise.at(point * step));
	}

	auto rigid_processing(noiz::Vec3<Type> const& point) -> Type {
		return 1.f - std::abs(noise.at(point * step));
	}

	auto hybrid_multi_fractal_processing(noiz::Vec3<Type> const& point) -> Type {
		//https://www.classes.cs.uchicago.edu/archive/2015/fall/23700-1/final-project/MusgraveTerrain00.pdf
		
		//this function assumes the octave could be a floating point value
		
		//double HybridMultifractal( Vector point, double H, double lacunarity,
		//double octaves, double offset )
		noiz::Vec3<Type> tempPoint = point;

		Type frequency, result, signal, weight;//, remainder; if octave is floating

		/* precompute and store spectral weights */
		if (hbf_exponent_array.size() != octave) {
			hbf_exponent_array.resize(octave);
			/* seize required memory for exponent_array */
			//exponent_array = (double*)malloc(octave * sizeof(double));
			frequency = 1.0;
			for (uint16_t i = 0; i < octave; i++) {
				/* compute weight for each frequency */
				hbf_exponent_array[i] = std::pow(frequency, -hbf_H);
				frequency *= lacunarity;
			}
		}
		/* get first octave of function */
		result = (noise.at(tempPoint * step) + hbf_offset) * hbf_exponent_array[0];
		weight = result;
		/* increase frequency */
		tempPoint = tempPoint * lacunarity;
		/* spectral construction inner loop, where the fractal is built */
		for (uint16_t i = 1; i < octave; i++) {
			/* prevent divergence */
			if (weight > 1.0) {weight = 1.0;}
			/* get next higher frequency */
			signal = (noise.at(tempPoint * step) + hbf_offset) * hbf_exponent_array[i];
			/* add it in, weighted by previous freq's local value */
			result += weight * signal;
			/* update the (monotonically decreasing) weighting value */
			/* (this is why H must specify a high fractal dimension) */
			weight *= signal;
			/* increase frequency */
			tempPoint = tempPoint * lacunarity;
		}
		
		/* octave is currently an int, may swap for this
		* take care of remainder in “octaves”
		remainder = octave - (int)octave;
		if (remainder)
			// “i” and spatial freq. are preset in loop above
			result += remainder * noise.at(point) * hbf_exponent_array[octave - 1];
		*/

	
		//values from 0 to ~4 can be expected. highest value ive seen is 3.7
		return result;
	}

/*
	auto blended_basic_processing(noiz::Vec3<Type> const& point, std::vector<Noise3<Type>> const& noise){
		*each noise source would need its own set of variables

		std::vector<octaves>
		std::vector<steps>
		std::vector<lacunarity>
		...

		Type sum = 0;
		for(int i = 0; i < noise.size(); i++){
			sum += basic_processing(point, noise[i]);
		}
		return sum / noise.size();

		*this is probably beyond the scope of this lib
		*end user could construct multiple objects of this class and it would be simpler than trying to implement it here
		*probably simpler for the end user too, regardless of what kinda trickery is implemented
	}
*/

};
using Noise_Processor3f = Noise_Processor3<float>;
using Noise_Processor3d = Noise_Processor3<double>;
} //namespace noiz