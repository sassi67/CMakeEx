#include <QObject>

#include "turbofloatex.h"
#include "TurboFloat.h"

#include <QDir>
#include <QCoreApplication>
#include <QLibrary>
#include <QDebug>

const QString PDETS_FILE = "dcs.info";

#ifdef Q_OS_WIN
const QString LIB_FILE = "osg80-osgConn.dll";
#else

#endif

TurboFloatEx::TurboFloatEx(QObject *parent) :
    QObject(parent)
{

}

bool TurboFloatEx::setPathTF()
{
    HRESULT hr = TF_FAIL;
    QDir datFileDir(QCoreApplication::applicationDirPath());
    QString datFile(QDir::toNativeSeparators(QString("%1/%2").arg(datFileDir.absolutePath()).arg(PDETS_FILE)));

    QLibrary library(QDir::toNativeSeparators(QString("%1/%2").arg(datFileDir.absolutePath()).arg(LIB_FILE)));

    if (library.load())
        qDebug() << "library loaded!!";
    else
        qDebug() << library.errorString();

    typedef HRESULT (*PDetsFromPath)(STRCTYPE);

    PDetsFromPath func = (PDetsFromPath)library.resolve("TF_PDetsFromPath");
    if (func) {
#ifdef Q_OS_WIN
        hr = func((const wchar_t*)(datFile.utf16()));
#else
        hr = func(datFile.toStdString().c_str());
#endif
    } else {
        qDebug() << "Error";
    }


    return (TF_OK == hr);
}
