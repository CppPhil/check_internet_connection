#pragma once
#include <QLabel>
#include <QMainWindow>

namespace cic {
class main_window : public QMainWindow {
public:
  explicit main_window(QWidget* parent = nullptr);

private:
  void on_timer_timeout();

  QLabel* label_;
};
} // namespace cic
