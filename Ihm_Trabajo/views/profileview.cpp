#include "profileview.h"
#include "controllers/profilecontroller.h"
#include "ui_profileview.h"
#include <QAction>
#include <QEvent>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>

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
      QIcon(":/resources/icons/eye_closed.svg"), QLineEdit::TrailingPosition);
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
  QList<QAction *> actions = ui->passwordEdit->actions();
  if (!actions.isEmpty()) {
    QAction *toggleAction = actions.last();
    if (ui->passwordEdit->echoMode() == QLineEdit::Password) {
      ui->passwordEdit->setEchoMode(QLineEdit::Normal);
      toggleAction->setIcon(QIcon(":/resources/icons/eye.svg"));
    } else {
      ui->passwordEdit->setEchoMode(QLineEdit::Password);
      toggleAction->setIcon(QIcon(":/resources/icons/eye_closed.svg"));
    }
  }
}

void ProfileView::setupValidation() {
  // Implement real-time validation visual feedback here if needed
}

QPixmap ProfileView::createCircularPixmap(const QImage &image, int size) {
  // Create a square pixmap with the image
  QPixmap source = QPixmap::fromImage(image).scaled(
      size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

  // Create a target pixmap with transparency
  QPixmap target(size, size);
  target.fill(Qt::transparent);

  // Create a painter and set up for circular clipping
  QPainter painter(&target);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);

  // Create circular clip path
  QPainterPath path;
  path.addEllipse(0, 0, size, size);
  painter.setClipPath(path);

  // Draw the image centered within the circular clip
  int x = (size - source.width()) / 2;
  int y = (size - source.height()) / 2;
  painter.drawPixmap(x, y, source);

  return target;
}

bool ProfileView::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->avatarLabel && event->type() == QEvent::MouseButtonRelease) {
    onChangeAvatarClicked();
    return true;
  }
  return QWidget::eventFilter(obj, event);
}
