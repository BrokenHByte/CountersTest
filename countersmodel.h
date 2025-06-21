#pragma once

#include <atomic>
#include <memory>
#include <shared_mutex>
#include <vector>

class CountersModel
{
public:
    CountersModel() = default;
    explicit CountersModel(const std::vector<size_t> &initialCounters);
    ~CountersModel() = default;

    void appendCounter(size_t startValue);
    void deleteCounter(size_t index);

    bool incrementCounter(size_t index);
    std::vector<size_t> getCounters() const;
private:
    mutable std::shared_mutex m_mutex;
    std::vector<std::unique_ptr<std::atomic_size_t>> m_counters;
};

