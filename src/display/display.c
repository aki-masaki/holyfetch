#include "display/display.h"
#include "display/colors.h"

#include <stdio.h>

char *art[13] = {"                                         ____________________________________________",
             "   /\\    /\\                             /",
             "   \\ \\  / /                            /",
             "    \\ \\/ /~~.                         /",
             "     \\  //_/                         /",
             "     /  \\/                          /",
             "    / /\\ \\                         /",
             "   / /| \\/                       _/",
             "  / / --/                       /",
             " / / /  |                   ___/",
             "/ / /   |                 _/",
             "\\/  \\   \\_      _________/",
             "---------------/"};

void display(char lines[][1024], config config) {
  printf("%s%s\n" RESET, config.art_color, art[0]);

  for (int i = 0; i < 10; i++) {
    if (config.line_def[i] == 1) {
      printf("%s%s " RESET "%s", config.art_color, art[i + 1], lines[i]);
    } else {
      printf("%s%s\n" RESET, config.art_color, art[i + 1]);
    }
  }

  printf("%s%s\n", config.art_color, art[11]);
  printf("%s\n" RESET, art[12]);
}
