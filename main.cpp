#include <QApplication>
#include <Windows.h>
#include "Manager.h"
int main(int argc, char *argv[])
{
    // »¥³âËø
    HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"LrDesktopCleanerQt");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
        return 0;
    }

	QApplication a(argc, argv);
	Manager m;
	return a.exec();
}

