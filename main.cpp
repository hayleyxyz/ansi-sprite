#include <iostream>
#include <map>
#include <Magick++.h>
#include "ansi_colours.h"

/**
 * Returns the distance between two 3D points
 * @param x1
 * @param y1
 * @param z1
 * @param x2
 * @param y2
 * @param z2
 * @return
 */
static inline double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}

/**
 * Print space with specified background colour
 * @param bg
 */
static inline void print_space(int bg) {
    printf("\e[30;48;5;%dm  \e[49m", bg);
}

int main(int argc, char **argv) {
    Magick::InitializeMagick(*argv);

    if(argc < 2) {
        std::cout << "usage: " << argv[0] << " image" << std::endl;
        return 1;
    }

    Magick::Image image;

    try {
        image.read(argv[1]);
    }
    catch(Magick::Exception &exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }

    // Map to store colours already resolved to an ANSI colour id
    std::map<Magick::Color, int> cached;

    auto size = image.size();
    for(auto y = 0; y < size.height(); y++) {
        for(auto x = 0; x < size.width(); x++) {
            auto colour = image.pixelColor(x, y);
            // Construct ColorRGB object so we can easily access r/g/b values
            Magick::ColorRGB colourRGB(colour);

            double closestDistance = -1;
            int closestColourId = 0;

            if(cached.find(colour) != cached.end()) {
                closestColourId = cached[colour];
            }
            else {
                // Loop through all ANSI colours to find closest match
                for (auto i = 0; i < 255; i++) {
                    auto colourId = ansi_colours[i].colourId;

                    auto colourDistance = distance(
                            ansi_colours[i].r,
                            ansi_colours[i].g,
                            ansi_colours[i].b,
                            colourRGB.red(),
                            colourRGB.green(),
                            colourRGB.blue()
                    );


                    if (closestDistance == -1 || closestDistance > colourDistance) {
                        closestDistance = colourDistance;
                        closestColourId = colourId;
                    }
                }

                cached[colour] = closestColourId;
            }

            if(colourRGB.alpha() == 0) {
                printf("  ");
            }
            else {
                print_space(closestColourId);
            }
        }

        printf("\n");
    }

    return 0;
}