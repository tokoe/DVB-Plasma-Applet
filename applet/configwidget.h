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

#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QtGui/QWidget>

class QLineEdit;

class ConfigWidget : public QWidget
{
    Q_OBJECT

    public:
        ConfigWidget( QWidget *parent );
        ~ConfigWidget();

        void setLocation( const QString &location );
        QString location() const;

        void setStation( const QString &station );
        QString station() const;

    private Q_SLOTS:
        void searchLocation();
        void searchStation();

    private:
        QLineEdit *mLocation;
        QLineEdit *mStation;
};

#endif
