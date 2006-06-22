//
// C++ Interface: njbmediadevice
//
// Description: This class is used to manipulate Nomad Creative Jukebox and others media player that works with the njb libraries.
//
//
// Author: Andres Oton <andres.oton@gmail.com>, (C) 2006
//
// Modified by: T.R.Shashwath <trshash84@gmail.com>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NJBMEDIADEVICE_H
#define NJBMEDIADEVICE_H

// #include <mediadevice.h>
#include <mediabrowser.h>
#include <transferdialog.h>
#include <libnjb.h>

#include <qptrlist.h>
#include <qlistview.h>

#include "playlist.h"
#include "track.h"

#include <qcstring.h>
#include <qstring.h>
#include <qvaluelist.h>

// kde
#include <kurl.h>
#include <kio/global.h>
#include <kio/slavebase.h>

/**
  This class is used to manipulate Nomad Creative Jukebox and others media player that works with the njb libraries.

  You can find the njb libs at : http://libnjb.sourceforge.net

  Based at kionjb from

  @author Andres Oton <andres.oton@gmail.com>
  @author T.R.Shashwath <trshash84@gmail.com>
 */

const int NJB_SUCCESS = 0;
const int NJB_FAILURE = -1;

// Global track list
extern trackValueList* theTracks;

class NjbMediaItem;

class NjbMediaDevice : public MediaDevice
{
    Q_OBJECT

    public:
        NjbMediaDevice();

        ~NjbMediaDevice();

        virtual bool isConnected();
        virtual bool isPlayable(const MetaBundle& bundle);
        virtual bool isPreferredFormat(const MetaBundle& bundle);

        //    virtual bool needsManualConfig();

        virtual MediaItem* newPlaylist(const QString& name, MediaItem* parent, QPtrList< MediaItem > items);

        //    virtual MediaItem* tagsChanged(MediaItem* item, const MetaBundle& changed);

        virtual QStringList supportedFiletypes();

        virtual bool hasTransferDialog() { return true; }
        virtual TransferDialog* getTransferDialog();
        virtual void addConfigElements(QWidget* arg1);
        virtual void addToDirectory(MediaItem* directory, QPtrList< MediaItem > items);
        virtual void addToPlaylist(MediaItem* playlist, MediaItem* after, QPtrList< MediaItem > items);
        virtual void applyConfig();
        virtual void init(MediaBrowser* parent);
        virtual void loadConfig();
        virtual void removeConfigElements(QWidget* arg1);
        virtual void rmbPressed(QListViewItem* qitem, const QPoint& point, int arg1);
        virtual void runTransferDialog();

        void setDeviceType(const QString& type);
        void setSpacesToUnderscores(bool yesno);
        static njb_t *theNjb();
        
    protected:

        virtual bool closeDevice();
        virtual bool getCapacity(KIO::filesize_t* total, KIO::filesize_t* available);

        // virtual bool isSpecialItem(MediaItem* item);

        virtual bool lockDevice(bool tryOnly);
        virtual bool openDevice(bool silent);

        int deleteFromDevice(unsigned id);
        virtual int deleteItemFromDevice(MediaItem* item, bool onlyPlayed);
        int deleteAlbum(NjbMediaItem *albumItem);
        int deleteArtist(NjbMediaItem *artistItem);
        int deleteTrack(NjbMediaItem *trackItem);

        int downloadSelectedItems( NjbMediaItem *item );
        int downloadArtist(NjbMediaItem *artistItem, KURL destDir);
        int downloadAlbum(NjbMediaItem *albumItem, KURL destDir);
        int downloadTrack(NjbMediaItem *trackItem, KURL destDir);
        int downloadTrackNow(NjbMediaItem *item, QString path);

        virtual MediaItem* copyTrackToDevice(const MetaBundle& bundle);

        virtual void cancelTransfer();
        virtual void synchronizeDevice() {};

        virtual void unlockDevice();

        virtual void updateRootItems() {};
    private:
        // TODO:
        MediaItem        *trackExists( const MetaBundle& );
        // miscellaneous methods
        static int progressCallback( u_int64_t sent, u_int64_t total, const char* /*buf*/, unsigned /*len*/, void* data);

        int readJukeboxMusic( void);

        NjbMediaItem *getAlbum(const QString &artist, const QString &album);

        NjbMediaItem * getArtist(const QString &artist);

        NjbMediaItem *addTrackToView(NjbTrack *track, NjbMediaItem *item=0);

        void clearItems();


        TransferDialog      *m_td;
        njb_t njbs[NJB_MAX_DEVICES];

        QListView *listAmarokPlayLists;

        QString devNode;

        QString m_errMsg;

        bool m_connected; // Replaces m_captured from the original code.

        static njb_t* m_njb;
        int m_libcount;
        bool m_busy;
        trackValueList trackList;
        unsigned m_progressStart;
        QString m_progressMessage;

};

#endif

