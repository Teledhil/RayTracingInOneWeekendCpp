#ifndef COLOR_H_
#define COLOR_H_

#include <iostream>

#include "vec3.h"

void write_color(std::ostream &stream, const color &pixel,
                 int samples_per_pixel);

static const color white(1.0, 1.0, 1.0);
static const color black(0.0, 0.0, 0.0);
static const color blue(0.5, 0.7, 1.0);
static const color red(1.0, 0, 0);

#endif // COLOR_H_
