/*****************************************************************//**
 * \file   LrDesktopCleanerQt.cpp
 * \brief  
 * 
 * \author Admin
 * \date   June 2020
 *********************************************************************/
#include "LrDesktopCleanerQt.h"

/**
 * ���캯��.
 * 
 * \param parent
 * \return 
 */
LrDesktopCleanerQt::LrDesktopCleanerQt(QWidget *parent)
	: QMainWindow(parent)
{
    addSystemTray();

	ui.setupUi(this);
    setWindowTitle(QStringLiteral(APP_TITLE));
}

/**
 * ���ϵͳ����.
 * 
 */
void LrDesktopCleanerQt::addSystemTray()
{
    QIcon systemIcon(":/LrDesktopCleanerQt/LrDesktopCleanerQt.ico");

    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(systemIcon);
    systemTrayIcon->setToolTip(QStringLiteral(APP_TITLE));
    systemTrayIcon->show();

    connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &LrDesktopCleanerQt::systemTrayActivated);
}

/**
 * �������̵���¼�.
 * 
 * \param reason
 */
void LrDesktopCleanerQt::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //��������ͼ��
        showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        //˫������ͼ��
        emit cleanAllFile();
        break;
    default:
        break;
    }
}

void LrDesktopCleanerQt::receiveCleanAllFileInstruction()
{
    emit cleanAllFile();
}

/**
 * ����رմ����¼�.
 * 
 * \param event
 */
void LrDesktopCleanerQt::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void LrDesktopCleanerQt::setInterval(int i)
{
    ui.secLabel->setText(QString::number(i));
}

/**
 * ����������Ϣ����ʾ������ʾ.
 * 
 * \param succeeded
 * \param fileName
 */
void LrDesktopCleanerQt::receiveFileCleaned(bool succeeded, QString fileName)
{
    fileName = fileName.split("/").back();
    QString tips = (succeeded ? QStringLiteral("ɾ���ɹ���") : QStringLiteral("ɾ��ʧ�ܣ�")) + fileName;
    systemTrayIcon->showMessage(nullptr, tips, QSystemTrayIcon::NoIcon, 5);
}