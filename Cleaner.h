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
	void add(QString);
	void remove(QString);
	void rename(QString, QString);
	void modify(QString);
	void run();
	void clean();
	void setInterval(const long);
	void setInitialHeap(Heap<FileEntry>);


signals:
	void fileCleaned(bool, QString);
	void heapModified(const Heap<FileEntry>);

public slots:
	void receiveCleanAllFile();
	void receiveFileAdded(QString);
	void receiveFileRemoved(QString);
	void receiveFileModified(QString);
	void receiveFileRenamed(QString oldName, QString newName);

};

