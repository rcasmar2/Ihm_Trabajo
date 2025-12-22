#pragma once

#include <QWidget>
#include "navtypes.h"

namespace Ui {
class ProfileView;
}

class ProfileController;

class ProfileView : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileView(QWidget *parent = nullptr);
    ~ProfileView();

    void setController(ProfileController *controller);
    void loadUser(const User *user);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void backRequested();

private slots:
    void onSaveClicked();
    void onChangeAvatarClicked();
    void togglePasswordVisibility();

private:
    Ui::ProfileView *ui;
    ProfileController *m_controller = nullptr;
    QString m_currentNick;
    QImage m_currentAvatar;

    void setupValidation();
};
