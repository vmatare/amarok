/*
   Copyright (C) 2007-2008 Maximilian Kossick <maximilian.kossick@googlemail.com>

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

#ifndef AMAROK_STREAM_P_H
#define AMAROK_STREAM_P_H

#include "debug.h"
#include "EngineController.h"
#include "EngineObserver.h"
#include "Meta.h"
#include "MetaConstants.h"
#include "TheInstances.h"

#include <QList>
#include <QObject>

using namespace MetaStream;

class MetaStream::Track::Private : public QObject, public EngineObserver
{
    Q_OBJECT
    public:
        Private( Track *t )
    : EngineObserver( The::engineController() )
                , track( t )
                {}
                void notify() const
                {
                    foreach( Meta::Observer *observer, observers )
                        observer->metadataChanged( track );
                }

                void engineNewMetaData( const QHash<qint64, QString> &metaData, bool trackChanged )
                {
                    Q_UNUSED( trackChanged )

                    if( metaData.value( Meta::valUrl ) == url.url() ) {
                        debug() << "Applying new Metadata.";
                        if( metaData.contains( Meta::valArtist ) )
                            artist = metaData.value( Meta::valArtist );
                        if( metaData.contains( Meta::valTitle ) )
                            title = metaData.value( Meta::valTitle );
                        if( metaData.contains( Meta::valAlbum ) )
                            album = metaData.value( Meta::valAlbum );
                        notify();
                    }
                }

    public:
        QSet<Meta::Observer*> observers;
        KUrl url;
        QString title;
        QString artist;
        QString album;

        Meta::ArtistPtr artistPtr;
        Meta::AlbumPtr albumPtr;
        Meta::GenrePtr genrePtr;
        Meta::ComposerPtr composerPtr;
        Meta::YearPtr yearPtr;

    private:
        Track *track;
};


// internal helper classes

class StreamArtist : public Meta::Artist
{
    public:
        StreamArtist( MetaStream::Track::Private *dptr )
            : Meta::Artist()
            , d( dptr )
            {}

        Meta::TrackList tracks()
        {
            return Meta::TrackList();
        }

        Meta::AlbumList albums()
        {
            return Meta::AlbumList();
        }

        QString name() const
        {
            if( d )
            {
                return d->artist;
            }
            else
            {
                return QString();
            }
        }

        QString prettyName() const
        {
            return name();
        }

        MetaStream::Track::Private * const d;
};

class StreamAlbum : public Meta::Album
{
public:
    StreamAlbum( MetaStream::Track::Private *dptr )
        : Meta::Album()
        , d( dptr )
    {}

    bool isCompilation() const
    {
        return false;
    }

    bool hasAlbumArtist() const
    {
        return false;
    }

    Meta::ArtistPtr albumArtist() const
    {
        return Meta::ArtistPtr();
    }

    Meta::TrackList tracks()
    {
        return Meta::TrackList();
    }

    QString name() const
    {
        if( d )
            return d->album;
        else
            return QString();
    }

    QString prettyName() const
    {
        return name();
    }

    QPixmap image( int size, bool withShadow )
    {
        return Meta::Album::image( size, withShadow );
    }

    MetaStream::Track::Private * const d;
};


#endif

