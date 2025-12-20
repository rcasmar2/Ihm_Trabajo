/********************************************************************************
** Form generated from reading UI file 'resultsview.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTSVIEW_H
#define UI_RESULTSVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ResultsView
{
public:
    QVBoxLayout *mainLayout;
    QFrame *headerFrame;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QSpacerItem *headerSpacer;
    QPushButton *backToChartButton;
    QVBoxLayout *contentLayout;
    QFrame *filterFrame;
    QHBoxLayout *filterLayout;
    QLabel *lblFrom;
    QDateEdit *fromDateEdit;
    QSpacerItem *filterSpacer;
    QPushButton *refreshButton;
    QTableWidget *resultsTable;

    void setupUi(QWidget *ResultsView)
    {
        if (ResultsView->objectName().isEmpty())
            ResultsView->setObjectName("ResultsView");
        ResultsView->resize(900, 700);
        mainLayout = new QVBoxLayout(ResultsView);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        headerFrame = new QFrame(ResultsView);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 60));
        headerLayout = new QHBoxLayout(headerFrame);
        headerLayout->setObjectName("headerLayout");
        headerLayout->setContentsMargins(20, -1, 20, -1);
        titleLabel = new QLabel(headerFrame);
        titleLabel->setObjectName("titleLabel");

        headerLayout->addWidget(titleLabel);

        headerSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);

        backToChartButton = new QPushButton(headerFrame);
        backToChartButton->setObjectName("backToChartButton");
        backToChartButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        headerLayout->addWidget(backToChartButton);


        mainLayout->addWidget(headerFrame);

        contentLayout = new QVBoxLayout();
        contentLayout->setSpacing(20);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(30, 30, 30, 30);
        filterFrame = new QFrame(ResultsView);
        filterFrame->setObjectName("filterFrame");
        filterLayout = new QHBoxLayout(filterFrame);
        filterLayout->setObjectName("filterLayout");
        filterLayout->setContentsMargins(15, 10, 15, 10);
        lblFrom = new QLabel(filterFrame);
        lblFrom->setObjectName("lblFrom");

        filterLayout->addWidget(lblFrom);

        fromDateEdit = new QDateEdit(filterFrame);
        fromDateEdit->setObjectName("fromDateEdit");
        fromDateEdit->setCalendarPopup(true);

        filterLayout->addWidget(fromDateEdit);

        filterSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        filterLayout->addItem(filterSpacer);

        refreshButton = new QPushButton(filterFrame);
        refreshButton->setObjectName("refreshButton");
        refreshButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        filterLayout->addWidget(refreshButton);


        contentLayout->addWidget(filterFrame);

        resultsTable = new QTableWidget(ResultsView);
        if (resultsTable->columnCount() < 4)
            resultsTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        resultsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        resultsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        resultsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        resultsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        resultsTable->setObjectName("resultsTable");
        resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
        resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        resultsTable->setAlternatingRowColors(false);
        resultsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        resultsTable->horizontalHeader()->setStretchLastSection(true);

        contentLayout->addWidget(resultsTable);


        mainLayout->addLayout(contentLayout);


        retranslateUi(ResultsView);

        QMetaObject::connectSlotsByName(ResultsView);
    } // setupUi

    void retranslateUi(QWidget *ResultsView)
    {
        ResultsView->setStyleSheet(QString());
        headerFrame->setStyleSheet(QCoreApplication::translate("ResultsView", "background-color: #1a1a1a; border-bottom: 1px solid #333333;", nullptr));
        titleLabel->setText(QCoreApplication::translate("ResultsView", "\360\237\223\210 Mis Resultados", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("ResultsView", "font-size: 20px; font-weight: bold; color: white; border: none;", nullptr));
        backToChartButton->setText(QCoreApplication::translate("ResultsView", "Volver a la Carta \360\237\227\272\357\270\217", nullptr));
        backToChartButton->setStyleSheet(QCoreApplication::translate("ResultsView", "\n"
"          QPushButton {\n"
"           background-color: #e94560;\n"
"           color: white;\n"
"           border: none;\n"
"           padding: 8px 16px;\n"
"           border-radius: 6px;\n"
"           font-weight: bold;\n"
"           font-size: 13px;\n"
"          }\n"
"          QPushButton:hover {\n"
"           background-color: #ff6b6b;\n"
"          }\n"
"         ", nullptr));
        filterFrame->setStyleSheet(QCoreApplication::translate("ResultsView", "background-color: #1a1a1a; border-radius: 8px; border: 1px solid #333333;", nullptr));
        lblFrom->setText(QCoreApplication::translate("ResultsView", "Mostrar desde:", nullptr));
        lblFrom->setStyleSheet(QCoreApplication::translate("ResultsView", "color: #cccccc; border: none; font-weight: bold;", nullptr));
        fromDateEdit->setDisplayFormat(QCoreApplication::translate("ResultsView", "dd/MM/yyyy", nullptr));
        fromDateEdit->setStyleSheet(QCoreApplication::translate("ResultsView", "\n"
"            QDateEdit {\n"
"             background-color: #2a2a2a;\n"
"             color: white;\n"
"             border: 1px solid #444;\n"
"             padding: 5px 10px;\n"
"             border-radius: 4px;\n"
"             text-decoration: none;\n"
"            }\n"
"            QDateEdit::drop-down {\n"
"             subcontrol-origin: padding;\n"
"             subcontrol-position: top right;\n"
"             width: 20px;\n"
"             border-left-width: 0px;\n"
"            }\n"
"           ", nullptr));
        refreshButton->setText(QCoreApplication::translate("ResultsView", "Aplicar Filtro", nullptr));
        refreshButton->setStyleSheet(QCoreApplication::translate("ResultsView", "\n"
"            QPushButton {\n"
"             background-color: #3498db;\n"
"             color: white;\n"
"             padding: 6px 15px;\n"
"             border-radius: 4px;\n"
"             font-weight: bold;\n"
"             border: none;\n"
"            }\n"
"            QPushButton:hover {\n"
"             background-color: #5dade2;\n"
"            }\n"
"           ", nullptr));
        QTableWidgetItem *___qtablewidgetitem = resultsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ResultsView", "Fecha y Hora", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = resultsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ResultsView", "Aciertos", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = resultsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ResultsView", "Fallos", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = resultsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ResultsView", "Nota Global", nullptr));
        resultsTable->setStyleSheet(QCoreApplication::translate("ResultsView", "\n"
"         QTableWidget {\n"
"          background-color: #1a1a1a;\n"
"          color: #eeeeee;\n"
"          gridline-color: #333333;\n"
"          border: 1px solid #333333;\n"
"          border-radius: 8px;\n"
"          selection-background-color: #34495e;\n"
"         }\n"
"         QHeaderView::section {\n"
"          background-color: #2a2a2a;\n"
"          color: #ffffff;\n"
"          padding: 8px;\n"
"          border: none;\n"
"          border-bottom: 2px solid #e94560;\n"
"          font-weight: bold;\n"
"         }\n"
"         QTableWidget::item {\n"
"          padding: 8px;\n"
"          border-bottom: 1px solid #222;\n"
"         }\n"
"        ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResultsView: public Ui_ResultsView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTSVIEW_H
