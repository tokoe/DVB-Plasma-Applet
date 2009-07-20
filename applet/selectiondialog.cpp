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

#include <QDebug>

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QStringListModel>

#include <kicon.h>
#include <klocale.h>
#include <kpassivepopup.h>

#include "request.h"

#include "selectiondialog.h"

SelectionDialog::SelectionDialog( Type type, const QString &queryPattern, QWidget *parent )
    : KDialog( parent ),
      mType( type ),
      mQueryPattern( queryPattern )
{
    setButtons( Ok | Cancel );
    setDefaultButton( None );
    showButtonSeparator( true );

    QGridLayout * layout = new QGridLayout( mainWidget() );
    layout->setMargin( marginHint() );
    layout->setSpacing( spacingHint() );

    QLabel *label = new QLabel( i18n( "Search:" ), mainWidget() );
    layout->addWidget( label, 0, 0 );

    mSearchEdit = new QLineEdit( mainWidget() );
    label->setBuddy( mSearchEdit );
    layout->addWidget( mSearchEdit, 0, 1 );

    QPushButton *searchButton = new QPushButton( mainWidget() );
    searchButton->setIcon( KIcon( "go-jump-locationbar" ) );
    layout->addWidget( searchButton, 0, 2 );

    connect( searchButton, SIGNAL( clicked() ), this, SLOT( startSearch() ) );
    connect( mSearchEdit, SIGNAL( returnPressed() ), this, SLOT( startSearch() ) );

    mResultView = new QListView( mainWidget() );
    layout->addWidget( mResultView, 1, 0, 1, 3 );
    connect( mResultView, SIGNAL( activated( const QModelIndex& ) ), this, SLOT( accept() ) );

    mModel = new QStringListModel( mResultView );
    mResultView->setModel( mModel );

    mSearchEdit->setFocus();
    searchButton->setDefault( true );
}

SelectionDialog::~SelectionDialog()
{
}

void SelectionDialog::setSelection( const QString &selection )
{
    mSearchEdit->setText( selection );
}

QString SelectionDialog::selection() const
{
    const QModelIndex currentIndex = mResultView->currentIndex();
    if ( !currentIndex.isValid() )
        return QString();

    return currentIndex.data( Qt::DisplayRole ).toString();
}

void SelectionDialog::startSearch()
{
    const QString path = mQueryPattern.arg( mSearchEdit->text() );
    Request *request = new Request( path, this );

    connect( request, SIGNAL( finished( bool ) ), this, SLOT( finished( bool ) ) );
}

void SelectionDialog::finished( bool error )
{
    Request *request = qobject_cast<Request*>( sender() );
    if ( !request )
        return;

    if ( error ) {
        KPassivePopup::message( request->errorString(), this );
        request->deleteLater();
        return;
    }

    QStringList result;

    const QVariant data = request->data();
    const QList<QVariant> topList = data.toList();
    if ( mType == Location ) {
        const QList<QVariant> firstList = topList[0].toList();
        for (int i = 0; i < firstList.count(); ++i) {
            const QString value = firstList[i].toList().first().toString();
            if ( !result.contains( value ) )
                result.append( value );
        }
    } else { // mType == Station
        const QList<QVariant> firstList = topList[1].toList();

        for (int i = 0; i < firstList.count(); ++i) {
            const QString value = firstList[i].toList().first().toString();
            if ( !result.contains( value ) )
                result.append( value );
        }
    }

    mModel->setStringList( result );
    mResultView->setCurrentIndex( mModel->index( 0, 0 ) );

    request->deleteLater();
}

#include "selectiondialog.moc"
