#include <Windows.h>
#include "Cleaner.h"
#include <Qfile>

Cleaner::Cleaner(int i) : interval(i)
{
}

Cleaner::~Cleaner()
{
}


void Cleaner::add(QString s)
{
	_mutex.lock();
	cleanList.push(FileEntry(QDateTime::currentDateTime(), s));
	emit heapModified(cleanList);
	_mutex.unlock();
}

void Cleaner::remove(QString s)
{
	_mutex.lock();
	cleanList.remove(FileEntry(QDateTime::currentDateTime(), s));
	emit heapModified(cleanList);
	_mutex.unlock();
}

void Cleaner::rename(QString s, QString re)
{
	_mutex.lock();
	QDateTime dt = QDateTime::currentDateTime();
	cleanList.remove(FileEntry(QDateTime::fromSecsSinceEpoch(0), s));
	cleanList.push(FileEntry(QDateTime::currentDateTime(), re));
	emit heapModified(cleanList);
	_mutex.unlock();
}

void Cleaner::modify(QString s)
{
	_mutex.lock();
	cleanList.push(FileEntry(QDateTime::currentDateTime(), s));
	emit heapModified(cleanList);
	_mutex.unlock();
}

// Cleaner::clean() 清理队列第一个
void Cleaner::clean()
{
	_mutex.lock();
	if (cleanList.empty()) return;
	QString s = cleanList.pop().path;
	emit heapModified(cleanList);
	_mutex.unlock();

	bool succeeded = QFile::remove(s);
	emit fileCleaned(succeeded, s);
}

void Cleaner::run()
{
	while (true)
	{
		if (!cleanList.empty()) {
			_mutex.lock();
			FileEntry entry = cleanList.top();
			_mutex.unlock();
			if (entry.time.toMSecsSinceEpoch() != 0
				&& entry.time.addSecs(interval) <= QDateTime::currentDateTime())
			{
				clean();
			}
		}
		Sleep(50);
	}
}

void Cleaner::setInterval(const long l)
{
	interval = l;
}

void Cleaner::setInitialHeap(Heap<FileEntry> heap)
{
	cleanList = heap;
}

void Cleaner::receiveCleanAllFile()
{
	bool empty = cleanList.empty();
	while (!empty)
	{
		clean();
		_mutex.lock();
		empty = cleanList.empty();
		_mutex.unlock();
	}
}


void Cleaner::receiveFileAdded(QString s)
{
	add(s);
}

void Cleaner::receiveFileRemoved(QString s)
{
	remove(s);
}

void Cleaner::receiveFileModified(QString s)
{
	modify(s);
}

void Cleaner::receiveFileRenamed(QString oldName, QString newName)
{
	rename(oldName, newName);
}