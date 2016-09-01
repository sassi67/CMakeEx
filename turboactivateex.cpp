#include "turboactivateex.h"
#include "TurboActivate.h"

#include <QDir>
#include <QCoreApplication>

const QString PDETS_FILE = "dcs.info";

TurboActivateEx::TurboActivateEx(QObject *parent):
    QObject(parent)
{

}

bool TurboActivateEx::setPathTA()
{
    HRESULT hr = TA_FAIL;
    QDir datFileDir(QCoreApplication::applicationDirPath());
    QString datFile(QDir::toNativeSeparators(QString("%1/%2").arg(datFileDir.absolutePath()).arg(PDETS_FILE)));

#ifdef Q_OS_WIN
    hr = PDetsFromPath((const wchar_t*)(datFile.utf16()));
#else
    hr = PDetsFromPath(datFile.toStdString().c_str());
#endif
    return (TA_OK == hr);
}
