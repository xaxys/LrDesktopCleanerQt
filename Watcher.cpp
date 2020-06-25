#include <Windows.h>
#include <QDir>
#include "Watcher.h"

Watcher::Watcher(const QString& path)
{
    watchPath = path;
}

Watcher::~Watcher()
{
}

void Watcher::run()
{
    DWORD cbBytes;
    QString fileName;
    QString fileOldName;
    char notify[1024];
    WCHAR dir[MAX_PATH];
    QDir watchDir(watchPath);

    memset(dir, 0, sizeof(dir));
    watchPath.toWCharArray(dir);

    HANDLE dirHandle = CreateFile(dir,
        GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);

    //若网络重定向或目标文件系统不支持该操作，函数失败，同时调用GetLastError()返回ERROR_INVALID_FUNCTION
    if (dirHandle == INVALID_HANDLE_VALUE)
    {
        //std::cout << "error" + GetLastError() << std::endl;
        return;
    }

    memset(notify, 0, strlen(notify));
    FILE_NOTIFY_INFORMATION* pnotify = (FILE_NOTIFY_INFORMATION*)notify;

    //std::cout << "Start Monitor..." << std::endl;

    while (true)
    {
        if (ReadDirectoryChangesW(dirHandle, &notify, 1024, true,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE,
            &cbBytes, NULL, NULL))
        {
            //转换文件名为多字节字符串;
            if (pnotify->FileName) fileName = QString::fromWCharArray(pnotify->FileName, pnotify->FileNameLength >> 1);

            //获取重命名的文件名;
            if (pnotify->NextEntryOffset != 0 && (pnotify->FileNameLength > 0 && pnotify->FileNameLength < MAX_PATH))
            {
                PFILE_NOTIFY_INFORMATION p = (PFILE_NOTIFY_INFORMATION)((char*)pnotify + pnotify->NextEntryOffset);
                fileOldName = fileName;
                fileName = QString::fromWCharArray(p->FileName, p->FileNameLength >> 1);
            }

            // 获得扩展名
            QString ext = fileName.right(4).toLower();

            //设置类型过滤器,监听文件创建、更改、删除、重命名等;
            if (ext == ".mp4" || ext == ".flv" || ext == ".avi") {
                switch (pnotify->Action)
                {
                case FILE_ACTION_ADDED:
                    emit fileAdded(watchDir.absoluteFilePath(fileName));
                    break;

                case FILE_ACTION_MODIFIED:
                    emit fileModified(watchDir.absoluteFilePath(fileName));
                    break;

                case FILE_ACTION_REMOVED:
                    emit fileRemoved(watchDir.absoluteFilePath(fileName));
                    break;

                case FILE_ACTION_RENAMED_OLD_NAME:
                    emit fileRenamed(watchDir.absoluteFilePath(fileOldName), watchDir.absoluteFilePath(fileName));
                    break;

                default:
                    //std::cout << "Unknow command!" << std::endl;
                    break;

                }
            }
        }
    }

    CloseHandle(dirHandle);
}
