#include "turbofloatex.h"
#include "include/TurboActivate/TurboActivate.h"
#include "include/TurboFloat/TurboFloat.h"

#include <QDir>
#include <QCoreApplication>

const QString PDETS_FILE = "TurboActivate.dat";

TurboFloatEx::TurboFloatEx(QObject *parent) : QObject(parent)
{

}

bool TurboFloatEx::setPathTA()
{

    HRESULT hr = TA_FAIL;
    QDir datFileDir(QCoreApplication::applicationDirPath());
    QString datFile(QDir::toNativeSeparators(QString("%1/%2").arg(datFileDir.absolutePath()).arg(PDETS_FILE)));

    hr = PDetsFromPath(datFile.toStdString().c_str());
    return (TA_OK == hr);
}

bool TurboFloatEx::setPathTF()
{
    HRESULT hr = TF_FAIL;
    QDir datFileDir(QCoreApplication::applicationDirPath());
    QString datFile(QDir::toNativeSeparators(QString("%1/%2").arg(datFileDir.absolutePath()).arg(PDETS_FILE)));

    hr = TF_PDetsFromPath(datFile.toStdString().c_str());
    return (TF_OK == hr);
}
