#pragma once

// day11b
struct Message {
  enum Type {
    kInterruptXHCI,
    kInterruptLAPICTimer,
  } type;
};

