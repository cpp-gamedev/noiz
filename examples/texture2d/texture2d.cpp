#include <noiz/noise2.hpp>

#include "../argument_parsing.hpp"


#include <charconv>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>
#include <fstream>

constexpr int32_t BASE_RESOLUTION = 256;
constexpr int32_t MAX_RESOLUTION_FACTOR = 16;

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
	class Texture_2D {
		public:
			explicit Texture_2D(int image_size_factor) : image_size{static_cast<uint16_t>(BASE_RESOLUTION * image_size_factor)} {}

			void build_and_write_image_noise(noiz::Noise2f& noise, float const& step) {

				std::ofstream out_file{"noise.ppm", std::ios::binary};
				//Bmp_Header::write_bmp_file_header(image_size, out_file);
				write_ppm_file_header(out_file, image_size);
				uint8_t grayscale_color; //0 is black, 255 is white
				for (int y = 0; y < image_size; y++) {
					for(int x = 0; x < image_size; x++) {
					// add noise at point
						float adjusted_noise = noise.at(noiz::Vec2f{.x = static_cast<float>(x) * step, .y = static_cast<float>(y) * step});
						adjusted_noise = (adjusted_noise + 1.f) * 0.5f;
						grayscale_color = static_cast<uint8_t>((adjusted_noise) * 255.f);
						/*
						* might be better do this
						char adjusted_noise = static_cast<char>(noise.at(noiz::Vec2f{.x = static_cast<float>(x) * step, .y = static_cast<float>(y) * step}) * 127.f);
						out_file.write(&adjusted_noise, 1);
						*/
						for(int i = 0; i < 3; i++) { 
							//bmp cant do 1 channel, needs all 3 channels. unfortunately that means writing the color 3 times.
							//id like to use png, but I think including a lib for an example is kinda rough.
							out_file.write((char*)&grayscale_color, sizeof(char)); //sizeof(char) == 1
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

	// build histogram
	auto texture2d = Texture_2D{config.image_size_factor};
	texture2d.build_and_write_image_noise(noise, config.step);
}
