/*****************************************************************//**
 * \file   LrDesktopCleanerQt.h
 * \brief  
 * 
 * \author Admin
 * \date   June 2020
 *********************************************************************/
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LrDesktopCleanerQt.h"
#include "resource.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QFileSystemWatcher>

class LrDesktopCleanerQt : public QMainWindow
{
	Q_OBJECT

public:
	LrDesktopCleanerQt(QWidget *parent = Q_NULLPTR);
	void closeEvent(QCloseEvent* event);
	void setInterval(int);

private:
	void addSystemTray();
	void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
	Ui::LrDesktopCleanerQtClass ui;
	QSystemTrayIcon *systemTrayIcon;

signals:
	void cleanAllFile();

public slots:
	void receiveFileCleaned(bool, QString);
	void receiveCleanAllFileInstruction();

};
