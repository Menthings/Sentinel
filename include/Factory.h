#pragma once
#include "Monitor.h"
#include <memory>

enum class MonitorType {
    CPU
};

class MonitorFactory {
public:
    static std::unique_ptr<Monitor> createMonitor(MonitorType type);
};
