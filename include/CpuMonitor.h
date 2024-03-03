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

    double calculateLoadLinux(long long idleTicks, long long totalTicks);
    double calculateLoadWindows();
    double calculateLoadMac();
};
