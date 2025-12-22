#include "profileview.h"
#include "controllers/profilecontroller.h"
#include "ui_profileview.h"
#include <QAction>
#include <QEvent>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>

ProfileView::ProfileView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ProfileView) {
  ui->setupUi(this);

  connect(ui->saveButton, &QPushButton::clicked, this,
          &ProfileView::onSaveClicked);
  connect(ui->cancelButton, &QPushButton::clicked, this,
          &ProfileView::backRequested);
  connect(ui->backToChartButton, &QPushButton::clicked, this,
          &ProfileView::backRequested); // New connection
  // connect(ui->changeAvatarButton, &QPushButton::clicked, this,
  //         &ProfileView::onChangeAvatarClicked);

  // Password visibility toggle setup can be added here
  QAction *toggleAction = ui->passwordEdit->addAction(
      QIcon(":/resources/icons/eye.svg"), QLineEdit::TrailingPosition);
  connect(toggleAction, &QAction::triggered, this,
          &ProfileView::togglePasswordVisibility);

  // Make avatar label clickable
  ui->avatarLabel->installEventFilter(this);
  ui->avatarLabel->setCursor(Qt::PointingHandCursor);
}

ProfileView::~ProfileView() { delete ui; }

void ProfileView::setController(ProfileController *controller) {
  m_controller = controller;

  // Connect controller signals
  connect(m_controller, &ProfileController::updateSuccessful, this,
          [this](const QString &msg) {
            QMessageBox::information(this, "Éxito", msg);
          });
  connect(m_controller, &ProfileController::updateFailed, this,
          [this](const QString &msg) {
            QMessageBox::critical(this, "Error", msg);
          });
}

void ProfileView::loadUser(const User *user) {
  if (!user)
    return;

  m_currentNick = user->nickName();
  m_currentAvatar = user->avatar();

  ui->nickLabel->setText("@" + m_currentNick);
  ui->emailEdit->setText(user->email());
  ui->birthdateEdit->setDate(user->birthdate());
  ui->passwordEdit->clear(); // Don't show password

  if (!m_currentAvatar.isNull()) {
    ui->avatarLabel->setPixmap(
        QPixmap::fromImage(m_currentAvatar)
            .scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
}

void ProfileView::onSaveClicked() {
  if (!m_controller)
    return;

  QString email = ui->emailEdit->text();
  QDate birthdate = ui->birthdateEdit->date();
  QString password = ui->passwordEdit->text(); // Option

  m_controller->updateProfile(m_currentNick, email, password, birthdate,
                              m_currentAvatar);
}

void ProfileView::onChangeAvatarClicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Seleccionar Avatar", "", "Images (*.png *.jpg *.jpeg)");
  if (!fileName.isEmpty()) {
    QImage img(fileName);
    if (!img.isNull()) {
      m_currentAvatar =
          img.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      ui->avatarLabel->setPixmap(
          QPixmap::fromImage(m_currentAvatar)
              .scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  }
}

void ProfileView::togglePasswordVisibility() {
  if (ui->passwordEdit->echoMode() == QLineEdit::Password) {
    ui->passwordEdit->setEchoMode(QLineEdit::Normal);
  } else {
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
  }
}

void ProfileView::setupValidation() {
  // Implement real-time validation visual feedback here if needed
}

bool ProfileView::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->avatarLabel && event->type() == QEvent::MouseButtonRelease) {
    onChangeAvatarClicked();
    return true;
  }
  return QWidget::eventFilter(obj, event);
}
