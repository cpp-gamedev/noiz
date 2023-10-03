#include <noiz/noise3.hpp>
#include <noiz/noise-processing3.hpp>

#include <argument_parsing.hpp>


#include <charconv>
#include <filesystem>
#include <format>
#include <iostream>
#include <vector>
#include <fstream>

constexpr int32_t constant_base_resolution = 128;
constexpr int32_t constant_max_resolution_factor = 32;

struct Config {
	noiz::Seed seed{noiz::detail::Generator::make_random_seed()};
	noiz::GridExtent3 grid_extent{constant_base_resolution, constant_base_resolution, constant_base_resolution}; // NOLINT
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
		grid_extent.x *= image_size_factor;
		grid_extent.y *= image_size_factor;
		grid_extent.z *= image_size_factor;
		return true;
	}
};

// pulls noise from noiz::lib, writes a pixel to ppm file
class Point_Cloud {
	public:
		explicit Point_Cloud(int image_size_factor) : image_size{static_cast<uint16_t>(constant_base_resolution * image_size_factor)} {}

		void build_and_write_object_with_noise(noiz::Noise3f& noise, float const& step) const {

			std::ofstream out_file{"hybrid_multifractal_noise.obj", std::ios::binary};
			
            float vertex_z;
            float vertex_y;

            noiz::Noise_Processor3<float> noise_processor{noise};

            for(int z = 0; z < image_size; z++){
                vertex_z = (static_cast<float>(z) / static_cast<float>(image_size)) - 0.5f;

                for (int y = 0; y < image_size; y++) {
                vertex_y = (static_cast<float>(y) / static_cast<float>(image_size)) - 0.5f;
                    for(int x = 0; x < image_size; x++) {
                    // add noise at point
                        //raw noise
#if 0 //raw noise
                        const float noise_value = noise.at(noiz::Vec3f{.x = static_cast<float>(x) * step, .y = static_cast<float>(y) * step, .z = static_cast<float>(z) * step});
                        if(noise_value > 0.0f){ //this should render a half of the points with raw noise

                            //no point in assigning x here? just write it directly to
                            out_file << "v " << ((static_cast<float>(x) / static_cast<float>(image_size)) - 0.5f) << " " << vertex_y << " " << vertex_z << '\n';
                        } 
#else //hybrid multi fractal noise
                        const float noise_value = noise_processor.hybrid_multi_fractal_processing(
                                noiz::Vec3f{.x = static_cast<float>(x) * step, .y = static_cast<float>(y) * step, .z = static_cast<float>(z) * step}
                            );
                        if(noise_value > 2.0f){ //this should render a half of the points with hmf noise

                            //no point in assigning x here? just write it directly to
                            out_file << "v " << ((static_cast<float>(x) / static_cast<float>(image_size)) - 0.5f) << " " << vertex_y << " " << vertex_z << '\n';
                        } 
#endif
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
		std::cout << "\t output image resolution is 128x128, with each dimension multiplied by image_size_factor, maximum scaling factor is 32[image size of 4096]" << std::endl;
		return EXIT_SUCCESS;
	}

	// parse args, if any
	if (!config.parse_args(args)) { 
		std::cout << args.args.front() << std::endl;
		return EXIT_FAILURE; 
	}


	auto noise = noiz::Noise3f{noiz::Seed{config.seed}, config.grid_extent};

	// build and write noise to image
	auto point_cloud = Point_Cloud{config.image_size_factor};
	point_cloud.build_and_write_object_with_noise(noise, config.step);
}
