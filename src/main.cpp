#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>

#include <QApplication>

#include "main_window.hpp"

int main(int argc, char* argv[]) {
  if (geteuid()) {
    fprintf(stderr,
            "Not running as root.\nThis application need to be run as root.\n");
    return EXIT_FAILURE;
  }

  QApplication application(argc, argv);
  cic::main_window main_window;

  main_window.show();

  return QApplication::exec();
}
