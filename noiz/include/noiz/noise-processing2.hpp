#pragma once
#include "noise2.hpp"

namespace noiz{
//static functions, maybe make members of noise2? not really sure
template <std::floating_point Type>
class Noise_Processor2 {
public:

	//if this is multi-threaded, accessing these variables directly could lead to strange behavior
	//i.e. a handful of threads processing noise, then another comes in and changes octave??
	//not sure if thats a use case worth covering?

	//default values
	uint8_t octave{ 6 };
	Type step = (Type)0.1;
	Type persistance = (Type)1.5;
	Type lacunarity = (Type)2.0;
	Type frequency = (Type)1.0;
	Type amplitude = (Type)1.0;

	//hybrid multifractal variables, i have no idea what H is or id expand it to be mroe descriptive
	Type hbf_H = (Type)0.25;
	Type hbf_offset = (Type)0.7;
	std::vector<Type> hbf_exponent_array;

	//could construct an object of noiz::noise2 here and it would simplify these functions a little bit.

	auto raw_noise(noiz::Vec2<Type> const& point, Noise2<Type> const& noise) -> Type{
		//redundant
		return noise->at(point * step);
	}

	auto basic_processing(noiz::Vec2<Type> const& point, Noise2<Type> const& noise) -> Type {
		
		Type total = Type(0);
		Type frequency = (Type)2;
		Type amplitude = (Type)1;
		Type normalizer = (Type)0;
		for(int i = 0; i < octaves; i++){
			total += at(point * frequency]) * amplitude;
			normalizer += amplitude;
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		return total/normalizer;
	}

	auto turbulence_processing(noiz::Vec2<Type> const& point, Noise2<Type> const& noise) -> Type {
		Type amplitude = this->amplitude;
		Type frequency = this->frequency;
		
			Type sum = 0.f;
			Type normalizer = 0.f;
			for (int i = 0; i < octave; i++) {
				sum += noise.at(noiz::Vec2<Type>{.x = point.x * frequency, .y = point.y * frequency}) * amplitude;
				normalizer += amplitude;
				amplitude *= persistance;
				frequency *= lacunarity;
			}
			//normally return sum;
			//but im doing some additional adjustment in this function
			return sum;
	}

	auto billowy_processing(noiz::Vec2<Type> const& point, Noise2<Type> const& noise) -> Type {
		return abs(noise.at(point));
	}

	auto rigid_processing(noiz::Vec2<Type> const& point, Noise2<Type> const& noise) -> Type {
		return 1.f - std::abs(noise.at(point);
	}

	auto hybrid_multi_fractal_processing(noiz::Vec2<Type> const& point, Noise2<Type> const& noise) -> Type {
		//https://www.classes.cs.uchicago.edu/archive/2015/fall/23700-1/final-project/MusgraveTerrain00.pdf
		
		//this function assumes the octave could be a decimal value
		//i also casted it down from 3d to 2d, im not sure what effects this has. hard to say without 3d rendering
		
		//double HybridMultifractal( Vector point, double H, double lacunarity,
		//double octaves, double offset )
		noiz::Vec2<Type> tempPoint = point;

		Type frequency, result, signal, weight, remainder;
		//double Noise3(); what is this??
		/* precompute and store spectral weights */
		if (hbf_exponent_array.size() != octave) {
			hbf_exponent_array.resize(octave);
			/* seize required memory for exponent_array */
			//exponent_array = (double*)malloc(octave * sizeof(double));
			frequency = 1.0;
			for (int i = 0; i < octave; i++) {
				/* compute weight for each frequency */
				hbf_exponent_array[i] = pow(frequency, -hbf_H);
				frequency *= lacunarity;
			}
		}
		/* get first octave of function */
		result = (noise.at(tempPoint) + hbf_offset) * hbf_exponent_array[0];
		weight = result;
		/* increase frequency */
		tempPoint = tempPoint * lacunarity;
		/* spectral construction inner loop, where the fractal is built */
		for (int i = 1; i < octave; i++) {
			/* prevent divergence */
			if (weight > 1.0) weight = 1.0;
			/* get next higher frequency */
			signal = (noise.at(tempPoint) + hbf_offset) * hbf_exponent_array[i];
			/* add it in, weighted by previous freq's local value */
			result += weight * signal;
			/* update the (monotonically decreasing) weighting value */
			/* (this is why H must specify a high fractal dimension) */
			weight *= signal;
			/* increase frequency */
			tempPoint = tempPoint * lacunarity;
		} /* for */
		/* take care of remainder in “octaves” */
		
		/* octave is currently an int, may swap for this
		remainder = octave - (int)octave;
		if (remainder)
			// “i” and spatial freq. are preset in loop above
			result += remainder * noise.at(point) * hbf_exponent_array[octave - 1];
		*/

	/* value checking
		static Type biggestValue = (Type)0;
		static Type smallestValue = (Type)1;
		if (result > biggestValue) {
			biggestValue = result;
			std::cout << "biggest result : " << result << std::endl;
		}
		else if (result < smallestValue) {
			smallestValue = result;
			std::cout << "smallest result : " << result << std::endl;
		}
	*/

		return result;
	}

/*
	auto blended_basic_processing(noiz::Vec2<Type> const& point, std::vector<Noise2<Type>> const& noise){
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
} //namespace noiz