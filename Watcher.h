#pragma once
#include <QObject>
#include <QFileSystemWatcher>
#include <QThread>

class Watcher : public QThread
{
	Q_OBJECT

public:
	Watcher(const QString&);
	~Watcher();
	void run();

private:
	QString watchPath;
	
signals:
	void fileAdded(const QString&);
	void fileRemoved(const QString&);
	void fileModified(const QString&);
	void fileRenamed(const QString& oldName, const QString& newName);
};

