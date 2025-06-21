#pragma once

#include <QObject>
#include <QTimer>

#include <atomic>
#include <memory>
#include <thread>

class DBController;
class CountersModel;
class CountersView;

class CountersController : public QObject
{
Q_OBJECT
public:
    CountersController();
    ~CountersController();

public slots:
    void onAddCounter();
    void onDeleteCounter(size_t index);
    void onSaveCounters();
    void onTimeUpdateFront();

private:
    void startThreadIncrement();
    std::shared_ptr<CountersModel> m_model;
    std::shared_ptr<CountersView> m_view;
    std::shared_ptr<DBController> m_db;
    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_stopIncrementThread {false};
    std::atomic<bool> m_threadStopped {false};
    QTimer m_updateFront;
};
