#include <noiz/noise2.hpp>


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
			size_of_bmp_file += static_cast<uint32_t>(width * height) * 3; //rgb, 1 byte per color
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
			bmpHeader.write_to_file(out_file);
		}
	};


namespace {
template <typename Type>
concept NumberT = std::integral<Type> || std::floating_point<Type>;

template <NumberT Type>
auto parse_as(Type& out, std::string_view const value) -> bool {
	if (value.empty()) { return false; }

	auto const* end = value.data() + value.size();
	auto const [ptr, ec] = std::from_chars(value.data(), end, out);

	return ec == std::errc{} && ptr == end;
}

struct Args {
	std::span<char const* const> args{};

	[[nodiscard]] constexpr auto next() -> std::string_view {
		if (args.empty()) { return {}; }
		auto const* ret = args.front();
		args = args.subspan(1);
		return ret;
	}

	template <NumberT Type>
	auto next_as(Type& out, std::string_view const key) -> bool {
		auto value = next();
		if (value.empty()) { return true; }

		if (!parse_as(out, value)) {
			std::cerr << std::format("invalid argument: '{}' for '{}'\n", value, key);
			return false;
		}

		return true;
	}
};

struct Config {
	noiz::Seed seed{noiz::detail::Generator::make_random_seed()};
	noiz::GridExtent2 grid_extent{50, 1}; // NOLINT
	int count{100};						  // NOLINT
	float step{0.1f};					  // NOLINT
	int image_size{1};

	// syntax: [count] [step]
	auto parse_args(Args args) -> bool {
		if (!args.next_as(count, "count")) { return false; }
		if (!args.next_as(step, "step")) { return false; }
		if (!args.next_as(image_size, "image_size")) { return false; }
		if (image_size < 0 || image_size > MAX_RESOLUTION_FACTOR) {
		    std::cerr << std::format("invalid image size: '{}'\n", image_size);
		    return false;
		}
		if (!args.args.empty()) {
			return false;
		}
		return true;
	}
};

// contains logic to store data points and draw them
class Histogram_Bitmap {
  public:
	struct Color_RGB{
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;

		void write_to_file(std::ofstream& out_file) const {
			//writing like this to avoid padding issues
			out_file.write((char*) &red, sizeof(uint8_t));
			out_file.write((char*) &green, sizeof(uint8_t));
			out_file.write((char*) &blue, sizeof(uint8_t));
		}

	};

	explicit Histogram_Bitmap(int image_sizeFactor, int dataCount) {
		image_size *= static_cast<uint8_t>(image_sizeFactor);
		heightValues.reserve(static_cast<size_t>(dataCount));
		
	}


	auto add(float const value) -> Histogram_Bitmap& {
		heightValues.push_back((value + 1.0f) * 0.5f * static_cast<float>(image_size));

		return *this;
	}

	void write_image() const {
		std::vector<float> height_in_pixels;
		height_in_pixels.resize(image_size);
		const float resolution_conversion_real_to_pixel = static_cast<float>(image_size) / static_cast<float>(heightValues.size() - 1);

		std::ofstream out_file{"histogram.bmp", std::ios::binary};

		height_in_pixels[0] = heightValues[0];
		height_in_pixels.back() = heightValues.back();

		for(uint16_t i = 1; i < image_size - 1; i++) {

			const float exactPoint = static_cast<float>(i) / resolution_conversion_real_to_pixel;
			const float flooredValue = std::floor(exactPoint);

			height_in_pixels.at(i) = heightValues.at(static_cast<uint16_t>(flooredValue) + ((exactPoint - flooredValue) >= 0.5f));
		}

		Bmp_Header::write_bmp_file_header(image_size, out_file);

		for(uint16_t y = 0; y < image_size; y++) {
			for(uint16_t x = 0; x < image_size; x++) {
				if(height_in_pixels.at(x) >= (float)y) {
					histogramColor.write_to_file(out_file);
				}
				else{
					backgroundColor.write_to_file(out_file);
				}
			}
		}
		out_file.close();
	}


  private:
	uint16_t image_size = BASE_RESOLUTION;
	Color_RGB backgroundColor{.red = 0, .green = 0, .blue = 0}; //black
	Color_RGB histogramColor{.red = 0, .green = 127, .blue = 0}; //darkish green
	std::vector<float> heightValues;
};
} // namespace

auto main(int argc, char** argv) -> int {
	auto config = Config{};
	auto noise = noiz::Noise2f{noiz::Seed{config.seed}, config.grid_extent};

	// skip exe name (argv[0])
	auto const args = Args{std::span{argv, static_cast<std::size_t>(argc)}.subspan(1)};

	// handle --help
	if (!args.args.empty() && args.args.front() == std::string_view{"--help"}) {
		std::cout << std::format("Usage: {} [count(=100)] [step(=0.1)] [image_size(=1)]\n", std::filesystem::path{*argv}.stem().string());
		std::cout << "\t output image resolution is 256x256, with each dimension multiplied by image_size, maximum scaling factor is 16[image size of 4096]" << std::endl;
		return EXIT_SUCCESS;
	}

	// parse args, if any
	if (!config.parse_args(args)) { 
		std::cout << args.args.front() << std::endl;
		return EXIT_FAILURE; 
	}

	// build histogram
	auto histogram_bitmap = Histogram_Bitmap{config.image_size, config.count};
	for (int i = 0; i < config.count; ++i) {
		// build point on line (y = 0)
		auto const point = noiz::Vec2f{.x = static_cast<float>(i) * config.step};
		// add noise at point
		histogram_bitmap.add(noise.at(point));
	}

	//write histogram to bmp file in same folder as execution
	histogram_bitmap.write_image();
}
