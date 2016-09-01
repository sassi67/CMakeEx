#include "turbofloatex.h"
#include "TurboFloat.h"

#include <QDir>
#include <QCoreApplication>

const QString PDETS_FILE = "dcs.info";

TurboFloatEx::TurboFloatEx(QObject *parent) :
    QObject(parent)
{

}

bool TurboFloatEx::setPathTF()
{
    HRESULT hr = TF_FAIL;
    QDir datFileDir(QCoreApplication::applicationDirPath());
    QString datFile(QDir::toNativeSeparators(QString("%1/%2").arg(datFileDir.absolutePath()).arg(PDETS_FILE)));

#ifdef Q_OS_WIN
    hr = TF_PDetsFromPath((const wchar_t*)(datFile.utf16()));
#else
    hr = TF_PDetsFromPath(datFile.toStdString().c_str());
#endif
    return (TF_OK == hr);
}
