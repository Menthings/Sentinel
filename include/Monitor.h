#pragma once

class Monitor {
public:
    virtual ~Monitor() = default;
    virtual void printLoad() const = 0;
};
