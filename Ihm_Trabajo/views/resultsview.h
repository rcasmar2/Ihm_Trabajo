#pragma once

#include <QWidget>
#include "navtypes.h"

namespace Ui {
class ResultsView;
}

class SessionController;

class ResultsView : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsView(QWidget *parent = nullptr);
    ~ResultsView();

    void setController(SessionController *controller);
    void loadData(const User *user);

signals:
    void backRequested();

private slots:
    void onRefreshClicked();

private:
    Ui::ResultsView *ui;
    SessionController *m_controller = nullptr;
    const User *m_currentUser = nullptr;

    void updateTable(const QVector<Session> &sessions);
};
