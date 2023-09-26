#include <noiz/noise2.hpp>

#include <argument_parsing.hpp>


#include <charconv>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>
#include <fstream>

constexpr int32_t constant_base_resolution = 256;
constexpr int32_t constant_max_resolution_factor = 16;

void write_ppm_file_header(std::ofstream& out_file, uint16_t image_size){
	out_file << "P6" << std::endl;
	out_file << image_size << " " << image_size << std::endl;
	out_file << 255 << std::endl; //255 is max color value, 8 bits
}

struct Config {
	noiz::Seed seed{noiz::detail::Generator::make_random_seed()};
	noiz::GridExtent2 grid_extent{50, 1}; // NOLINT
	float step{0.1f};					  // NOLINT
	int image_size_factor{1};

	// syntax: [count] [step]
	auto parse_args(Args args) -> bool {
		if (!args.next_as(step, "step")) { return false; }
		if (!args.next_as(image_size_factor, "image_size_factor")) { return false; }
		if (image_size_factor < 0 || image_size_factor > constant_max_resolution_factor) {
			std::cerr << std::format("invalid image size factor: '{}'\n", image_size_factor);
			return false;
		}
		if (!args.args.empty()) {
			return false;
		}
		grid_extent.x = constant_base_resolution * image_size_factor;
		grid_extent.y = constant_base_resolution * image_size_factor;
		return true;
	}
};

// pulls noise from noiz::lib, writes a pixel to ppm file
class Texture_2D {
	public:
		explicit Texture_2D(int image_size_factor) : image_size{static_cast<uint16_t>(constant_base_resolution * image_size_factor)} {}

		void build_and_write_image_noise(noiz::Noise2f& noise, float const& step) const {

			std::ofstream out_file{"noise.ppm", std::ios::binary};
			write_ppm_file_header(out_file, image_size);
			uint8_t grayscale_color; //0 is black, 255 is white
			for (int y = 0; y < image_size; y++) {
				for(int x = 0; x < image_size; x++) {
				// add noise at point
					float adjusted_noise = noise.at(noiz::Vec2f{.x = static_cast<float>(x) * step, .y = static_cast<float>(y) * step});
					adjusted_noise = (adjusted_noise + 1.f) * 0.5f;
					grayscale_color = static_cast<uint8_t>((adjusted_noise) * 255.f);
					for(int i = 0; i < 3; i++) {
						//ppm cant use 1 channel grayscale, forced to 3 channel
						out_file.write(reinterpret_cast<char*>(&grayscale_color), sizeof(char)); //sizeof(char) == 1
					}
				}
			}
			out_file.close();
		}

	private:
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

	// build and write noise to image
	auto texture2d = Texture_2D{config.image_size_factor};
	texture2d.build_and_write_image_noise(noise, config.step);
}
