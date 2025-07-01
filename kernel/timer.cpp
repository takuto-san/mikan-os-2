#include "timer.hpp"

#include "interrupt.hpp"

namespace {
  // 定数とメモリマップドレジスタの定義
  const uint32_t kCountMax = 0xffffffffu;
  volatile uint32_t& lvt_timer = *reinterpret_cast<uint32_t*>(0xfee00320);
  volatile uint32_t& initial_count = *reinterpret_cast<uint32_t*>(0xfee00380);
  volatile uint32_t& current_count = *reinterpret_cast<uint32_t*>(0xfee00390);
  volatile uint32_t& divide_config = *reinterpret_cast<uint32_t*>(0xfee003e0);
}

/**
 * InitializeLAPICTimer
 *   ローカルAPICタイマを初期化する
 *
 *   タイマの分周比を1:1に設定し、タイマモードをPeriodic（周期的）に設定する
 *   割り込みベクタは InterruptVector::kLAPICTimer を使用する
 *   initial_countはInitial Countレジスタに設定する値
 */
// day11c, day11b
void InitializeLAPICTimer() {
  timer_manager = new TimerManager;

  divide_config = 0b1011; // divide 1:1
  lvt_timer = (0b010 << 16) | InterruptVector::kLAPICTimer; // not-masked, periodic
  initial_count = 0x1000000u;
}

/**
 * StartLAPICTimer
 *   ローカルAPICタイマを開始する
 *   initial_countに最大値を設定することでカウンタを動作させる
 */
void StartLAPICTimer() {
  initial_count = kCountMax;
}

/**
 * LAPICTimerElapsed
 *   タイマ開始からの経過時間を取得する
 *
 * @return kCountMaxから現在のカウント値を引いた値（経過時間）
 */
uint32_t LAPICTimerElapsed() {
  return kCountMax - current_count;
}

/**
 * StopLAPICTimer
 *   ローカルAPICタイマを停止する
 *   initial_countに0を設定することでカウンタを停止させる
 */
void StopLAPICTimer() {
  initial_count = 0;
}

/**
 * Tick
 *   割り込み回数を1だけ増やす
 */
void TimerManager::Tick() {
  ++tick_;
}

/** @brief タイマー管理クラスのグローバルインスタンス */
TimerManager* timer_manager;

/**
 * LAPICTimerOnInterrupt
 *   タイマ割り込みのハンドラの中からTick()を呼び出す
 */
void LAPICTimerOnInterrupt() {
  timer_manager->Tick();
}