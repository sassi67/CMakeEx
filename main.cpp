#include <iostream>
#include "turbofloatex.h"
#include "turboactivateex.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TurboFloatEx tf;
    if (tf.setPathTF())
        cout << "TF Path set correctly." << endl;
    else
        cout << "TF Path NOT set correctly." << endl;

    TurboActivateEx ta;
    if (ta.setPathTA())
        cout << "TA Path set correctly." << endl;
    else
        cout << "TA Path NOT set correctly." << endl;

    return 0;
}
