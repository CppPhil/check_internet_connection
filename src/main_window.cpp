#include <chrono>

#include <QTimer>

#include <Poco/Net/ICMPClient.h>
#include <Poco/Net/NetException.h>

#include <cstdio>

#include "main_window.hpp"

namespace cic {
namespace {
QString map_to_string(bool has_connection) {
  if (has_connection)
    return "Internet connection established.";

  return "Internet connection NOT established!";
}
} // namespace

main_window::main_window(QWidget* parent)
  : QMainWindow(parent), label_(map_to_string(false)) {
  setWindowTitle("Check internet connection");

  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &main_window::on_timer_timeout);
  using namespace std::literals::chrono_literals;
  timer->start(500ms);
}

void main_window::on_timer_timeout() {
  try {
    constexpr auto icmp_timeout = 300;

    const auto valid_reply_count = Poco::Net::ICMPClient::pingIPv4(
      /* address */ "8.8.8.8",
      /* repeat */ 1,
      /* dataSize */ 48,
      /* ttl */ 128,
      /* timeout */ icmp_timeout);

    label_.setText(map_to_string(valid_reply_count == 1));
  } catch (const Poco::Net::NetException& ex) {
    fprintf(stderr, "ex: %s\n", ex.what());
  }
}
} // namespace cic
