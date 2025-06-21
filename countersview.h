#pragma once

#include <QElapsedTimer>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class CountersView;
}
QT_END_NAMESPACE

class CountersView : public QMainWindow {
    Q_OBJECT

public:
    CountersView(QWidget *parent = nullptr);
    ~CountersView();
    void updateFront(const std::vector<size_t> &counters);
public slots:
    void onClickDelete();
signals:
    void toAddCounter();
    void toSaveCounters();
    void toDeleteCounter(size_t row);

private:
    Ui::CountersView *ui;

    QElapsedTimer m_timerFrequency;
    size_t m_oldSum{0};
    size_t m_oldNumber{0};
};
