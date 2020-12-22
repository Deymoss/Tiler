#ifndef THREADCLASS_H
#define THREADCLASS_H

#include <QObject>
#include <QThread>
#include "MainStruct.h"
class ThreadClass : public QThread
{
    Q_OBJECT
public:
    ThreadClass(MainStruct data);
    void run() override;
signals:
    void zoomFinished();
};

#endif // THREADCLASS_H
