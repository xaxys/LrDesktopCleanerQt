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
    QString desktopPath;                        // �����ļ���·��
    QString configPath;                         // �����ļ�·��
    QString logPath;                            // ��־�ļ�·��
    QString storagePath;                        // �ѳ־û��ļ�·��

    LrDesktopCleanerQt *w;
    Watcher *watcher;
    Cleaner *cleaner;
    HeapStorage *storage;

    void initConfig();

public:
    Manager();
    ~Manager();

};

