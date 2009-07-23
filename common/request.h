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

#ifndef REQUEST_H
#define REQUEST_H

#include <QtCore/QObject>

class QString;
class QVariant;

class Request : public QObject
{
    Q_OBJECT

    public:
        Request( const QString &path, QObject *parent );
        ~Request();

        QVariant data() const;

        QString errorString() const;

    Q_SIGNALS:
        void finished( bool error );

    private:
        class Private;
        Private* const d;

        Q_PRIVATE_SLOT( d, void _k_finished( KJob* ) )
        Q_PRIVATE_SLOT( d, void _k_data( KIO::Job*, const QByteArray& ) )
};

#endif
