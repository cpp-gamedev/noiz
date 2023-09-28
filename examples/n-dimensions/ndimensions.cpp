#include <noiz/noise2.hpp>
#include <noiz/noise3.hpp>
#include <noizN/noise.hpp>


#include <charconv>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>
#include <fstream>

#include <chrono>

constexpr int32_t constant_base_resolution = 256;
constexpr int32_t constant_max_resolution_factor = 16;

// pulls noise from noiz::lib, writes a pixel to ppm file


auto main(int argc, char** argv) -> int {

	const uint16_t two_dimensional_breadth = 512;   //NOLINT
	const uint16_t three_dimensional_breadth = 256; //NOLINT
	//768 and 512 were chosen because 768^2 == 512^3
	noiz::GridExtent2 grid_extent2 = {two_dimensional_breadth, two_dimensional_breadth}; 		
	noiz::GridExtent3 grid_extent3 = {three_dimensional_breadth, three_dimensional_breadth, three_dimensional_breadth}; 	
	noiz::GridExtent grid_extentN2 = {.components = {two_dimensional_breadth, two_dimensional_breadth}};
	noiz::GridExtent grid_extentN3 = {.components = {three_dimensional_breadth, three_dimensional_breadth, three_dimensional_breadth}};


	noiz::Seed seed{noiz::detail::Generator::make_random_seed()};

	auto noise3 = noiz::Noise3f{noiz::Seed{seed}, grid_extent3};

	//SeeN is kinda redundant, just too lazy to fix the includes
	//different seeds may affect benchmarking time???
	noiz::SeedN seedN{noiz::detail::GeneratorN::make_random_seed()}; 


	double two_dimensional_time_multiplier = (two_dimensional_breadth * two_dimensional_breadth);
	double three_dimensional_time_multiplier = (three_dimensional_breadth * three_dimensional_breadth);

	std::cout << "~ starting test ~ " << std::endl;
	{
		auto start_time = std::chrono::high_resolution_clock::now();

		auto noise2 = noiz::Noise2f{noiz::Seed{seed}, grid_extent2};
		noiz::Vec2f point;
		for(int x = 0; x < two_dimensional_breadth; x++){
			for(int y = 0; y < two_dimensional_breadth; y++){
				point.x = static_cast<float>(x);
				point.y = static_cast<float>(y);
				noise2.at(point); //NOLINT
			}
		}
		
		auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
		std::cout << "milliseconds for noise2 to process " << two_dimensional_time_multiplier << " points : " << duration_ms << std::endl;
	}

	{
		auto start_time = std::chrono::high_resolution_clock::now();

		auto noise2N = noiz::Noisef{noiz::SeedN{seedN}, grid_extentN2};
		noiz::Vecf point;
		point.components.resize(2);
		for(int x = 0; x < two_dimensional_breadth; x++){
			for(int y = 0; y < two_dimensional_breadth; y++){
				point.components[0] = static_cast<float>(x);
				point.components[1] = static_cast<float>(y);
				noise2N.at(point); //NOLINT
			}
		}

		auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
		std::cout << "milliseconds for noise2N to process " << two_dimensional_time_multiplier << " points : " << duration_ms << std::endl;
	}

	{
		auto start_time = std::chrono::high_resolution_clock::now();

		auto noise3 = noiz::Noise3f{noiz::Seed{seed}, grid_extent3};
		noiz::Vec3f point;
		for(int x = 0; x < three_dimensional_breadth; x++){
			for(int y = 0; y < three_dimensional_breadth; y++){
				for(int z = 0; z < three_dimensional_breadth; z++){
					point.x = static_cast<float>(x);
					point.y = static_cast<float>(y);
					point.z = static_cast<float>(z);
					noise3.at(point); //NOLINT
				}
			}
		}
		
		auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
		std::cout << "milliseconds for noise3 to process " << three_dimensional_time_multiplier << " points : " << duration_ms << std::endl;
	}

	{
		auto start_time = std::chrono::high_resolution_clock::now();

		auto noise3N = noiz::Noisef{noiz::SeedN{seedN}, grid_extentN3};
		noiz::Vecf point;
		point.components.resize(3);
		for(int x = 0; x < three_dimensional_breadth; x++){
			for(int y = 0; y < three_dimensional_breadth; y++){
				for(int z = 0; z < three_dimensional_breadth; z++){
					point.components[0] = static_cast<float>(x);
					point.components[1] = static_cast<float>(y);
					point.components[2] = static_cast<float>(z);
					noise3N.at(point); //NOLINT
				}
			}
		}
		
		auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
		std::cout << "milliseconds for noise3N to process " << three_dimensional_time_multiplier << " points : " << duration_ms << std::endl;
	}

	system("pause"); //NOLINT
}
