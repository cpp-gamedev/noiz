#include <noiz/noise2.hpp>

#include "../argument_parsing.hpp"


#include <charconv>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>
#include <fstream>

//https://adrianb.io/2014/08/09/perlinnoise.html

constexpr int32_t BASE_RESOLUTION = 256;
constexpr int32_t MAX_RESOLUTION_FACTOR = 16;

	void write_ppm_file_header(std::ofstream& out_file, uint16_t image_size){
		out_file << "P6" << std::endl;
		out_file << image_size << " " << image_size << std::endl;
		out_file << 255 << std::endl; //255 is max color value, 8 bits
	}

	struct Config {
		noiz::Seed seed{noiz::detail::Generator::make_random_seed()};
		noiz::GridExtent2 grid_extent{256, 256}; // NOLINT
		float step{0.1f};					  // NOLINT
		int image_size_factor{1};

		// syntax: [count] [step]
		auto parse_args(Args args) -> bool {
			if (!args.next_as(step, "step")) { return false; }
			if (!args.next_as(image_size_factor, "image_size_factor")) { return false; }
			if (image_size_factor < 0 || image_size_factor > MAX_RESOLUTION_FACTOR) {
				std::cerr << std::format("invalid image size factor: '{}'\n", image_size_factor);
				return false;
			}
			if (!args.args.empty()) {
				return false;
			}
			grid_extent.x = BASE_RESOLUTION * image_size_factor;
			grid_extent.y = BASE_RESOLUTION * image_size_factor;
			return true;
		}
	};

// contains logic to store data points and draw them
	class Octave_Texture_2D {
		public:
			explicit Octave_Texture_2D(int image_size_factor) : image_size{static_cast<uint16_t>(BASE_RESOLUTION * image_size_factor)} {}

			void build_and_write_image_noise(noiz::Noise2f& noise, float const& step) {

				std::ofstream out_file{"noise.ppm", std::ios::binary};
				//Bmp_Header::write_bmp_file_header(image_size, out_file);
				write_ppm_file_header(out_file, image_size);
				//uint8_t grayscale_color; //0 is black, 255 is white
				Color_RGB water = Color_RGB{.red = 0, .green = 0, .blue = 127};
				Color_RGB beach = Color_RGB{.red = 200, .green = 200, .blue = 200};
				Color_RGB grass = Color_RGB{.red = 0, .green = 127, .blue = 0};

				for (int y = 0; y < image_size; y++) {
					for(int x = 0; x < image_size; x++) {
					// add noise at point
						//float adjusted_noise = noise.at(noiz::Vec2f{.x = static_cast<float>(x) * step, .y = static_cast<float>(y) * step});
						const float adjusted_noise = noise.at_with_octave(noiz::Vec2f{.x = static_cast<float>(x) * step, .y = static_cast<float>(y) * step}, 4, 0.25f, 2.f);
						//adjusted_noise = (adjusted_noise + 1.f) * 0.5f;
						//grayscale_color = static_cast<uint8_t>((adjusted_noise) * 255.f);
						if(adjusted_noise > 0.05f){
							grass.write_to_file(out_file);
						}
						else if(adjusted_noise > -0.05f){
							beach.write_to_file(out_file);
						}
						else{
							water.write_to_file(out_file);
						}
					}
				}
				out_file.close();
			}

		private:
			struct Color_RGB{
				uint8_t red;
				uint8_t green;
				uint8_t blue;
				void write_to_file(std::ofstream& out_file){
					out_file.write((char*)&red, sizeof(uint8_t));
					out_file.write((char*)&green, sizeof(uint8_t));
					out_file.write((char*)&blue, sizeof(uint8_t));
				}
			};
			uint16_t image_size;
	};

auto main(int argc, char** argv) -> int {
	auto config = Config{};

	// skip exe name (argv[0])
	auto const args = Args{std::span{argv, static_cast<std::size_t>(argc)}.subspan(1)};

	// handle --help
	if (!args.args.empty() && args.args.front() == std::string_view{"--help"}) {
		std::cout << std::format("Usage: {} [step(=0.1)] [image_size_factor(=1)]\n", std::filesystem::path{*argv}.stem().string());
		std::cout << "\t output image resolution is 256x256, with each dimension multiplied by image_size_factor, maximum scaling factor is 16[image size of 4096]" << std::endl;
		return EXIT_SUCCESS;
	}

	// parse args, if any
	if (!config.parse_args(args)) { 
		std::cout << args.args.front() << std::endl;
		return EXIT_FAILURE; 
	}

	auto noise = noiz::Noise2f{noiz::Seed{config.seed}, config.grid_extent};

	// build histogram
	auto texture2d = Octave_Texture_2D{config.image_size_factor};
	texture2d.build_and_write_image_noise(noise, config.step);
}
