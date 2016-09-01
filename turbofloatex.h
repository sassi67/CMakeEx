#ifndef TURBOFLOATEX_H
#define TURBOFLOATEX_H

#include <QObject>

class TurboFloatEx : public QObject
{
    Q_OBJECT
public:
    explicit TurboFloatEx(QObject *parent = 0);
    bool setPathTF();

signals:

public slots:
};

#endif // TURBOFLOATEX_H
