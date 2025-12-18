#include "resultsview.h"
#include "ui_resultsview.h"
#include "controllers/sessioncontroller.h"
#include <QDateTime>

ResultsView::ResultsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultsView)
{
    ui->setupUi(this);
    
    // Set default date to 1 month ago
    ui->fromDateEdit->setDate(QDate::currentDate().addMonths(-1));

    connect(ui->refreshButton, &QPushButton::clicked, this, &ResultsView::onRefreshClicked);
    connect(ui->backToChartButton, &QPushButton::clicked, this, &ResultsView::backRequested); // New connection
}

ResultsView::~ResultsView()
{
    delete ui;
}

void ResultsView::setController(SessionController *controller)
{
    m_controller = controller;
}

void ResultsView::loadData(const User *user)
{
    m_currentUser = user;
    onRefreshClicked();
}

void ResultsView::onRefreshClicked()
{
    if (!m_controller || !m_currentUser) return;

    QDate from = ui->fromDateEdit->date();
    QVector<Session> sessions = m_controller->getSessionHistory(m_currentUser, from);
    updateTable(sessions);
}

void ResultsView::updateTable(const QVector<Session> &sessions)
{
    ui->resultsTable->setRowCount(0);
    
    for (const Session &s : sessions) {
        int row = ui->resultsTable->rowCount();
        ui->resultsTable->insertRow(row);
        
        // Date
        ui->resultsTable->setItem(row, 0, new QTableWidgetItem(s.timeStamp().toString("dd/MM/yyyy HH:mm")));
        
        // Hits
        QTableWidgetItem *hitsItem = new QTableWidgetItem(QString::number(s.hits()));
        hitsItem->setForeground(QBrush(QColor("#27ae60"))); // Green
        ui->resultsTable->setItem(row, 1, hitsItem);
        
        // Faults
        QTableWidgetItem *faultsItem = new QTableWidgetItem(QString::number(s.faults()));
        faultsItem->setForeground(QBrush(QColor("#e74c3c"))); // Red
        ui->resultsTable->setItem(row, 2, faultsItem);
        
        // Score (Simple calculation)
        int total = s.hits() + s.faults();
        double score = total > 0 ? (double(s.hits()) / total) * 10.0 : 0.0;
        ui->resultsTable->setItem(row, 3, new QTableWidgetItem(QString::number(score, 'f', 1)));
    }
}
