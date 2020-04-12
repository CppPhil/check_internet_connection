#include <QApplication>

#include "main_window.hpp"

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  cic::main_window main_window;

  main_window.show();

  return QApplication::exec();
}
