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
	void fileAdded(QString);
	void fileRemoved(QString);
	void fileModified(QString);
	void fileRenamed(QString oldName, QString newName);
};

