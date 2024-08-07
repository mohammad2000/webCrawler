/********************************************************************************
** Form generated from reading UI file 'qxServer.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QXSERVER_H
#define UI_QXSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dlgServer
{
public:
    QGroupBox *grpError;
    QPlainTextEdit *txtError;
    QGroupBox *grpServerParams;
    QLabel *lblThreadCount;
    QLabel *lblSerializationType;
    QComboBox *cboSerializationType;
    QLabel *lblPortNumber;
    QSpinBox *spinPortNumber;
    QCheckBox *chkCompressData;
    QPushButton *btnStartStop;
    QFrame *lineServerParams;
    QLabel *imgIsRunning;
    QSpinBox *spinThreadCount;
    QCheckBox *chkEncryptData;
    QGroupBox *grpTransaction;
    QPlainTextEdit *txtTransaction;

    void setupUi(QWidget *dlgServer)
    {
        if (dlgServer->objectName().isEmpty())
            dlgServer->setObjectName(QString::fromUtf8("dlgServer"));
        dlgServer->resize(552, 392);
        dlgServer->setMinimumSize(QSize(552, 392));
        dlgServer->setMaximumSize(QSize(552, 392));
        grpError = new QGroupBox(dlgServer);
        grpError->setObjectName(QString::fromUtf8("grpError"));
        grpError->setGeometry(QRect(10, 310, 531, 71));
        txtError = new QPlainTextEdit(grpError);
        txtError->setObjectName(QString::fromUtf8("txtError"));
        txtError->setGeometry(QRect(10, 20, 511, 41));
        QPalette palette;
        QBrush brush(QColor(170, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(112, 111, 113, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        txtError->setPalette(palette);
        txtError->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        txtError->setReadOnly(true);
        grpServerParams = new QGroupBox(dlgServer);
        grpServerParams->setObjectName(QString::fromUtf8("grpServerParams"));
        grpServerParams->setGeometry(QRect(10, 10, 531, 81));
        lblThreadCount = new QLabel(grpServerParams);
        lblThreadCount->setObjectName(QString::fromUtf8("lblThreadCount"));
        lblThreadCount->setGeometry(QRect(180, 15, 81, 16));
        lblSerializationType = new QLabel(grpServerParams);
        lblSerializationType->setObjectName(QString::fromUtf8("lblSerializationType"));
        lblSerializationType->setGeometry(QRect(10, 50, 91, 16));
        cboSerializationType = new QComboBox(grpServerParams);
        cboSerializationType->setObjectName(QString::fromUtf8("cboSerializationType"));
        cboSerializationType->setGeometry(QRect(110, 50, 51, 20));
        lblPortNumber = new QLabel(grpServerParams);
        lblPortNumber->setObjectName(QString::fromUtf8("lblPortNumber"));
        lblPortNumber->setGeometry(QRect(10, 20, 81, 16));
        spinPortNumber = new QSpinBox(grpServerParams);
        spinPortNumber->setObjectName(QString::fromUtf8("spinPortNumber"));
        spinPortNumber->setGeometry(QRect(110, 20, 51, 22));
        spinPortNumber->setMinimum(1);
        spinPortNumber->setMaximum(32000);
        chkCompressData = new QCheckBox(grpServerParams);
        chkCompressData->setObjectName(QString::fromUtf8("chkCompressData"));
        chkCompressData->setGeometry(QRect(180, 40, 101, 17));
        btnStartStop = new QPushButton(grpServerParams);
        btnStartStop->setObjectName(QString::fromUtf8("btnStartStop"));
        btnStartStop->setGeometry(QRect(330, 20, 111, 51));
        QFont font;
        font.setBold(true);
        btnStartStop->setFont(font);
        lineServerParams = new QFrame(grpServerParams);
        lineServerParams->setObjectName(QString::fromUtf8("lineServerParams"));
        lineServerParams->setGeometry(QRect(320, 10, 3, 61));
        lineServerParams->setFrameShape(QFrame::VLine);
        lineServerParams->setFrameShadow(QFrame::Sunken);
        imgIsRunning = new QLabel(grpServerParams);
        imgIsRunning->setObjectName(QString::fromUtf8("imgIsRunning"));
        imgIsRunning->setGeometry(QRect(450, 20, 71, 51));
        imgIsRunning->setAlignment(Qt::AlignCenter);
        spinThreadCount = new QSpinBox(grpServerParams);
        spinThreadCount->setObjectName(QString::fromUtf8("spinThreadCount"));
        spinThreadCount->setGeometry(QRect(260, 15, 51, 22));
        spinThreadCount->setMinimum(1);
        spinThreadCount->setMaximum(999);
        chkEncryptData = new QCheckBox(grpServerParams);
        chkEncryptData->setObjectName(QString::fromUtf8("chkEncryptData"));
        chkEncryptData->setGeometry(QRect(180, 55, 101, 17));
        grpTransaction = new QGroupBox(dlgServer);
        grpTransaction->setObjectName(QString::fromUtf8("grpTransaction"));
        grpTransaction->setGeometry(QRect(10, 100, 531, 201));
        txtTransaction = new QPlainTextEdit(grpTransaction);
        txtTransaction->setObjectName(QString::fromUtf8("txtTransaction"));
        txtTransaction->setGeometry(QRect(10, 20, 511, 171));
        txtTransaction->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        txtTransaction->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        txtTransaction->setLineWrapMode(QPlainTextEdit::NoWrap);
        txtTransaction->setReadOnly(true);

        retranslateUi(dlgServer);

        QMetaObject::connectSlotsByName(dlgServer);
    } // setupUi

    void retranslateUi(QWidget *dlgServer)
    {
        dlgServer->setWindowTitle(QCoreApplication::translate("dlgServer", "qxServer", nullptr));
        grpError->setTitle(QCoreApplication::translate("dlgServer", "Log last server error", nullptr));
        grpServerParams->setTitle(QCoreApplication::translate("dlgServer", "Server parameters", nullptr));
#if QT_CONFIG(tooltip)
        lblThreadCount->setToolTip(QCoreApplication::translate("dlgServer", "Thread Count", nullptr));
#endif // QT_CONFIG(tooltip)
        lblThreadCount->setText(QCoreApplication::translate("dlgServer", "Thread Count :", nullptr));
#if QT_CONFIG(tooltip)
        lblSerializationType->setToolTip(QCoreApplication::translate("dlgServer", "Serialization Type", nullptr));
#endif // QT_CONFIG(tooltip)
        lblSerializationType->setText(QCoreApplication::translate("dlgServer", "Serialization Type :", nullptr));
#if QT_CONFIG(tooltip)
        cboSerializationType->setToolTip(QCoreApplication::translate("dlgServer", "Serialization Type", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lblPortNumber->setToolTip(QCoreApplication::translate("dlgServer", "Port Number", nullptr));
#endif // QT_CONFIG(tooltip)
        lblPortNumber->setText(QCoreApplication::translate("dlgServer", "Port Number :", nullptr));
#if QT_CONFIG(tooltip)
        spinPortNumber->setToolTip(QCoreApplication::translate("dlgServer", "Port Number", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        chkCompressData->setToolTip(QCoreApplication::translate("dlgServer", "Compress Data", nullptr));
#endif // QT_CONFIG(tooltip)
        chkCompressData->setText(QCoreApplication::translate("dlgServer", "Compress Data", nullptr));
        btnStartStop->setText(QCoreApplication::translate("dlgServer", "Start Server /\n"
"Stop Server", nullptr));
        imgIsRunning->setText(QCoreApplication::translate("dlgServer", "[ is running ? ]", nullptr));
#if QT_CONFIG(tooltip)
        spinThreadCount->setToolTip(QCoreApplication::translate("dlgServer", "Thread Count", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        chkEncryptData->setToolTip(QCoreApplication::translate("dlgServer", "Encrypt Data", nullptr));
#endif // QT_CONFIG(tooltip)
        chkEncryptData->setText(QCoreApplication::translate("dlgServer", "Encrypt Data", nullptr));
        grpTransaction->setTitle(QCoreApplication::translate("dlgServer", "Log last client-server reply-request transaction", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dlgServer: public Ui_dlgServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QXSERVER_H
