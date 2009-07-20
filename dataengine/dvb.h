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

#ifndef DVB_DATAENGINE_H
#define DVB_DATAENGINE_H

#include "plasma/dataengine.h"

class Request;

class DvbEngine : public Plasma::DataEngine
{
    Q_OBJECT

    public:
        DvbEngine( QObject* parent, const QVariantList& args );
        ~DvbEngine();

    protected:
        void init();
        bool sourceRequestEvent( const QString &identifier );

    protected Q_SLOTS:
        bool updateSourceEvent( const QString &identifier );

    private Q_SLOTS:
        void finished( bool );

    private:
        Request *mCurrentRequest;
};

K_EXPORT_PLASMA_DATAENGINE(dvb, DvbEngine)

#endif
