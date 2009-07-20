/*
 *   Copyright (C) 2007 Tobias Koenig <tokoe@kde.org>
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
#include <QtCore/QVariant>

#include "parser.h"

Parser::Parser()
    : d( 0 )
{
}

Parser::~Parser()
{
}

/**
 * Splits '[ "Hello", "World", [ "in", "doubt" ], "outside" ]' into
 * the following tokens:
 *
 *      Hello
 *      World
 *      [ "in", "doubt" ]
 *      outside
 */
static QStringList splitString( const QString &str )
{
    QStringList retval;
    int brackets = 0;
    bool inElement = false;

    QString data( str.trimmed() );
    data = data.mid( 1 );
    data = data.left( data.size() - 1 );
    data = data.trimmed();

    int startPos = 0;
    int pos = 0;
    while ( pos < data.size() ) {
        if ( data[pos] == '"' ) {
            if ( brackets == 0 ) {
                if ( !inElement ) {
                    startPos = pos;
                    inElement = true;
                } else {
                    retval.append( data.mid( startPos + 1, pos - startPos - 1 ) );
                    inElement = false;
                }
            }
        } else if ( data[pos] == '[' ) {
            if ( brackets == 0 )
                startPos = pos;

            brackets++;
        } else if ( data[pos] == ']' ) {
            brackets--;

            if ( brackets == 0 )
                retval.append( data.mid( startPos, pos - startPos + 1 ) );
        }

        pos++;
    }

    return retval;
}

static QVariant parseInternal( const QString &str )
{
    QString data( str.trimmed() );

    if ( data.startsWith( "[" ) && data.endsWith( "]" ) ) { // is list
        QList<QVariant> retval;

        QStringList list = splitString( data );
        for ( int i = 0; i < list.count(); ++i ) {
            retval.append( parseInternal( list[ i ] ) );
        }

        return retval;
    } else {
        return QVariant( str );
    }
}

QVariant Parser::parse( const QString &data ) const
{
    return parseInternal( data );
}
