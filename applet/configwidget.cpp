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

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>

#include <klocale.h>

#include "selectiondialog.h"

#include "configwidget.h"

ConfigWidget::ConfigWidget( QWidget *parent )
    : QWidget( parent )
{
    QGridLayout *layout = new QGridLayout( this );
    mLocation = new QLineEdit( this );
    mLocation->setReadOnly( true );
    mStation = new QLineEdit( this );
    mStation->setReadOnly( true );

    QLabel *label = new QLabel( i18n( "Location:" ), this );
    label->setBuddy( mLocation );
    layout->addWidget( label, 0, 0 );
    layout->addWidget( mLocation, 0, 1 );

    label = new QLabel( i18n( "Station:" ), this );
    label->setBuddy( mStation );
    layout->addWidget( label, 1, 0 );
    layout->addWidget( mStation, 1, 1 );

    QToolButton *button = new QToolButton( this );
    button->setText( "..." );
    layout->addWidget( button, 0, 2 );
    connect( button, SIGNAL( clicked() ), this, SLOT( searchLocation() ) );

    button = new QToolButton( this );
    button->setText( "..." );
    layout->addWidget( button, 1, 2 );
    connect( button, SIGNAL( clicked() ), this, SLOT( searchStation() ) );
}

ConfigWidget::~ConfigWidget()
{
}

void ConfigWidget::setLocation( const QString &location )
{
    mLocation->setText( location );
}

QString ConfigWidget::location() const
{
    return mLocation->text();
}

void ConfigWidget::setStation( const QString &station )
{
    mStation->setText( station );
}

QString ConfigWidget::station() const
{
    return mStation->text();
}

void ConfigWidget::searchLocation()
{
    SelectionDialog dlg( SelectionDialog::Location, "/abfahrtsmonitor/Haltestelle.do?ort=%1", this );
    dlg.setCaption( i18n( "Location Selection" ) );
    dlg.setSelection( mLocation->text() );
    if ( dlg.exec() ) {
        const QString location = dlg.selection();
        if ( !location.isEmpty() )
            mLocation->setText( location );
    }
}

void ConfigWidget::searchStation()
{
    QString encodedLocation( QUrl::toPercentEncoding( mLocation->text() ) );
    QString path = QString( "/abfahrtsmonitor/Haltestelle.do?ort=%1" ).arg( encodedLocation );
    SelectionDialog dlg( SelectionDialog::Station, path + "&hst=%1", this );
    dlg.setCaption( i18n( "Station Selection" ) );
    dlg.setSelection( mStation->text() );
    if ( dlg.exec() ) {
        const QString station = dlg.selection();
        if ( !station.isEmpty() )
            mStation->setText( station );
    }
}

#include "configwidget.moc"
