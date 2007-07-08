/* This file is part of the KDE project
   Copyright (C) 2007 Maximilian Kossick <maximilian.kossick@googlemail.com>

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

#define DEBUG_PREFIX "ServiceSqlRegistry"

#include "ServiceSqlRegistry.h"

#include "debug.h"


#include <QHashIterator>
#include <QMutableHashIterator>
#include <QMutexLocker>

ServiceSqlRegistry::ServiceSqlRegistry( ServiceMetaFactory * metaFactory )
    : QObject( 0 )
    , m_metaFactory( metaFactory )
{
    setObjectName( "ServiceSqlRegistry" );

   /* m_timer = new QTimer( this );
    m_timer->setInterval( 60000 );  //try to clean up every 60 seconds, change if necessary
    m_timer->setSingleShot( false );
    connect( m_timer, SIGNAL( timeout() ), this, SLOT( emptyCache() ) );
    m_timer->start();*/
}

ServiceSqlRegistry::~ServiceSqlRegistry()
{
    //don't delete m_collection
}



TrackPtr
ServiceSqlRegistry::getTrack( const QStringList &rowData )
{
    
   // DEBUG_BLOCK

    int id = rowData[0].toInt();


    QMutexLocker locker( &m_trackMutex );
    if( m_trackMap.contains( id ) )
        return m_trackMap.value( id );
    else
    {

        int index = 0;
        TrackPtr trackPtr = m_metaFactory->createTrack( rowData.mid(index, m_metaFactory->getTrackSqlRowCount() ) );
        index += m_metaFactory->getTrackSqlRowCount();

        ServiceTrack * track = dynamic_cast<ServiceTrack *> ( trackPtr.data() );

        AlbumPtr albumPtr;

        

        if ( m_albumMap.contains( track->albumId() ) )
            albumPtr = m_albumMap.value( track->albumId() );
        else {

            QStringList subRows = rowData.mid(index, m_metaFactory->getAlbumSqlRowCount() );

            albumPtr = m_metaFactory->createAlbum( subRows );
        }

        index += m_metaFactory->getAlbumSqlRowCount();

        ServiceAlbum * album = dynamic_cast<ServiceAlbum *> ( albumPtr.data() );

        album->addTrack( trackPtr );
        track->setAlbum( albumPtr );

        m_albumMap.insert( track->albumId(), albumPtr );


        ArtistPtr artistPtr;
        debug() << "get artist with id " << track->artistId() << endl;

        if ( m_artistMap.contains( track->artistId() ) )
            artistPtr = m_artistMap.value( track->artistId() );
        else {
            
           
            debug() << "not in registry, lets create it" << endl;
            debug() << "Row length: " << rowData.count() << endl;
            //debug() << "Track row: " << rowData << endl;

            debug() << "index: " << index << endl;
            debug() << "length: " <<  m_metaFactory->getArtistSqlRowCount() << endl; 

            QStringList subRows = rowData.mid(index, m_metaFactory->getArtistSqlRowCount() );

            debug() << "Artist sub row: " << subRows << endl;

            artistPtr = m_metaFactory->createArtist( subRows );
        }

        index += m_metaFactory->getArtistSqlRowCount();
       
        ServiceArtist * artist = dynamic_cast<ServiceArtist *> ( artistPtr.data() );

        artist->addTrack( trackPtr );
        track->setArtist( artistPtr );

        m_artistMap.insert( track->artistId(), artistPtr );



        GenrePtr genrePtr;

        int genreId = rowData[index].toInt();


        if ( m_genreMap.contains( genreId ) )
            genrePtr = m_genreMap.value( genreId );
        else {

            genrePtr = m_metaFactory->createGenre( rowData.mid(index, m_metaFactory->getGenreSqlRowCount() ) );
        }

        //index += m_metaFactory->getGenreSqlRowCount();

        ServiceGenre * genre = dynamic_cast<ServiceGenre *> ( genrePtr.data() );

        genre->addTrack( trackPtr );
        track->setGenre( genrePtr );

        m_genreMap.insert( genreId, genrePtr );

        m_trackMap.insert( id, trackPtr );
        return trackPtr;
    }
}

ArtistPtr
ServiceSqlRegistry::getArtist( const QStringList &rowData )
{

    int id = rowData[0].toInt();

    

    QMutexLocker locker( &m_artistMutex );
    if( m_artistMap.contains( id ) )
        return m_artistMap.value( id );
    else
    {
        ArtistPtr artist = m_metaFactory->createArtist( rowData );
        m_artistMap.insert( id, artist );
        return artist;
    }
}

