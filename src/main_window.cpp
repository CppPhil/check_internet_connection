#include <chrono>

#include <QTimer>

#include "main_window.hpp"
#include "ping.hpp"

namespace cic {
namespace {
QString map_to_string(bool has_connection) {
  if (has_connection)
    return "Internet connection established.";

  return "Internet connection NOT established!";
}
} // namespace

main_window::main_window(QWidget* parent)
  : QMainWindow(parent), label_(nullptr) {
  setFixedSize(800, 600);

  setWindowTitle("Check internet connection");

  label_ = new QLabel(map_to_string(false), this);
  label_->resize(800, 600);

  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &main_window::on_timer_timeout);
  using namespace std::literals::chrono_literals;
  timer->start(500ms);
}

void main_window::on_timer_timeout() {
  label_->setText(map_to_string(ping("8.8.8.8")));
}
} // namespace cic
