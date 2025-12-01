#include "display/display.h"
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
  printf("%s\n", art[0]);

  for (int i = 0; i < 10; i++) {
    if (config.line_def[i] == 1) {
      printf("%s %s", art[i + 1], lines[i]);
    } else {
      printf("%s\n", art[i + 1]);
    }
  }

  printf("%s\n", art[11]);
  printf("%s\n", art[12]);
}
