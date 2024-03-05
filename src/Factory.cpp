#include "Factory.h"
#include "CpuMonitor.h"
#include <stdexcept>

std::unique_ptr<Monitor> MonitorFactory::createMonitor(MonitorType type) {
    switch (type) {
        case MonitorType::CPU:
            return std::make_unique<CpuMonitor>();
        default:
            throw std::runtime_error("Unsupported monitor type");
    }
}
