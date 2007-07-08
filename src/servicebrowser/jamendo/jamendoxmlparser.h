/***************************************************************************
 *   Copyright (c) 2007  Nikolaj Hald Nielsen <nhnFreespirit@gmail.com>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/

#ifndef JAMENDOXMLPARSER_H
#define JAMENDOXMLPARSER_H

#include "jamendodatabasehandler.h"
//#include "jamendotypes.h"
#include "threadmanager.h"

#include <QDomElement>
#include <QMap>
#include <QString>
#include <QStringList>

/**
* Parser for the XML file from http://img.jamendo.com/data/dbdump.en.xml.gz
*
* @author Nikolaj Hald Nielsen
*/
class JamendoXmlParser : public ThreadManager::Job
{
    Q_OBJECT

public:

    /**
     * Constructor
     * @param fileName The file to parse 
     * @return Pointer to new object
     */
    JamendoXmlParser( const QString &fileName );

    /**
     * The function that starts the actual work. Inherited fromThreadManager::Job 
     * Note the work is performed in a separate thread
     * @return Returns true on success and false on failure
     */
    bool doJob();

    /**
     * Called when the job has completed. Is executed in the GUI thread
     */
    void completeJob();

    /**
     * Destructor
     * @return none
     */
    ~JamendoXmlParser();

    /**
     * Reads, and starts parsing, file. Should not be used directly.
     * @param filename The file to read
     */
    void readConfigFile( const QString &filename );


signals:

    /**
     * Signal emmited when parsing is complete.
     */
    void doneParsing();

private:

    JamendoDatabaseHandler * m_dbHandler;

    QString m_sFileName;

    QMap<int, QStringList> albumTags; //used for applying genres to individual tracks

    int m_nNumberOfTracks;
    int m_nNumberOfAlbums;
    int m_nNumberOfArtists;

    /**
     * Parses a DOM element
     * @param e The element to parse
     */
    void parseElement( QDomElement e );

    /**
     * Parses all children of a DOM element
     * @param e The element whose children is to be parsed
     */
    void parseChildren( QDomElement e );

    /**
     * Parse a DOM element representing an artist
     * @param e The artist element to parse
     */
    void parseArtist( QDomElement e );

    /**
     * Parse a DOM element representing an album
     * @param e The album element to parse
     */
    void parseAlbum( QDomElement e );

    /**
     * Parse a DOM element representing a track
     * @param e The track element to parse
     */
    void parseTrack( QDomElement e );


    
    /**
     * Parse a jamendo <Covers> tag and get the cover of a specific size
     * if possible, otherwise return an empty string
     * @param e the node to parse
     * @param size the desired size of the cover
     */
    QString getCoverUrl( QDomElement e, int size );

    void countTransaction();

    int n_numberOfTransactions;
    int n_maxNumberOfTransactions;
    QMap<int, int> m_albumArtistMap;
};

#endif
