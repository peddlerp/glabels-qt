/*  Merge/Factory.cpp
 *
 *  Copyright (C) 2016  Jim Evins <evins@snaught.com>
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

#include "Factory.h"

#include "None.h"
#include "TextCsv.h"


namespace merge
{

	///
	/// Static data
	///
	QMap<QString,Factory::BackendEntry> Factory::mBackendIdMap;
	QMap<QString,Factory::BackendEntry> Factory::mBackendNameMap;


	///
	/// Constructor
	///
	Factory::Factory()
	{
		registerBackend( None::id(),
				 tr("None"),
				 NONE,
				 &None::create );
	
		registerBackend( TextCsv::id(),
				 tr("Text: Comma Separated Values (CSV)"),
				 FILE,
				 &TextCsv::create );
	}


	///
	/// Initialize
	///
	void Factory::init()
	{
		static Factory* singletonInstance = 0;
		if ( !singletonInstance )
		{
			singletonInstance = new Factory();
		}
	}


	///
	/// Create Merge object
	///
	Merge* Factory::createMerge( const QString& id )
	{
		QMap<QString,BackendEntry>::iterator iBackend = mBackendIdMap.find( id );
		if ( iBackend != mBackendIdMap.end() )
		{
			return iBackend->create();
		}
	
		return None::create();
	}


	///
	/// Get name list
	///
	QStringList Factory::nameList()
	{
		QStringList list;
	
		foreach ( BackendEntry backend, mBackendIdMap )
		{
			list << backend.name;
		}

		return list;
	}


	///
	/// Convert ID to name
	///
	QString Factory::idToName( const QString& id )
	{
		if ( mBackendIdMap.contains( id ) )
		{
			return mBackendIdMap[id].name;
		}
		else
		{
			return tr("None");
		}
	}


	///
	/// Convert name to ID
	///
	QString Factory::nameToId( const QString& name )
	{
		if ( mBackendNameMap.contains( name ) )
		{
			return mBackendNameMap[name].id;
		}
		else
		{
			return "None";
		}
	}


	///
	/// Convert ID to type
	///
	Factory::SourceType Factory::idToType( const QString& id )
	{
		if ( mBackendIdMap.contains( id ) )
		{
			return mBackendIdMap[id].type;
		}
		else
		{
			return NONE;
		}
	}


	///
	/// Lookup ID from index
	///
	QString Factory::indexToId( int index )
	{
		QList<QString> ids = mBackendIdMap.keys();

		if ( (index > 0) && (index < ids.size()) )
		{
			return ids[index];
		}

		return "None";
	}


	///
	/// Register backend
	///
	void Factory::registerBackend( const QString& id,
				       const QString& name,
				       SourceType     type,
				       CreateFct      create )
	{
		BackendEntry backend;

		backend.name   = name;
		backend.type   = type;
		backend.create = create;
	
		mBackendIdMap[ id ] = backend;
	}

}

