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

#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>

#include <kconfigdialog.h>

#include "configwidget.h"
#include "dvb.h"

DvbApplet::DvbApplet( QObject *parent, const QVariantList &args )
    : Plasma::Applet( parent, args ),
      mIdentifier( "Dresden", "Bernhardstrasse" ),
      mConfigWidget( 0 )
{
    setHasConfigurationInterface( true );

    loadConfig();

    resize( QSizeF( 341, 213 ) );

    mBackground = QImage( ":background" );

    Plasma::DataEngine *engine = dataEngine( "dvb" );
    engine->connectSource( mIdentifier.identifier(), this );
}

DvbApplet::~DvbApplet()
{
}

void DvbApplet::init()
{
    updateInfo();
}

void DvbApplet::dataUpdated( const QString&, const Plasma::DataEngine::Data &data )
{
    mInfo = data[ mIdentifier.identifier() ];
    update();
}

void DvbApplet::createConfigurationInterface( KConfigDialog *parent )
{
    mConfigWidget = new ConfigWidget( parent );
    mConfigWidget->setLocation( mIdentifier.location() );
    mConfigWidget->setStation( mIdentifier.station() );

    parent->setButtons( KDialog::Ok | KDialog::Cancel );
    parent->addPage( mConfigWidget, parent->windowTitle(), icon() );

    connect( parent, SIGNAL( applyClicked() ), this, SLOT( applyConfig() ) );
    connect( parent, SIGNAL( okClicked() ), this, SLOT( applyConfig() ) );
}

void DvbApplet::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton ) {
        if ( event->pos().x() > 5 && event->pos().x() < 21 &&
             event->pos().y() > 193 && event->pos().y() < 209 ) {
            updateInfo();
        }
    }

    event->ignore();
}

void DvbApplet::paintInterface( QPainter *p, const QStyleOptionGraphicsItem*, const QRect& )
{
    p->save();
    p->setRenderHint( QPainter::Antialiasing );
    p->drawImage( QRectF( 0, 0, 341, 213 ), mBackground );

    QFont font = p->font();
    font.setPointSize(12);
    font.setBold(true);
    p->setFont(font);
    p->setPen( Qt::black );

    p->drawText( QRectF( 35, 0, 300, 25 ), Qt::AlignTop | Qt::AlignLeft, mIdentifier.station() );

    font = p->font();
    font.setPointSize(9);
    font.setBold(true);
    p->setFont(font);

    int height = 50;
    p->setPen( QColor( "#d47c2e" ) );
    const QList<QVariant> list = mInfo.toList();

    if ( list.isEmpty() || (list.count() == 1 && list[0].type() == QVariant::String && list[0].toString().isEmpty()) ) {
        p->drawText( QRectF( 0, height, mBackground.width(), 100 ), Qt::AlignVCenter | Qt::AlignCenter, i18n( "No Information available" ) );
    } else {
        for (int i = 0; i < list.count() && i < 10; ++i ) {
            p->drawText( QRectF( 10, height, 30, 19 ), Qt::AlignTop | Qt::AlignRight, list[i].toList()[0].toString() );
            p->drawText( QRectF( 52, height, 238, 19 ), Qt::AlignTop | Qt::AlignLeft, list[i].toList()[1].toString() );
            p->drawText( QRectF( 300, height, 25, 19 ), Qt::AlignTop | Qt::AlignRight, list[i].toList()[2].toString() );
            height += 13;
        }
    }

    p->restore();
}

void DvbApplet::applyConfig()
{
    dataEngine( "dvb" )->disconnectSource( mIdentifier.identifier(), this );

    mIdentifier = Identifier( mConfigWidget->location(),
                              mConfigWidget->station() );

    saveConfig();

    dataEngine( "dvb" )->connectSource( mIdentifier.identifier(), this );

    updateInfo();
}

void DvbApplet::loadConfig()
{
    KConfigGroup cg = config();
    mIdentifier = Identifier( cg.readEntry( "location", "Dresden" ),
                              cg.readEntry( "station", "Bernhardstrasse" ) );
}

void DvbApplet::saveConfig()
{
    KConfigGroup cg = config();
    cg.writeEntry( "location", mIdentifier.location() );
    cg.writeEntry( "station", mIdentifier.station() );
}

void DvbApplet::updateInfo()
{
    Plasma::DataEngine *engine = dataEngine( "dvb" );

    const Plasma::DataEngine::Data data = engine->query( mIdentifier.identifier() );
    if ( data.contains( mIdentifier.identifier() ) ) {
        mInfo = data[ mIdentifier.identifier() ];
        update();
    }
}

#include "dvb.moc"
