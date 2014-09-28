/*  FieldButton.h
 *
 *  Copyright (C) 2014  Jim Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef glabels_FieldButton_h
#define glabels_FieldButton_h

#include <QPushButton>
#include <QString>
#include "FieldMenu.h"


namespace glabels
{

	///
	/// Field Button
	///
	class FieldButton : public QPushButton
	{
		Q_OBJECT

		/////////////////////////////////
		// Life Cycle
		/////////////////////////////////
	public:
		FieldButton( const QString& name, QWidget* parent = 0 );


		/////////////////////////////////
		// Signals
		/////////////////////////////////
	signals:
		void keySelected( const QString& key );


		/////////////////////////////////
		// Properties
		/////////////////////////////////
	public:
		QString key() const;


		/////////////////////////////////
		// Public Methods
		/////////////////////////////////
	public:
		void setKeys( const QList<QString>& keyList );
		void clearKeys();


		/////////////////////////////////
		// Slots
		/////////////////////////////////
	private slots:
		void onMenuKeySelected( const QString& key );


		/////////////////////////////////
		// Private Data
		/////////////////////////////////
	private:
		FieldMenu*  mMenu;
		QString     mKey;
		bool        mLabelIsKey;

	};


}

#endif // glabels_FieldButton_h