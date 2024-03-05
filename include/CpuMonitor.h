#pragma once
#include "Monitor.h"

class CpuMonitor : public Monitor {
public:
    CpuMonitor();
    ~CpuMonitor() override;

    void printLoad() const override;

private:
    double getCPULoad() const;
    double getCPULoadLinux() const;
    double getCPULoadWindows() const;
    double getCPULoadMac() const;
};
