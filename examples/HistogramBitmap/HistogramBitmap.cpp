#include <noiz/noise2.hpp>


#include <charconv>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>
#include <fstream>

	struct BmpHeader {
		char bitmapSignatureBytes[2] = {'B', 'M'};
		uint32_t sizeOfBitmapFile = 54;
		uint32_t reservedBytes = 0;
		uint32_t pixelDataOffset = 54;

		BmpHeader(uint16_t imageSize) {
			sizeOfBitmapFile += imageSize * imageSize * 3; //rgb, 1 byte per color
		}
		void writeToFile(std::ofstream& outFile){
			//writing like this to avoid padding issues
			outFile.write(bitmapSignatureBytes, 2);
			outFile.write((char*) &sizeOfBitmapFile, sizeof(uint32_t));
			outFile.write((char*) &reservedBytes, sizeof(uint32_t));
			outFile.write((char*) &pixelDataOffset, sizeof(uint32_t));
		}
	};


	struct BmpInfoHeader {
		uint32_t sizeOfThisHeader = 40;
		int32_t width = 256; // in pixels
		int32_t height = 256;
		uint16_t numberOfColorPlanes = 1; // must be 1
		uint16_t colorDepth = 24;
		uint32_t compressionMethod = 0;
		uint32_t rawBitmapDataSize = 0;
		int32_t horizontalResolution = 3780; // in pixel per meter //do i even need this?
		int32_t verticalResolution = 3780; // in pixel per meter
		uint32_t colorTableEntries = 0;
		uint32_t importantColors = 0;

		BmpInfoHeader(uint16_t imageSize){
			width = imageSize;
			height = imageSize;
		}
		void writeToFile(std::ofstream& outFile){
			//writing like this to avoid padding issues
			outFile.write((char*) &sizeOfThisHeader, sizeof(uint32_t));
			outFile.write((char*) &width, sizeof(int32_t));
			outFile.write((char*) &height, sizeof(int32_t));
			outFile.write((char*) &numberOfColorPlanes, sizeof(uint16_t));
			outFile.write((char*) &colorDepth, sizeof(uint16_t));
			outFile.write((char*) &compressionMethod, sizeof(uint32_t));
			outFile.write((char*) &rawBitmapDataSize, sizeof(uint32_t));
			outFile.write((char*) &horizontalResolution, sizeof(int32_t));
			outFile.write((char*) &verticalResolution, sizeof(int32_t));
			outFile.write((char*) &colorTableEntries, sizeof(uint32_t));
			outFile.write((char*) &importantColors, sizeof(uint32_t));
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
	int imageSize{1};

	// syntax: [count] [step]
	auto parse_args(Args args) -> bool {
		if (!args.next_as(count, "count")) { return false; }
		if (!args.next_as(step, "step")) { return false; }
		if (!args.next_as(imageSize, "imageSize")) { return false; }
		assert(imageSize > 0 && imageSize < 16);
		if (!args.args.empty()) {
			std::cerr << std::format("unrecognized argument: '{}'\n", args.next());
			return false;
		}
		return true;
	}
};

void writeBitmapFileHeader(uint16_t imageSize, std::ofstream& outFile) {

	assert(outFile.is_open());
	BmpHeader bmpHeader{imageSize};
	bmpHeader.writeToFile(outFile);
	BmpInfoHeader bmpInfoHeader{imageSize};
	bmpInfoHeader.writeToFile(outFile);
}

// contains logic to store data points and draw them
class HistogramBitmap {
  public:
	struct Color{
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;

		void writeToFile(std::ofstream& outFile) {
			//writing like this to avoid padding issues
			outFile.write((char*) &red, sizeof(uint8_t));
			outFile.write((char*) &green, sizeof(uint8_t));
			outFile.write((char*) &blue, sizeof(uint8_t));
		}

	};

	explicit HistogramBitmap(int imageSizeFactor, int dataCount) {
		imageSize *= imageSizeFactor;
		heightValues.reserve(dataCount);
	}


	auto add(float const value) -> HistogramBitmap& {
		heightValues.push_back((value + 1.0f) * 0.5f * static_cast<float>(imageSize));

		return *this;
	}

	void writeImage() {
		std::vector<float> interpolatedHeight;
		interpolatedHeight.resize(imageSize);
		float stepSizeOfBaseGraph = static_cast<float>(imageSize) / static_cast<float>(heightValues.size() - 1);

		std::ofstream outFile{"histogram.bmp", std::ios::binary};

#if false
		//leaving this for future cubic interpolation
		//i need to learn more about it

		for(int i = 1; i < imageSize - 1; i++) {
			interpolatedHeight[i] = cubicInterpolation();
		}	
	
#else
		interpolatedHeight[0] = heightValues[0];
		interpolatedHeight.back() = heightValues.back();

		for(int i = 1; i < imageSize - 1; i++) {
			float exactPoint = static_cast<float>(i) / stepSizeOfBaseGraph;
			uint16_t lastPoint = std::floor(exactPoint);
			uint16_t nextPoint = std::ceil(exactPoint);
			float weighting = exactPoint - (float)lastPoint;
			interpolatedHeight[i] = std::lerp(heightValues[lastPoint], heightValues[nextPoint], weighting);
		}

		writeBitmapFileHeader(imageSize, outFile);
		Color outColor;
		for(int y = 0; y < imageSize; y++) {
			for(int x = 0; x < imageSize; x++) {
				if(interpolatedHeight[x] >= (float)y) {
					histogramColor.writeToFile(outFile);
				}
				else{
					backgroundColor.writeToFile(outFile);
				}
			}
		}

#endif
		outFile.close();
	}


  private:
	uint16_t imageSize = 256;
	Color backgroundColor{.red = 0, .green = 0, .blue = 0};
	Color histogramColor{.red = 0, .green = 127, .blue = 0};
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
		std::cout << std::format("Usage: {} [count(=100)] [step(=0.1)] [imageSize(=1)]\n", std::filesystem::path{*argv}.stem().string());
		std::cout << "\t output image resolution is 256x256, with each dimension multiplied by imageSize, maximum imageSize is 16[4096]" << std::endl;
		return EXIT_SUCCESS;
	}

	// parse args, if any
	if (!config.parse_args(args)) { 
		std::cout << args.args.front() << std::endl;
		return EXIT_FAILURE; 
	}

	// build histogram
	auto histogramBitmap = HistogramBitmap{config.imageSize, config.count};
	for (int i = 0; i < config.count; ++i) {
		// build point on line (y = 0)
		auto const point = noiz::Vec2f{.x = static_cast<float>(i) * config.step};
		// add noise at point
		histogramBitmap.add(noise.at(point));
	}

	//write histogram to bmp file in same folder as execution
	histogramBitmap.writeImage();
}
