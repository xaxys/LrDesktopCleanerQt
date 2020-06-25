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

    //�������ض����Ŀ���ļ�ϵͳ��֧�ָò���������ʧ�ܣ�ͬʱ����GetLastError()����ERROR_INVALID_FUNCTION
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
            //ת���ļ���Ϊ���ֽ��ַ���;
            if (pnotify->FileName) fileName = QString::fromWCharArray(pnotify->FileName, pnotify->FileNameLength >> 1);

            //��ȡ���������ļ���;
            if (pnotify->NextEntryOffset != 0 && (pnotify->FileNameLength > 0 && pnotify->FileNameLength < MAX_PATH))
            {
                PFILE_NOTIFY_INFORMATION p = (PFILE_NOTIFY_INFORMATION)((char*)pnotify + pnotify->NextEntryOffset);
                fileOldName = fileName;
                fileName = QString::fromWCharArray(p->FileName, p->FileNameLength >> 1);
            }

            // �����չ��
            QString ext = fileName.right(4).toLower();

            //�������͹�����,�����ļ����������ġ�ɾ������������;
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
