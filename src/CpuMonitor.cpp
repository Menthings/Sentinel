#include "CpuMonitor.h"
#include <iostream>
#include <iomanip>
#ifdef __linux__
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono> 
#elif _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach/mach.h>
#endif

CpuMonitor::CpuMonitor() {
    std::cout << "CpuMonitor initialized." << std::endl;
}

CpuMonitor::~CpuMonitor() {
    std::cout << "CpuMonitor destroyed." << std::endl;
}

double CpuMonitor::getCPULoad() {
    #ifdef __linux__
    return getCPULoadLinux();
    #elif _WIN32
    return getCPULoadWindows();
    #elif __APPLE__
    return getCPULoadMac();
    #else
    std::cerr << "System not supported" << std::endl;
    return -1.0;
    #endif
}

void CpuMonitor::printLoad() {
    double load = getCPULoad();
    if (load >= 0) {
        std::cout << "Current CPU load: " << std::fixed << std::setprecision(2) << load << "%" << std::endl;
    } else {
        std::cout << "Unable to recover CPU load" << std::endl;
    }
}

#ifdef __linux__
double CpuMonitor::getCPULoadLinux() {
    std::ifstream fileStart("/proc/stat");
    std::string lineStart;
    std::getline(fileStart, lineStart);
    fileStart.close();

    std::istringstream issStart(lineStart);
    std::string cpuStart;
    long long userStart, niceStart, systemStart, idleStart;
    issStart >> cpuStart >> userStart >> niceStart >> systemStart >> idleStart;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::ifstream fileEnd("/proc/stat");
    std::string lineEnd;
    std::getline(fileEnd, lineEnd);
    fileEnd.close();

    std::istringstream issEnd(lineEnd);
    std::string cpuEnd;
    long long userEnd, niceEnd, systemEnd, idleEnd;
    issEnd >> cpuEnd >> userEnd >> niceEnd >> systemEnd >> idleEnd;

    long long idleDiff = idleEnd - idleStart;
    long long totalDiff = (userEnd + niceEnd + systemEnd + idleEnd) - (userStart + niceStart + systemStart + idleStart);

    return totalDiff == 0 ? 0.0 : 100.0 * (totalDiff - idleDiff) / totalDiff;
}
#endif

#ifdef _WIN32
double CpuMonitor::getCPULoadWindows() {
    static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
    static int numProcessors;
    static bool init = false;
    if (!init) {
        SYSTEM_INFO sysInfo;
        FILETIME ftime, fsys, fuser;
        GetSystemInfo(&sysInfo);
        numProcessors = sysInfo.dwNumberOfProcessors;

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&lastCPU, &ftime, sizeof(FILETIME));

        HANDLE hProcess = GetCurrentProcess();
        GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);
        memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
        memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
        init = true;
    }

    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    HANDLE hProcess = GetCurrentProcess();
    GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));

    percent = double((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart));
    percent /= double(now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;

    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}
#endif

#ifdef __APPLE__
double CpuMonitor::getCPULoadMac() {
    processor_info_array_t cpuInfo;
    mach_msg_type_number_t numCpuInfo;
    natural_t numCPUs;
    unsigned int numCPUsU = 0;
    processor_cpu_load_info_t cpuLoad;
    mach_msg_type_number_t numCpuLoad;

    host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &numCPUs, reinterpret_cast<processor_info_array_t*>(&cpuLoad), &numCpuLoad);

    double totalLoad = 0.0;
    for (unsigned int i = 0; i < numCPUs; ++i) {
        float inUse, total;
        inUse = cpuLoad[i].cpu_ticks[CPU_STATE_USER] + cpuLoad[i].cpu_ticks[CPU_STATE_SYSTEM] + cpuLoad[i].cpu_ticks[CPU_STATE_NICE];
        total = inUse + cpuLoad[i].cpu_ticks[CPU_STATE_IDLE];
        totalLoad += inUse / total;
    }

    vm_deallocate(mach_task_self(), reinterpret_cast<vm_address_t>(cpuLoad), sizeof(cpu_load_info_data_t) * numCPUs);

    return totalLoad / numCPUs * 100;
}
#endif