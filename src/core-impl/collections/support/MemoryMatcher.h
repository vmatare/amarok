/****************************************************************************************
 * Copyright (c) 2007 Nikolaj Hald Nielsen <nhn@kde.org>                                *
 * Copyright (c) 2007 Maximilian Kossick <maximilian.kossick@googlemail.com>            *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#ifndef MEMORYMATCHER_H
#define MEMORYMATCHER_H

#include "amarok_export.h"
#include "MemoryCollection.h"
#include "core/collections/QueryMaker.h"
#include "core/meta/forward_declarations.h"

/**
A helper class for finding items in a MemoryCollection

	@author 
*/
class AMAROK_EXPORT MemoryMatcher{
    public:
        MemoryMatcher();
        virtual ~MemoryMatcher();
        virtual Meta::TrackList match( Collections::MemoryCollection *memColl) = 0;
        virtual Meta::TrackList match( const Meta::TrackList &tracks ) = 0;

        bool isLast() const;
        void setNext( MemoryMatcher *next );
        MemoryMatcher* next() const;

    private:
        MemoryMatcher *m_next;
};


class AMAROK_EXPORT TrackMatcher : public MemoryMatcher
{
    public:
        explicit TrackMatcher( const Meta::TrackPtr &track );
        Meta::TrackList match( Collections::MemoryCollection *memColl ) override;
        Meta::TrackList match( const Meta::TrackList &tracks ) override;

    private:
        Meta::TrackPtr m_track;
};


class AMAROK_EXPORT ArtistMatcher : public MemoryMatcher
{
    public:
        ArtistMatcher( const Meta::ArtistPtr &artist, Collections::QueryMaker::ArtistMatchBehaviour artistMode
                       = Collections::QueryMaker::TrackArtists );
        Meta::TrackList match( Collections::MemoryCollection *memColl ) override;
        Meta::TrackList match( const Meta::TrackList &tracks ) override;

    private:
        Meta::ArtistPtr m_artist;
        Collections::QueryMaker::ArtistMatchBehaviour m_queryMode;
};

class AMAROK_EXPORT AlbumMatcher : public MemoryMatcher
{
    public:
        explicit AlbumMatcher( const Meta::AlbumPtr &album );
        Meta::TrackList match( Collections::MemoryCollection *memColl ) override;
        Meta::TrackList match( const Meta::TrackList &tracks ) override;

    private:
        Meta::AlbumPtr m_album;
};

class AMAROK_EXPORT GenreMatcher : public MemoryMatcher
{
    public:
        explicit GenreMatcher( const Meta::GenrePtr &genre );
        Meta::TrackList match( Collections::MemoryCollection *memColl ) override;
        Meta::TrackList match( const Meta::TrackList &tracks ) override;

    private:
        Meta::GenrePtr m_genre;
};

class AMAROK_EXPORT ComposerMatcher : public MemoryMatcher
{
    public:
        explicit ComposerMatcher( const Meta::ComposerPtr &composer );
        Meta::TrackList match( Collections::MemoryCollection *memColl ) override;
        Meta::TrackList match( const Meta::TrackList &tracks ) override;

    private:
        Meta::ComposerPtr m_composer;
};

class AMAROK_EXPORT YearMatcher : public MemoryMatcher
{
    public:
        explicit YearMatcher( const Meta::YearPtr &year );
        Meta::TrackList match( Collections::MemoryCollection *memColl ) override;
        Meta::TrackList match( const Meta::TrackList &tracks ) override;

    private:
        Meta::YearPtr m_year;
};

class AMAROK_EXPORT LabelMatcher : public MemoryMatcher
{
    public:
        explicit LabelMatcher( const Meta::LabelPtr &label );
        Meta::TrackList match( Collections::MemoryCollection *memColl ) override;
        Meta::TrackList match( const Meta::TrackList &tracks ) override;

    private:
        Meta::LabelPtr m_label;
};


#endif
