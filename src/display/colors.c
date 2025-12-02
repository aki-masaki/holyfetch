#include "display/colors.h"

color_map colors[] = {
    {"red", RED},         {"green", GREEN}, {"yellow", YELLOW}, {"blue", BLUE},
    {"magenta", MAGENTA}, {"cyan", CYAN},   {"nocolor", RESET},
};

size_t colors_count = sizeof(colors) / sizeof(colors[0]);
