#ifndef TURBOACTIVATEEX_H
#define TURBOACTIVATEEX_H

#include <QObject>

class TurboActivateEx : public QObject
{
    Q_OBJECT
public:
    explicit TurboActivateEx(QObject *parent = 0);
    bool setPathTA();

signals:

public slots:
};

#endif // TURBOACTIVATEEX_H
