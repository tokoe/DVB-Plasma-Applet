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

#include <QtCore/QStringList>

#include "common.h"

Identifier::Identifier( const QString &location, const QString &station )
    : mLocation( location ), mStation( station )
{
}

Identifier::Identifier( const QString &identifier )
{
    const QStringList parts = identifier.split( ':', QString::KeepEmptyParts );
    mLocation = parts[ 0 ];
    mStation = parts[ 1 ];
}

QString Identifier::identifier() const
{
    return QString( "%1:%2" ).arg( mLocation ).arg( mStation );
}

QString Identifier::location() const
{
    return mLocation;
}

QString Identifier::station() const
{
    return mStation;
}
