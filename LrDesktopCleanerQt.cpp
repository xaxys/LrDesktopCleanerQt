/*****************************************************************//**
 * \file   LrDesktopCleanerQt.cpp
 * \brief  
 * 
 * \author Admin
 * \date   June 2020
 *********************************************************************/
#include "LrDesktopCleanerQt.h"

/**
 * 构造函数.
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
 * 添加系统托盘.
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
 * 处理托盘点击事件.
 * 
 * \param reason
 */
void LrDesktopCleanerQt::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //单击托盘图标
        showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标
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
 * 处理关闭窗口事件.
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
 * 接收清理信息，显示气泡提示.
 * 
 * \param succeeded
 * \param fileName
 */
void LrDesktopCleanerQt::receiveFileCleaned(bool succeeded, QString fileName)
{
    fileName = fileName.split("/").back();
    QString tips = (succeeded ? QStringLiteral("删除成功：") : QStringLiteral("删除失败：")) + fileName;
    systemTrayIcon->showMessage(nullptr, tips, QSystemTrayIcon::NoIcon, 5);
}