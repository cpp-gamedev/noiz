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

	struct Bmp_Header {

		//universal header
		char bitmapSignatureBytes[2] = {'B', 'M'};
		uint32_t size_of_bmp_file = 54; //14 for the universal header, 40 for the specific implementation. this will be increased later once the size of the image is known 
		uint32_t reserved_bytes = 0;
		uint32_t pixel_data_offset = 54; //== size of bmp header

		//continued data
		//as far as I understand, there is multiple implementations of the following data, and this is one of the more common ones
		uint32_t size_of_header = 40; //does not include the universal portion size
		int32_t width = BASE_RESOLUTION; // in pixels
		int32_t height = BASE_RESOLUTION;
		uint16_t number_of_color_planes = 1; // must be 1
		uint16_t color_depth = 24; //1 byte (8 bits) multiplied by the number of colors
		uint32_t compression_method = 0; //not used
		uint32_t raw_bitmap_data_size = 0; //not used
		int32_t horizontal_resolution = 3780; // in pixel per meter 
		int32_t vertical_resolution = 3780; //i dont really know what this is relevant to, but it works...
		uint32_t color_table_entries = 0; //not used
		uint32_t important_colors = 0; //not used

		explicit Bmp_Header(uint16_t image_size) : width{image_size}, height{image_size} {
			size_of_bmp_file += static_cast<uint32_t>(width * height) & 3;//rgb
		}
		void write_to_file(std::ofstream& out_file) const {
			//writing like this to avoid padding issues
			out_file.write(&bitmapSignatureBytes[0], 2);
			out_file.write((char*) &size_of_bmp_file, sizeof(uint32_t));
			out_file.write((char*) &reserved_bytes, sizeof(uint32_t));
			out_file.write((char*) &pixel_data_offset, sizeof(uint32_t));

			out_file.write((char*) &size_of_header, sizeof(uint32_t));
			out_file.write((char*) &width, sizeof(int32_t));
			out_file.write((char*) &height, sizeof(int32_t));
			out_file.write((char*) &number_of_color_planes, sizeof(uint16_t));
			out_file.write((char*) &color_depth, sizeof(uint16_t));
			out_file.write((char*) &compression_method, sizeof(uint32_t));
			out_file.write((char*) &raw_bitmap_data_size, sizeof(uint32_t));
			out_file.write((char*) &horizontal_resolution, sizeof(int32_t));
			out_file.write((char*) &vertical_resolution, sizeof(int32_t));
			out_file.write((char*) &color_table_entries, sizeof(uint32_t));
			out_file.write((char*) &important_colors, sizeof(uint32_t));
		}

		static void write_bmp_file_header(uint16_t image_size, std::ofstream& out_file) {
			const Bmp_Header bmpHeader{image_size};
			assert(out_file.is_open());
			bmpHeader.write_to_file(out_file);
		}
};


struct Config {
	noiz::Seed seed{noiz::detail::Generator::make_random_seed()};
	noiz::GridExtent2 grid_extent{50, 1}; // NOLINT
	float step{0.1f};					  // NOLINT
	int image_size{1};

	// syntax: [count] [step]
	auto parse_args(Args args) -> bool {
		if (!args.next_as(step, "step")) { return false; }
		if (!args.next_as(image_size, "image_size")) { return false; }
		if (image_size < 0 || image_size > MAX_RESOLUTION_FACTOR) {
		    std::cerr << std::format("invalid image size: '{}'\n", image_size);
		    return false;
		}
		if (!args.args.empty()) {
			return false;
		}
		grid_extent.x = BASE_RESOLUTION * image_size;
		grid_extent.y = BASE_RESOLUTION * image_size;
		return true;
	}
};

// contains logic to store data points and draw them
class Texture_2D {
	public:
		explicit Texture_2D(int image_sizeFactor) : image_size{static_cast<uint16_t>(BASE_RESOLUTION * image_sizeFactor)} {}

		void build_and_write_image_noise(noiz::Noise2f& noise, float const& step) {

			std::ofstream out_file{"noise.bmp", std::ios::binary};
			Bmp_Header::write_bmp_file_header(image_size, out_file);

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
		std::cout << std::format("Usage: {} [step(=0.1)] [image_size(=1)]\n", std::filesystem::path{*argv}.stem().string());
		std::cout << "\t output image resolution is 256x256, with each dimension multiplied by image_size, maximum scaling factor is 16[image size of 4096]" << std::endl;
		return EXIT_SUCCESS;
	}

	// parse args, if any
	if (!config.parse_args(args)) { 
		std::cout << args.args.front() << std::endl;
		return EXIT_FAILURE; 
	}

	auto noise = noiz::Noise2f{noiz::Seed{config.seed}, config.grid_extent};

	// build histogram
	auto texture2d = Texture_2D{config.image_size};
	texture2d.build_and_write_image_noise(noise, config.step);
}
