#pragma once
#include <QDateTime>
#include <QString>

class FileEntry
{
public:
	QDateTime time;
	QString path;

public:
	FileEntry() : time(), path() {}
	FileEntry(QDateTime dt, QString s) : time(dt), path(s) {}
	bool operator < (const FileEntry& b) { return time > b.time; }
	bool operator == (const FileEntry& b) { return path == b.path; }

    //序列化
    friend QDataStream& operator << (QDataStream& output, const FileEntry& entry)
    {
        output << entry.time << entry.path;
        return output;
    }
    //反序列化
    friend QDataStream& operator >> (QDataStream& input, FileEntry& entry)
    {
        input >> entry.time >> entry.path;
        return input;
    }
};

