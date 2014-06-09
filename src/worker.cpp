#include "worker.h"
#include <process.h>
#include "hid.h"

Worker::Worker()
{
    doWorkEvent = CreateEvent(nullptr, false, true, _T("WINK760_DOWORK"));
    exitThreadEvent = CreateEvent(nullptr, false, false, _T("WINK760_EXIT_THREAD"));
    threadHandle = CreateThread(nullptr, 0, &Worker::ThreadProc, this, 0, 0);
}

Worker::~Worker()
{
    CloseHandle(exitThreadEvent);
    CloseHandle(doWorkEvent);
    CloseHandle(threadHandle);
}

DWORD WINAPI Worker::ThreadProc(_In_  LPVOID lpParameter)
{
    Worker* worker = reinterpret_cast<Worker*>(lpParameter);
    return worker->doWork();
}

DWORD Worker::doWork(void)
{
    HANDLE handels[] = {doWorkEvent, exitThreadEvent};
    for (;;)
    {
        DWORD waitResult = WaitForMultipleObjects(2, handels, false, INFINITE);
        switch (waitResult)
        {
            // Do work
            case WAIT_OBJECT_0 + 0:
                setFunctionKeyStatus(true);
                break;
            // Exit thread
            case WAIT_OBJECT_0 + 1:
                return 0;
        }

    }
}

void Worker::Work()
{
    SetEvent(doWorkEvent);
}

void Worker::Exit()
{
    SetEvent(exitThreadEvent);
    WaitForSingleObject(threadHandle, INFINITE);
}

