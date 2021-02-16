#include "findnecessarytile.h"

FindNecessaryTile::FindNecessaryTile(QVector<ConstantStruct> constants)
{
    this->constants = constants;
}

int FindNecessaryTile::getTile(int x, int y, int zoom)
{
    int countOfXNum = x-constants.at(zoom-1).xTileStart;
    int countOfYnum = y - constants.at(zoom-1).yTileStart;
    int countTls = constants.at(zoom-1).xTileCount*countOfYnum + countOfXNum;
    int result = 0;
    for(int k = 0; k<zoom-1; k++)
    {
        result += constants.at(k).countOfTiles;
    }
    countTls += result;
    qDebug()<<countTls;
    return countTls;
}
