#include "ApplicationContext.h"
#include <QtConcurrent>
#include "../hid/hid.h"

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent), m_Locked(false), m_RemapFunctionKeys(false), m_WatchDevices(false)
{
    Q_CONNECT(RemapFunctionKeysChanged(), RemapFunctionKeysChangedHandler());
    QObject::connect(&this->m_futureWatcher, SIGNAL(finished()), this, SLOT(OperationFinished()));
}

ApplicationContext::~ApplicationContext()
{
   
}

void ApplicationContext::RemapFunctionKeysChangedHandler(void)
{
    this->setLocked(true);
    m_futureWatcher.setFuture(QtConcurrent::run(setFunctionKeyStatus, this->m_RemapFunctionKeys));
}

void ApplicationContext::OperationFinished(void)
{
    this->setLocked(false);
}