#include "Factory.h"
#include <iostream>
#include <map>
#include <string>

std::string to_string(MonitorType type) {
    switch (type) {
        case MonitorType::CPU: return "CPU";
        default: return "Unknown";
    }
}

int main() {
    std::map<int, MonitorType> menuOptions = {
        {1, MonitorType::CPU},
    };

    std::cout << "Select the option you want to monitor:\n";
    for (const auto& option : menuOptions) {
        std::cout << option.first << ". " << to_string(option.second) << "\n";
    }

    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;

    if (menuOptions.count(choice)) {
        auto monitor = MonitorFactory::createMonitor(menuOptions[choice]);
        monitor->printLoad();
    } else {
        std::cout << "Invalid choice. Please run the program again.\n";
    }

    return 0;
}
