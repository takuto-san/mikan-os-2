#pragma once

#include <cstdint>

void InitializeLAPICTimer();
void StartLAPICTimer();
uint32_t LAPICTimerElapsed();
void StopLAPICTimer();

// day11c
/**
 * TimerManager
 *   割り込みの回数を数える
 *   CurrentTick()は現在の割り込み回数を返す
 */
class TimerManager {
 public:
  void Tick();
  unsigned long CurrentTick() const { return tick_; }

 private:
  volatile unsigned long tick_{0};
};

extern TimerManager* timer_manager;

void LAPICTimerOnInterrupt();