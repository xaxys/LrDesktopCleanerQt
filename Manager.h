#pragma once
#include <QObject>
#include "Watcher.h"
#include "Cleaner.h"
#include "LrDesktopCleanerQt.h"
#include "HeapStorage.h"

class Manager : public QObject
{
	Q_OBJECT

private:
    int interval;
    QString desktopPath;                        // 桌面文件夹路径
    QString configPath;                         // 配置文件路径
    QString logPath;                            // 日志文件路径
    QString storagePath;                        // 堆持久化文件路径

    LrDesktopCleanerQt *w;
    Watcher *watcher;
    Cleaner *cleaner;
    HeapStorage *storage;

    void initConfig();

public:
    Manager();
    ~Manager();

};

