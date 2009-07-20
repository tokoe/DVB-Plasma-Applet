/*
 *   Copyright (C) 2007 Tobias Koenig <tokoe@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
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

#include <QtCore/QUrl>

#include "common.h"
#include "dvb.h"
#include "request.h"

DvbEngine::DvbEngine( QObject* parent, const QVariantList &args )
    : Plasma::DataEngine( parent, args ),
      mCurrentRequest( 0 )
{
    setMinimumPollingInterval( 0 );
    setPollingInterval( 30000 );
}

DvbEngine::~DvbEngine()
{
    if ( mCurrentRequest )
        delete mCurrentRequest;
}

void DvbEngine::init()
{
}

bool DvbEngine::updateSourceEvent( const QString &identifier )
{
    const Identifier id( identifier );
    const QString quotedLocation( QUrl::toPercentEncoding( id.location() ) );
    const QString quotedStation( QUrl::toPercentEncoding( id.station() ) );
    const QString path = QString( "/abfahrtsmonitor/Abfahrten.do?ort=%1&hst=%2" ).arg( quotedLocation )
                                                                                 .arg( quotedStation );
    mCurrentRequest = new Request( path, this );
    mCurrentRequest->setProperty( "identifier", identifier );

    connect( mCurrentRequest, SIGNAL( finished( bool ) ), this, SLOT( finished( bool ) ) );

    return false;
}

bool DvbEngine::sourceRequestEvent( const QString &identifier )
{
    setData( identifier, DataEngine::Data() );

    updateSourceEvent( identifier );

    return true;
}

void DvbEngine::finished( bool error )
{
    Request *request = qobject_cast<Request*>( sender() );
    if ( !request )
        return;

    const QString identifier = request->property( "identifier" ).toString();

    if ( error ) {
        setData( identifier, DataEngine::Data() );
    } else {
        setData( identifier, request->data() );
    }

    mCurrentRequest = 0;
    request->deleteLater();
}

#include "dvb.moc"
