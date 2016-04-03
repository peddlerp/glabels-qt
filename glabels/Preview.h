/*  Preview.h
 *
 *  Copyright (C) 2013-2016  Jim Evins <evins@snaught.com>
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

#ifndef Preview_h
#define Preview_h

#include <QGraphicsView>
#include <QGraphicsScene>

#include "PageRenderer.h"


class LabelModel; // Forward reference


///
///  Preview Widget
///
class Preview : public QGraphicsView
{
	Q_OBJECT


	/////////////////////////////////
	// Life Cycle
	/////////////////////////////////
public:
	Preview( QWidget *parent = 0 );


	/////////////////////////////////
	// Properties
	/////////////////////////////////
public:
	void setModel( const LabelModel* model );
	void setRenderer( const PageRenderer* renderer );


	/////////////////////////////////////
	// Event handlers
	/////////////////////////////////////
protected:
	void resizeEvent( QResizeEvent* event );

		
	/////////////////////////////////
	// Internal Methods
	/////////////////////////////////
private:
	void clearScene();
	void drawPaper( const glabels::Distance& pw, const glabels::Distance& ph );
	void drawLabels();
	void drawLabel( const glabels::Distance& x,
	                const glabels::Distance& y,
	                const QPainterPath&      path );
		
	void drawPreviewOverlay();


	/////////////////////////////////
	// Private Data
	/////////////////////////////////
private:
	const LabelModel*   mModel;
	const PageRenderer* mRenderer;    
	QGraphicsScene*     mScene;

};


#endif // Preview_h