#pragma once

class CpuMonitor {
public:
    CpuMonitor();
    ~CpuMonitor();

    double getCPULoad();
    void printLoad();

private:
    double getCPULoadLinux();
    double getCPULoadWindows();
    double getCPULoadMac();
};
