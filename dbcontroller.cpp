#include "dbcontroller.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTranslator>

DBController::DBController()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(NAME_BD);

    if (!m_db.open()) {
        qDebug() << QObject::tr("Error: ") << m_db.lastError().text();
        return;
    }

    QSqlQuery initialQuery(INITIAL_SCRIPT, m_db);
    initialQuery.exec();
    if (!initialQuery.exec()) {
        qDebug() << QObject::tr("Initial error:") << initialQuery.lastError().text();
    }
}

DBController::~DBController()
{
    m_db.close();
}

std::vector<size_t> DBController::loadCounters() const
{
    QSqlQuery query(m_db);

    if (!query.exec(SELECT_COUNTERS)) {
        qDebug() << QObject::tr("Insert error:") << query.lastError().text();
    }

    std::vector<size_t> result;
    while (query.next()) {
        QVariantMap row;
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            result.push_back(query.value(0).toULongLong());
        }
    }
    return result;
}

void DBController::saveCounters(const std::vector<size_t> &data)
{
    QSqlQuery query(m_db);
    if (!query.exec(DELETE_COUNTERS)) {
        qDebug() << QObject::tr("Delete error:") << query.lastError().text();
    }

    query.prepare(INSERT_COUNTERS);
    QVariantList list;
    for(auto value : data)
    {
        list << value;
    }

    query.addBindValue(list);
    if (!query.execBatch()) {
        qDebug() << QObject::tr("Insert error:") << query.lastError().text();
    }
}
