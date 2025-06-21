#pragma once

#include <QSqlDatabase>
#include <QObject>
#include <vector>

class DBController
{

public:
    DBController();
    ~DBController();
    std::vector<size_t> loadCounters() const;
    void saveCounters(const std::vector<size_t> &data);
private:
    QSqlDatabase m_db;
    const char* INITIAL_SCRIPT = "CREATE TABLE IF NOT EXISTS counters"
                                 " (value UNSIGNED BIG INT NOT NULL)";
    const char* SELECT_COUNTERS = "SELECT * FROM counters";
    const char* DELETE_COUNTERS = "DELETE FROM counters";
    const char* INSERT_COUNTERS = "INSERT INTO counters (value) VALUES (?)";
    const char* NAME_BD = "counters.db";
};

