/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2019 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program. If not, see http://www.gnu.org/licenses/.    **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: https://www.polyphone-soundfonts.com                 **
**             Date: 01.01.2013                                           **
***************************************************************************/

#ifndef GRANDORGUESTOP_H
#define GRANDORGUESTOP_H

#include <QMap>
#include "basetypes.h"
class GrandOrgueRank;
class GrandOrguePipe;
class SoundfontManager;

class GrandOrgueStop
{
public:
    GrandOrgueStop(QString rootDir);
    ~GrandOrgueStop();
    void processData(QString key, QString value);
    void createPreset(SoundfontManager * sm, EltID idSf2, QMap<int, GrandOrgueRank*> * ranks);

private:
    QString _rootDir;
    QMap<int, GrandOrguePipe *> _pipes;
    QMap<QString, QString> _properties;
    QMap<int, QMap<QString, QString> > _rankProperties;
};

#endif // GRANDORGUESTOP_H
