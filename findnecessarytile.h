#ifndef FINDNECESSARYTILE_H
#define FINDNECESSARYTILE_H
#include <QDebug>
#include "MainStruct.h"

class FindNecessaryTile
{
public:
    FindNecessaryTile(QVector<ConstantStruct> constants);
    int getTile(int x, int y, int zoom);
    QVector<ConstantStruct> constants;
};

#endif // FINDNECESSARYTILE_H
