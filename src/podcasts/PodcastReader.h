/* This file is part of the KDE project
   Copyright (C) 2007 Bart Cerneels <bart.cerneels@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#ifndef PODCASTREADER_H
#define PODCASTREADER_H

#include "PodcastCollection.h"
#include "PodcastMetaBase.h"

#include <QXmlStreamReader>

/**
	@author Bart Cerneels <bart.cerneels@gmail.com>
*/
class PodcastReader : public QXmlStreamReader
{
public:
    PodcastReader( PodcastCollection * collection );

    bool read( QIODevice *device );

    ~PodcastReader();
private:
    void readRss();
    void readChannel();
    PodcastTrackPtr readItem();
    QString readTitle();
    QString readLink();
    QString readDescription();
    QString readEnclosure();
    QString readGuid();
    QString readPubDate();
    void readUnknownElement();
//     void readImage();
//     void readCopyright();
    PodcastCollection * m_collection;
};

#endif
