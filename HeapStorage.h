#pragma once
#include <QObject>
#include <QString>
#include "FileEntry.hpp"
#include "Heap.hpp"

class HeapStorage : public QObject
{
	Q_OBJECT
private:
	QString storagePath;

public:
	HeapStorage(QString);
	Heap<FileEntry> fromStorage();

public slots:
	void receiveHeapModified(const Heap<FileEntry>);
};

