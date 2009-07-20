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

#ifndef DVB_H
#define DVB_H

#include <QtGui/QImage>

#include <plasma/applet.h>
#include <plasma/dataengine.h>

#include "common.h"

class ConfigWidget;

class DvbApplet : public Plasma::Applet
{
    Q_OBJECT

    public:
        DvbApplet( QObject *parent, const QVariantList &args );
        ~DvbApplet();

        virtual void init();
        virtual void paintInterface( QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect );

    public Q_SLOTS:
        void dataUpdated( const QString &name, const Plasma::DataEngine::Data &data );
        void createConfigurationInterface( KConfigDialog *parent );

    private Q_SLOTS:
        void applyConfig();
        void updateInfo();

    protected:
        void mousePressEvent( QGraphicsSceneMouseEvent* );

    private:
        void loadConfig();
        void saveConfig();

        QImage mBackground;
        QVariant mInfo;
        Identifier mIdentifier;
        ConfigWidget *mConfigWidget;
};

K_EXPORT_PLASMA_APPLET(dvb, DvbApplet)

#endif
