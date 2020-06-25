#include "HeapStorage.h"
#include <QFile>
#include <QDataStream>

HeapStorage::HeapStorage(QString path) : storagePath(path)
{
}

Heap<FileEntry> HeapStorage::fromStorage()
{
	QFile storageFile(storagePath);
	if (!storageFile.open(QIODevice::ReadOnly)) return Heap<FileEntry>();
	QDataStream in(&storageFile);
	QVector<FileEntry> vec;
	in >> vec;
	Heap<FileEntry> heap(vec.toStdVector());
	storageFile.close();
	return heap;
}

void HeapStorage::receiveHeapModified(const Heap<FileEntry> heap)
{
	QFile storageFile(storagePath);
	if (!storageFile.open(QIODevice::WriteOnly)) return;
	QDataStream output(&storageFile);
	QVector<FileEntry> vec = QVector<FileEntry>::fromStdVector(heap.toVector());
	output << vec;
	storageFile.close();
}