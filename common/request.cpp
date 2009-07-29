/*
 *   Copyright (C) 2007-2009 Tobias Koenig <tokoe@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtCore/QString>
#include <QtCore/QVariant>

#include <kio/job.h>
#include <kio/jobclasses.h>
#include <klocale.h>

#include "common.h"
#include "parser.h"
#include "request.h"

class Request::Private
{
    public:
        Private( Request *parent )
            : mParent( parent )
        {
        }

        void _k_finished( KJob* );
        void _k_data( KIO::Job*, const QByteArray &data );

        Request *mParent;
        QVariant mData;
        QByteArray mRawData;
        QString mErrorString;
        KIO::TransferJob *mHttp;
};

void Request::Private::_k_finished( KJob *job )
{
    mHttp = 0;

    if ( job->error() ) {
        mErrorString = job->errorString();
        emit mParent->finished( true );
        return;
    }

    QByteArray data = mRawData;
    data.replace( "&quot;", "\"" );
    data.replace( "&#196;", QString( QChar( 196 ) ).toLocal8Bit() );
    data.replace( "&#214;", QString( QChar( 214 ) ).toLocal8Bit() );
    data.replace( "&#220;", QString( QChar( 220 ) ).toLocal8Bit() );
    data.replace( "&#223;", QString( QChar( 223 ) ).toLocal8Bit() );
    data.replace( "&#228;", QString( QChar( 228 ) ).toLocal8Bit() );
    data.replace( "&#246;", QString( QChar( 246 ) ).toLocal8Bit() );
    data.replace( "&#252;", QString( QChar( 252 ) ).toLocal8Bit() );

    const Parser parser;

    if ( !data.startsWith( '[' ) || !data.endsWith( ']' ) ) {
        mErrorString = i18n( "Retrieved invalid data from server!" );
        emit mParent->finished( true );
        return;
    }

    mData = parser.parse( QString::fromLocal8Bit( data ) );

    emit mParent->finished( false );
}

void Request::Private::_k_data( KIO::Job*, const QByteArray &data )
{
    mRawData.append( data );
}

Request::Request( const QString &path, QObject *parent )
    : QObject( parent ), d( new Private( this ) )
{
    KUrl url( QString( "http://" DVB_SERVER_URL ) + path );

    d->mHttp = KIO::get( url, KIO::NoReload, KIO::HideProgressInfo );
    connect( d->mHttp, SIGNAL( result( KJob* ) ), this, SLOT( _k_finished( KJob* ) ) );
    connect( d->mHttp, SIGNAL( data( KIO::Job*, const QByteArray& ) ), this, SLOT( _k_data( KIO::Job*, const QByteArray& ) ) );
}

Request::~Request()
{
    delete d->mHttp;
    delete d;
}

QVariant Request::data() const
{
    return d->mData;
}

QString Request::errorString() const
{
    return d->mErrorString;
}

#include "request.moc"
