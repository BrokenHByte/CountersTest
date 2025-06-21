#include "counterscontroller.h"
#include "countersmodel.h"
#include "countersview.h"
#include "dbcontroller.h"

#include <QTimer>

CountersController::CountersController()
{
    m_db = std::make_shared<DBController>();
    auto counters = m_db->loadCounters();
    m_model = std::make_shared<CountersModel>(counters);
    m_view = std::make_shared<CountersView>();
    m_view->show();

    connect(m_view.get(), &CountersView::toAddCounter,  this, &CountersController::onAddCounter);
    connect(m_view.get(), &CountersView::toSaveCounters,  this, &CountersController::onSaveCounters);
    connect(m_view.get(), &CountersView::toDeleteCounter,  this, &CountersController::onDeleteCounter);
    connect(&m_updateFront, &QTimer::timeout, this, &CountersController::onTimeUpdateFront);

    startThreadIncrement();
    m_updateFront.start(100);
}

CountersController::~CountersController()
{
    m_stopIncrementThread.store(true);
    while (!m_threadStopped.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void CountersController::onAddCounter()
{
    m_model->appendCounter(0);
}

void CountersController::onDeleteCounter(size_t index)
{
    m_model->deleteCounter(index);
}

void CountersController::onSaveCounters()
{
    auto counters = m_model->getCounters();
    m_db->saveCounters(counters);
}

void CountersController::onTimeUpdateFront()
{
    auto counters = m_model->getCounters();
    m_view->updateFront(counters);
}

void CountersController::startThreadIncrement()
{
    m_thread = std::make_unique<std::thread>([&model = *m_model,
                                                  &stop = m_stopIncrementThread,
                                                      &confirmation = m_threadStopped]() {
        while(!stop)
        {
            size_t index = 0;
            while(model.incrementCounter(index++)) {}
        }
        confirmation = true;
    });
    m_thread->detach();
}
