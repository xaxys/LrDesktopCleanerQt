#pragma once
#include <thread>
#include <queue>
#include <string>
#include <mutex>
#include <QObject>
#include <QThread>
#include <QDateTime>
#include "Heap.hpp"
#include "FileEntry.hpp"

class Cleaner : public QThread
{
	Q_OBJECT

	long interval;
	Heap<FileEntry> cleanList; // 下次清理
	mutable std::mutex _mutex;

public:
	Cleaner(int);
	~Cleaner();
	void add(const QString&);
	void remove(const QString&);
	void rename(const QString&, const QString&);
	void modify(const QString&);
	void run();
	void clean();
	void setInterval(const long);
	void setInitialHeap(const Heap<FileEntry>);

signals:
	void fileCleaned(bool, const QString&);
	void heapModified(const Heap<FileEntry>);

public slots:
	void receiveCleanAllFile();
	void receiveFileAdded(const QString&);
	void receiveFileRemoved(const QString&);
	void receiveFileModified(const QString&);
	void receiveFileRenamed(const QString& oldName, const QString& newName);

};

