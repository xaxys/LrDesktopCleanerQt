#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include "Manager.h"

Manager::Manager()
{
    initConfig();

    w = new LrDesktopCleanerQt();
    watcher = new Watcher(desktopPath);
    cleaner = new Cleaner(interval);
    storage = new HeapStorage(storagePath);
    connect(w, &LrDesktopCleanerQt::cleanAllFile, cleaner, &Cleaner::receiveCleanAllFile);
    connect(watcher, &Watcher::fileAdded, cleaner, &Cleaner::receiveFileAdded);
    connect(watcher, &Watcher::fileRemoved, cleaner, &Cleaner::receiveFileRemoved);
    connect(watcher, &Watcher::fileModified, cleaner, &Cleaner::receiveFileModified);
    connect(watcher, &Watcher::fileRenamed, cleaner, &Cleaner::receiveFileRenamed);
    connect(cleaner, &Cleaner::fileCleaned, w, &LrDesktopCleanerQt::receiveFileCleaned);
    connect(cleaner, &Cleaner::heapModified, storage, &HeapStorage::receiveHeapModified);
    w->setInterval(interval);
    cleaner->setInitialHeap(storage->fromStorage());
    watcher->start();
    cleaner->start();
}

Manager::~Manager()
{
    watcher->terminate();
    cleaner->terminate();
    w->close();
    delete watcher;
    delete cleaner;
    delete storage;
    delete w;
}


void Manager::initConfig()
{
    interval = 3600;

    desktopPath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0];
    QDir appLocalDataDir = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
    if (!appLocalDataDir.exists()) appLocalDataDir.mkdir(appLocalDataDir.absolutePath());

    configPath = appLocalDataDir.absoluteFilePath(QStringLiteral("interval.txt"));
    logPath = appLocalDataDir.absoluteFilePath(QStringLiteral("log.txt"));
    storagePath = appLocalDataDir.absoluteFilePath(QStringLiteral("storage.heap"));

    QFile config(configPath);
    if (config.exists())
    {
        config.open(QIODevice::ReadOnly | QIODevice::Text);
        interval = config.readLine().toInt();
    }
    else
    {
        config.open(QIODevice::WriteOnly | QIODevice::Text);
        config.write(QByteArray::number(interval));
        config.close();
    }
    config.close();
}