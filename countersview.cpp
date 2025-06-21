#include "countersview.h"
#include "ui_countersview.h"

#include <QPushButton>

CountersView::CountersView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CountersView) {
    ui->setupUi(this);
    ui->table->setColumnCount(1);
    ui->table->setHorizontalHeaderLabels({tr("Counters")});

    connect(ui->btnAdd, &QPushButton::pressed, this, &CountersView::toAddCounter);
    connect(ui->btnSave, &QPushButton::pressed, this,
            &CountersView::toSaveCounters);
    connect(ui->btnDelete, &QPushButton::pressed, this,
            &CountersView::onClickDelete);
}

CountersView::~CountersView() { delete ui; }

void CountersView::updateFront(const std::vector<size_t> &counters) {
    double seconds = 1.0 * m_timerFrequency.nsecsElapsed() / 1.0e+9;
    m_timerFrequency.restart();

    ui->table->setRowCount(counters.size());
    size_t sum = 0;
    for (size_t i = 0; i < counters.size(); i++) {
        sum += counters[i];
        auto item = ui->table->item(i, 0);
        if (!item) {
            item = new QTableWidgetItem();
            ui->table->setItem(i, 0, item);
        }
        item->setText(QString::number(counters[i]));
    }

    if (m_oldNumber == counters.size()) {
        double frequency = (sum - m_oldSum) / seconds;
        ui->labelFrequency->setText(
            QString(tr("Frequency %1 incr./sec.")).arg(frequency, 0, 'f', 2));
    }
    m_oldSum = sum;
    m_oldNumber = counters.size();
}

void CountersView::onClickDelete() {
    if (ui->table->currentRow() < 0) {
        return;
    }
    emit toDeleteCounter(ui->table->currentRow());
}
