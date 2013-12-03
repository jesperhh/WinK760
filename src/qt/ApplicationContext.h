#ifndef APPLICATION_CONTEXT_H
#define APPLICATION_CONTEXT_H

#include <QObject>
#include <QFutureWatcher>

#define Q_PROPERTY_FULL(member, _type) \
    Q_PROPERTY(_type member READ member WRITE set##member NOTIFY member##Changed) \
    public: \
    void set##member##(_type _arg_##member) { if (m_##member != _arg_##member) {m_##member = _arg_##member; Q_EMIT member##Changed();  } } \
    _type member##() const { return m_##member; } \
    Q_SIGNALS: \
    void member##Changed(void); \
    private: \
    _type m_##member;

#define Q_CONNECT(_signal, _slot) \
    QObject::connect(this, SIGNAL(_signal), this, SLOT(_slot))

class ApplicationContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY_FULL(RemapFunctionKeys, bool)
    Q_PROPERTY_FULL(WatchDevices, bool)
    Q_PROPERTY_FULL(Locked, bool)
    Q_PROPERTY_FULL(Battery, int)
    
public:
    explicit ApplicationContext(QObject *parent = 0);
    ~ApplicationContext();
    Q_INVOKABLE void refreshBattery(void);

private slots:
    void RemapFunctionKeysChangedHandler();
    void OperationFinished();

private:
    QFutureWatcher<bool> m_futureWatcher;
};

#endif // APPLICATION_CONTEXT_H
