#pragma once
#include "detail/data2.hpp"

#include <cmath>
#include <vector>
#include <numeric>

//https://cs.nyu.edu/~perlin/noise/
//https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
//p is perlin's array, idk if it matters if i stick to it or randomly generate those numbers?

static constexpr uint16_t DEFAULT_SAMPLE_COUNT = 256;

namespace noiz {
    template <std::floating_point Type>
    class PerlinNoise2 {
    public:
       

        //this is for random seed
        explicit PerlinNoise2() : PerlinNoise2(detail::Generator::make_random_seed()) {}

        //this is for using a given seed, re-generating a texture
        explicit PerlinNoise2(Seed generator_seed) {
            pNumbers.resize(DEFAULT_SAMPLE_COUNT);

            //fill perms with values from 0 to 255
            //never used iota before, need to research this
            std::iota(pNumbers.begin(), pNumbers.end(), 0);

            //shuffle values, i dont quite understand what this means beyond the shuffle itself yet
            std::shuffle(pNumbers.begin(), pNumbers.end(), generator_seed);

            //duplicate permutations
            //is this a memcpy from p.begin() to p.end()??

            //removing this requires a modulus in every index, 
            //the upfront computation/memory cost is a little rough but over a decent runtime it'll imrpove performance
            pNumbers.insert(pNumbers.end(), pNumbers.begin(), pNumbers.end());

        }

        auto at(Vec2<Type> const& point) -> Type {

            //find unit cube that contains point
            const int X = (int)std::floor(point.x) & 255;
            const int Y = (int)std::floor(point.y) & 255;

            const Type x = point.x - std::floor(point.x);                  // FIND RELATIVE X,Y,Z
            const Type y = point.y - std::floor(point.y);                  // OF POINT IN CUBE

            const Type u = fade(x);                                     // COMPUTE FADE CURVES
            const Type v = fade(y);                                     // FOR EACH OF X,Y,Z.

            //int A = p[X] + Y;
            int AA = pNumbers[(pNumbers[X] + Y) % 256];
            int AB = pNumbers[(pNumbers[X] + Y + 1) % 256];      // HASH COORDINATES OF
            //int B = p[X+1]+Y;
            int BA = pNumbers[(pNumbers[X+1]+Y) % 256];
            int BB = pNumbers[(pNumbers[X+1]+Y + 1) % 256];      // THE 8 CUBE CORNERS,

            static constexpr Type unit = (Type)1;

            return 
                //add blended results from corners
                std::lerp(
                    std::lerp( 
                        grad(pNumbers[AA], x, y),
                        grad(pNumbers[BA], x - unit, y),
                        u
                    ),                                          
                    std::lerp(
                        grad(pNumbers[AB], x, y - unit),        
                        grad(pNumbers[BB], x - unit, y - unit),
                        u
                    ),
                    v
                );
        }
        //                                                             //idk what the 6 15 or 10 are, i dont really udnerstand this at all
        static constexpr auto fade(Type t) -> Type { return t * t * t * (t * (t * 6 - 15) + 10); }
        static constexpr auto grad(int const& hash, Type const& x, Type const& y) -> Type {
            // CONVERT LO 4 BITS OF HASH CODE
            // INTO 12 GRADIENT DIRECTIONS.
            // perlin is too big for good comments????????
            const int h = hash & 15;                    
            //const Type u = h<8 ? x : y;                 
            //const Type v = h<4 ? y : ((h==12||h==14) * x);

            //NOT A FAN OF THE ? OPERATOR
            const Type u = ((h < 8) * x) + ((h >= 8) * y);

            //i have no idea what the 12 and 14 are, may need to be removed as I move from 3d to 2d
            const Type v = ((h < 4) * y) + ((h >= 4) * (h == 12 || h == 14) * x);

            return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
        }

        //i have no idea what the significance of these numbers is, ill mess around with it later
        /*
        std::vector<uint8_t> perlins_numbers ={ 151,160,137,91,90,15,
            131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
            190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
            88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
            77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
            102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
            135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
            5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
            223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
            129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
            251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
            49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };
        */
    protected:
        std::vector<int> pNumbers; //permutations
    };

    using PerlinNoise2f = PerlinNoise2<float>;
    using PerlinNoise2d = PerlinNoise2<double>;
}//namespace noiz