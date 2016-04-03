/*  ObjectEditor.cpp
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

#include "ObjectEditor.h"


#include "LabelModel.h"
#include "LabelModelObject.h"
#include "LabelModelBoxObject.h"

#include <cmath>
#include <QtDebug>


///
/// Constructor
///
ObjectEditor::ObjectEditor( QWidget *parent )
	: mModel(0), mObject(0), mBlocked(false)
{
	setupUi( this );

	lineColorButton->init( "No line", QColor(0,0,0,0), QColor(0,0,0,255) );
	fillColorButton->init( "No fill", QColor(0,0,0,0), QColor(0,0,0,255) );
	shadowColorButton->init( "Default", QColor(0,0,0,255), QColor(0,0,0,255) );

	setEnabled( false );
	hidePages();
}

	
void ObjectEditor::setModel( LabelModel* model )
{
	mModel = model;

	connect( mModel, SIGNAL(sizeChanged()), this, SLOT(onLabelSizeChanged()) );
	connect( mModel, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()) );

	onLabelSizeChanged();
	onSelectionChanged();
}

	
void ObjectEditor::hidePages()
{
	notebook->removeTab( notebook->indexOf(textPage) );
	notebook->removeTab( notebook->indexOf(barcodePage) );
	notebook->removeTab( notebook->indexOf(imagePage) );
	notebook->removeTab( notebook->indexOf(lineFillPage) );
	notebook->removeTab( notebook->indexOf(posSizePage) );
	notebook->removeTab( notebook->indexOf(shadowPage) );
}


void ObjectEditor::loadLineFillPage()
{
	if ( mObject )
	{
		mBlocked = true;
			
		lineWidthSpin->setValue( mObject->lineWidth().pt() );
		lineColorButton->setColorNode( mObject->lineColorNode() );
		fillColorButton->setColorNode( mObject->fillColorNode() );

		mBlocked = false;			
	}
}


void ObjectEditor::loadPositionPage()
{
	if ( mObject )
	{
		mBlocked = true;
			
		posXSpin->setValue( mObject->x0().in() );
		posYSpin->setValue( mObject->y0().in() );

		mBlocked = false;			
	}
}


void ObjectEditor::loadRectSizePage()
{
	if ( mObject )
	{
		mBlocked = true;
			
		sizeWSpin->setValue( mObject->w().in() );
		sizeHSpin->setValue( mObject->h().in() );

		mBlocked = false;			
	}
}


void ObjectEditor::loadShadowPage()
{
	if ( mObject )
	{
		mBlocked = true;
			
		shadowEnableCheck->setChecked( mObject->shadow() );
		shadowXSpin->setValue( mObject->shadowX().in() );
		shadowYSpin->setValue( mObject->shadowY().in() );
		shadowColorButton->setColorNode( mObject->shadowColorNode() );
		shadowOpacitySpin->setValue( 100*mObject->shadowOpacity() );

		mBlocked = false;			
	}
}


void ObjectEditor::onLabelSizeChanged()
{
	if ( mModel )
	{
		mBlocked = true;

		glabels::Distance whMax = std::max( mModel->w(), mModel->h() );
			
		posXSpin->setRange( -whMax.in(), 2*whMax.in() );
		posYSpin->setRange( -whMax.in(), 2*whMax.in() );
		sizeWSpin->setRange( 0, 2*whMax.in() );
		sizeHSpin->setRange( 0, 2*whMax.in() );
			
		mBlocked = false;			
	}
}
	

void ObjectEditor::onSelectionChanged()
{
	if ( mObject )
	{
		disconnect( mObject, 0, this, 0 );
	}

	hidePages();

	if ( mModel->isSelectionAtomic() )
	{
		mObject = mModel->getFirstSelectedObject();

		if ( dynamic_cast<LabelModelBoxObject*>(mObject) )
		{
			titleImageLabel->setPixmap( QPixmap(":icons/24x24/actions/glabels-box.png") );
			titleLabel->setText( "Box object properties" );

			notebook->addTab( lineFillPage, "line/fill" );
			notebook->addTab( posSizePage, "position/size" );
			notebook->addTab( shadowPage, "shadow" );

			sizeRectFrame->setVisible( true );
			sizeResetImageButton->setVisible( false );
			sizeLineFrame->setVisible( false );

			loadLineFillPage();
			loadPositionPage();
			loadRectSizePage();
			loadShadowPage();
				
			setEnabled( true );
		}
		else
		{
			Q_ASSERT_X( false, "ObjectEditor::onSelectionChanged", "Invalid object" );
		}

		connect( mObject, SIGNAL(changed()), this, SLOT(onObjectChanged()) );
		connect( mObject, SIGNAL(moved()), this, SLOT(onObjectMoved()) );
		connect( mObject, SIGNAL(destroyed(QObject*)), this, SLOT(onObjectDestroyed()) );
	}
	else
	{
		mObject = 0;

		titleImageLabel->setPixmap( QPixmap(":icons/24x24/actions/glabels-object-properties.png") );
		titleLabel->setText( "Object properties" );
		setEnabled( false );
	}
}

	
void ObjectEditor::onObjectChanged()
{
	if ( !mBlocked )
	{
		loadLineFillPage();
		loadRectSizePage();
		loadShadowPage();
	}
}

	
void ObjectEditor::onObjectMoved()
{
	if ( !mBlocked )
	{
		loadPositionPage();
	}
}


void ObjectEditor::onObjectDestroyed()
{
	disconnect( mObject, 0, this, 0 );
	mObject = 0;
}
	
	
void ObjectEditor::onLineControlsChanged()
{
	if ( !mBlocked )
	{
		mBlocked = true;

		mObject->setLineWidth( glabels::Distance::pt(lineWidthSpin->value()) );
		mObject->setLineColorNode( lineColorButton->colorNode() );

		mBlocked = false;
	}
}

	
void ObjectEditor::onFillControlsChanged()
{
	if ( !mBlocked )
	{
		mBlocked = true;

		mObject->setFillColorNode( fillColorButton->colorNode() );

		mBlocked = false;
	}
}


void ObjectEditor::onPositionControlsChanged()
{
	if ( !mBlocked )
	{
		mBlocked = true;

		mObject->setPosition( posXSpin->value(), posYSpin->value() );

		mBlocked = false;
	}
}


void ObjectEditor::onRectSizeControlsChanged()
{
	if ( !mBlocked )
	{
		mBlocked = true;
			
		glabels::Distance spinW = glabels::Distance::in(sizeWSpin->value());
		glabels::Distance spinH = glabels::Distance::in(sizeHSpin->value());
				
		if ( sizeAspectCheck->isChecked() )
		{
			if ( fabs(spinW - mObject->w()) > fabs(spinH - mObject->h()) )
			{
				mObject->setWHonorAspect( spinW );
				sizeHSpin->setValue( mObject->h().in() );
			}
			else
			{
				mObject->setHHonorAspect( spinH );
				sizeWSpin->setValue( mObject->w().in() );
			}
		}
		else
		{
			mObject->setSize( spinW, spinH );
		}
			
		mBlocked = false;
	}
}


void ObjectEditor::onShadowControlsChanged()
{
	if ( !mBlocked )
	{
		mBlocked = true;

		mObject->setShadow( shadowEnableCheck->isChecked() );
		mObject->setShadowX( glabels::Distance::in(shadowXSpin->value()) );
		mObject->setShadowY( glabels::Distance::in(shadowYSpin->value()) );
		mObject->setShadowColorNode( shadowColorButton->colorNode() );
		mObject->setShadowOpacity( shadowOpacitySpin->value()/100.0 );

		mBlocked = false;
	}
}


void ObjectEditor::onChanged()
{
	if ( !mBlocked )
	{
		mBlocked = true;

		qDebug() << "Form changed.";

		mBlocked = false;
	}
}