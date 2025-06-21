#include "countersmodel.h"
#include <mutex>

using namespace std;

CountersModel::CountersModel(const std::vector<size_t> &initialCounters)
{
    for(auto counters : initialCounters)
        m_counters.push_back(std::make_unique<atomic_size_t>(counters));
}

void CountersModel::appendCounter(size_t startValue)
{
    std::unique_lock lock(m_mutex);
    m_counters.push_back(std::make_unique<atomic_size_t>(startValue));
}

void CountersModel::deleteCounter(size_t index)
{
    std::unique_lock lock(m_mutex);
    m_counters.erase(m_counters.begin() + index);
}

std::vector<size_t> CountersModel::getCounters() const
{
    std::shared_lock lock(m_mutex);
    std::vector<size_t> result(m_counters.size());
    for (size_t i = 0; i < m_counters.size(); ++i) {
        // В зависимости насколько критично нам видеть точные счётчики
        result[i] = static_cast<size_t>(m_counters[i]->load(std::memory_order_relaxed));
    }
    return result;
}

bool CountersModel::incrementCounter(size_t index)
{
    std::shared_lock lock(m_mutex);
    if(index >= m_counters.size())
    {
        return false;
    }
    m_counters[index]->fetch_add(1);
    return true;
}