GenrePtr
ServiceSqlRegistry::getGenre( const QStringList &rowData ) 
{
    int id = rowData[0].toInt();
    debug() << "Genre row: " << rowData << endl;
    debug() << "Create genre with id: " << id << endl;
    QMutexLocker locker( &m_genreMutex );
    if( m_genreMap.contains( id ) )
        return m_genreMap.value( id );
    else
    {
        GenrePtr genre = m_metaFactory->createGenre( rowData );
        m_genreMap.insert( id, genre );
        return genre;
    }
}

/*ComposerPtr
ServiceSqlRegistry::getComposer( const QString &name, int id )
{
    QMutexLocker locker( &m_composerMutex );
    if( m_composerMap.contains( name ) )
        return m_composerMap.value( name );
    else
    {
        ComposerPtr composer( new SqlComposer( m_collection, id, name ) );
        m_composerMap.insert( name, composer );
        return composer;
    }
}*/

/*YearPtr
ServiceSqlRegistry::getYear( const QString &name, int id )
{
    QMutexLocker locker( &m_yearMutex );
    if( m_yearMap.contains( name ) )
        return m_yearMap.value( name );
    else
    {
        YearPtr year( new SqlYear( m_collection, id, name ) );
        m_yearMap.insert( name, year );
        return year;
    }
}*/

AlbumPtr
ServiceSqlRegistry::getAlbum(  const QStringList &rowData)
{

    int id = rowData[0].toInt();
    QMutexLocker locker( &m_albumMutex );
    if( m_albumMap.contains( id ) )
        return m_albumMap.value( id );
    else
    {
        AlbumPtr album = m_metaFactory->createAlbum( rowData );
        m_albumMap.insert( id, album );
        return album;
    }
}

/*
void
ServiceSqlRegistry::emptyCache()
{
    DEBUG_BLOCK
    bool hasTrack, hasAlbum, hasArtist, hasYear, hasGenre, hasComposer;
    hasTrack = hasAlbum = hasArtist = hasYear = hasGenre = hasComposer = false;

    //try to avoid possible deadlocks by aborting when we can't get all locks
    if ( ( hasTrack = m_trackMutex.tryLock() )
         && ( hasAlbum = m_albumMutex.tryLock() )
         && ( hasArtist = m_artistMutex.tryLock() )
         && ( hasYear = m_yearMutex.tryLock() )
         && ( hasGenre = m_genreMutex.tryLock() )
         && ( hasComposer = m_composerMutex.tryLock() ) )
    {
        //this very simple garbage collector doesn't handle cyclic object graphs
        //so care has to be taken to make sure that we are not dealing with a cyclic graph
        //by invalidating the tracks cache on all objects
        #define foreachInvalidateCache( Type, RealType, x ) \
        for( QMutableHashIterator<QString,Type > iter(x); iter.hasNext(); ) \
            RealType::staticCast( iter.next().value() )->invalidateCache()

        foreachInvalidateCache( AlbumPtr, KSharedPtr<SqlAlbum>, m_albumMap );
        foreachInvalidateCache( ArtistPtr, KSharedPtr<SqlArtist>, m_artistMap );
        foreachInvalidateCache( GenrePtr, KSharedPtr<SqlGenre>, m_genreMap );
        foreachInvalidateCache( ComposerPtr, KSharedPtr<SqlComposer>, m_composerMap );
        foreachInvalidateCache( YearPtr, KSharedPtr<SqlYear>, m_yearMap );

        //elem.count() == 2 is correct because elem is one pointer to the object
        //and the other is stored in the hash map
        #define foreachCollectGarbage( Key, Type, x ) \
        for( QMutableHashIterator<Key,Type > iter(x); iter.hasNext(); ) \
        { \
            Type elem = iter.next().value(); \
            if( elem.count() == 2 ) \
                iter.remove(); \
        }

        foreachCollectGarbage( TrackId, TrackPtr, m_trackMap )
        //run before artist so that album artist pointers can be garbage collected
        foreachCollectGarbage( QString, AlbumPtr, m_albumMap )
        foreachCollectGarbage( QString, ArtistPtr, m_artistMap )
        foreachCollectGarbage( QString, GenrePtr, m_genreMap )
        foreachCollectGarbage( QString, ComposerPtr, m_composerMap )
        foreachCollectGarbage( QString, YearPtr, m_yearMap )
    }

    //make sure to unlock all necessary locks
    //important: calling unlock() on an unlocked mutex gives an undefined result
    //unlocking a mutex locked by another thread results in an error, so be careful
    if( hasTrack ) m_trackMutex.unlock();
    if( hasAlbum ) m_albumMutex.unlock();
    if( hasArtist ) m_artistMutex.unlock();
    if( hasYear ) m_yearMutex.unlock();
    if( hasGenre ) m_genreMutex.unlock();
    if( hasComposer ) m_composerMutex.unlock();
}*/



#include "ServiceSqlRegistry.moc"
