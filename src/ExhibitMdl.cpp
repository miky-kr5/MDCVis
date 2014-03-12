/*------------------------------------------------------------------------------
; File:          ExhibitMdl.cpp
; Description:   Implementation of the exhibit model class.
; Author:        Miguel Angel Astor, sonofgrendel@gmail.com
; Date created:  2/19/2014
;
; Copyright (C) 2014 Museo de Ciencias de Caracas
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;-----------------------------------------------------------------------------*/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <sqlite/sqlite3.h>

#include "ExhibitMdl.hpp"

using std::cout;
using std::cerr;
using std::endl;

void swap( int &, int & );

mdcExhibitMdl * mdcExhibitMdl::instance = NULL;

mdcExhibitMdl * mdcExhibitMdl::getInstance() {
  if ( instance == NULL ) {
    instance = new mdcExhibitMdl();
  }
  instance->refs++;
  return instance;
}

void mdcExhibitMdl::freeInstance() {
  if ( instance != NULL ) {
    instance->refs--;
    if ( instance->refs == 0 ) {
      instance->releaseDatabase();

      delete instance;
    }
  }
}

mdcExhibitMdl::mdcExhibitMdl(): db(NULL), refs(0) { }

mdcExhibitMdl::~mdcExhibitMdl() { }

bool mdcExhibitMdl::setDatabaseFile( const char * file ){
  int rc;

  rc = sqlite3_open_v2( file, &db, SQLITE_OPEN_READONLY, NULL );

  if ( rc != SQLITE_OK ) {
    cerr << "Failed to open database file: " << sqlite3_errmsg( db ) << endl;
    sqlite3_close(db);
    db = NULL;
    dbUsable = false;
  }else{
    dbUsable = true;
  }

  return dbUsable;
}

bool mdcExhibitMdl::releaseDatabase() {
  if ( dbUsable ) {
    sqlite3_close(db);
    db = NULL;
    dbUsable = false;
    return true;
  }

  return false;
}

// Different methods to get row ids.
int mdcExhibitMdl::getNumOfExhibits() {
  const char   *   query = "SELECT id FROM exhibits";
  sqlite3_stmt *   ppStmt;
  int              rc;
  int              i = 0;

  if ( dbUsable ) {
    rc = sqlite3_prepare_v2( db, query, 24, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getNumOfExhibits() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      return BAD_VALUE;
    }

    while( ( rc = sqlite3_step( ppStmt ) ) == SQLITE_ROW ){
      i++;
    }

    if ( rc != SQLITE_DONE ) {
      cerr << "mdcExhibitMdl::getNumOfExhibits() - Error processing query: " << sqlite3_errmsg( db ) << endl;
    }

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getNumOfExhibits() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
      return BAD_VALUE;
    }

    return i;

  }else{
    cerr << "mdcExhibitMdl::getNumOfExhibits() - Database is unusable." << endl;
  }

  return BAD_VALUE;
}

int mdcExhibitMdl::getFirstNExhibitIds( int * exhibits, int n ) {
  const char   *   query = "SELECT id FROM exhibits";
  sqlite3_stmt *   ppStmt;
  int              rc;
  int              i = 0;

  if ( dbUsable ) {
    if ( n > 0 ) {
      rc = sqlite3_prepare_v2( db, query, 24, &ppStmt, NULL);

      if ( rc != SQLITE_OK ) {
	cerr << "mdcExhibitMdl::getFirstNExhibitIds() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
	return BAD_VALUE;
      }

      while( ( rc = sqlite3_step( ppStmt ) ) == SQLITE_ROW && i < n ){
	exhibits[i] = sqlite3_column_int( ppStmt, 0 );
	i++;
      }

      if ( i != n && rc != SQLITE_DONE ) {
	cerr << "mdcExhibitMdl::getFirstNExhibitIds() - Error processing query: " << sqlite3_errmsg( db ) << endl;
      }

      rc = sqlite3_finalize( ppStmt );

      if ( rc != SQLITE_OK ) {
	cerr << "mdcExhibitMdl::getFirstNExhibitIds() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
	return BAD_VALUE;
      }

      return i;

    } else {
      cerr << "mdcExhibitMdl::getFirstExhibitIds() - Received a negative number or zero as parameter." << endl;
      return BAD_VALUE;
    }
  } else {
    cerr << "mdcExhibitMdl::getFirstExhibitIds() - Database is unusable." << endl;
    return BAD_VALUE;
  }
}

int mdcExhibitMdl::getExhibitsIdsRange( int * exhibits, int n1, int n2 ) {
  const char   *   query = "SELECT id FROM exhibits";
  sqlite3_stmt *   ppStmt;
  int              rc;
  int              i = 0;
  int              j = 0;
  int              a = n1;
  int              b = n2;

  if ( dbUsable ) {
    if ( n1 <= 0 || n2 <= 0 ){
      cerr << "mdcExhibitMdl::getExhibitsIdsRange() - Received a negative number or zero as parameter." << endl;
      return BAD_VALUE;
    }

    if ( a > b ) {
      swap(a, b);
    }

    rc = sqlite3_prepare_v2( db, query, 24, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitsIdsRange() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      return BAD_VALUE;
    }

    while( ( rc = sqlite3_step( ppStmt ) ) == SQLITE_ROW && i <= b ){
      if ( i >= a ) {
	exhibits[j++] = sqlite3_column_int( ppStmt, 0 );
      }
      i++;
    }

    if ( i != b + 1 && rc != SQLITE_DONE ) {
      cerr << "mdcExhibitMdl::getExhibitsIdsRange() - Error processing query: " << sqlite3_errmsg( db ) << endl;
    }

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitsIdsRange() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
      return BAD_VALUE;
    }

    return j;

  } else {
    cerr << "mdcExhibitMdl::getExhibitsIdsRange() - Database is unusable." << endl;
    return BAD_VALUE;
  }
}

// Text getters.
void mdcExhibitMdl::getExhibitTitleById( char ** title, int id ) {
  const char   *   query = "SELECT title FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;

  if ( dbUsable ) {

    rc = sqlite3_prepare_v2( db, query, 40, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitTitleById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *title = NULL;
      return;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitTitleById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *title = NULL;
      return;
    }

    rc = sqlite3_step( ppStmt );

    if ( rc != SQLITE_ROW && rc != SQLITE_DONE ) {
    	cerr << "mdcExhibitMdl::getExhibitTitleById() - Failed to get an exhibit from the database: " << sqlite3_errmsg( db ) << endl;
    	*title = NULL;
    	return;
    }

    *title = ( char * )malloc( sizeof( char ) * strlen( reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) ) + 1 );
    strcpy( *title, reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitTitleById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
    }

  } else {
    cerr << "mdcExhibitMdl::getExhibitTitleById() - Database is unusable." << endl;
  }
}

void mdcExhibitMdl::getExhibitDescriptionById( char ** desc, int id ) {
  const char   *   query = "SELECT description FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;

  if ( dbUsable ) {

    rc = sqlite3_prepare_v2( db, query, 46, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitDescriptionById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *desc = NULL;
      return;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitDescriptionById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *desc = NULL;
      return;
    }

    rc = sqlite3_step( ppStmt );

    if ( rc != SQLITE_ROW && rc != SQLITE_DONE ) {
    	cerr << "mdcExhibitMdl::getExhibitDescriptionById() - Failed to get an exhibit from the database: " << sqlite3_errmsg( db ) << endl;
    	*desc = NULL;
    	return;
    }

    *desc = ( char * )malloc( sizeof( char ) * strlen( reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) ) + 1 );
    strcpy( *desc, reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitDescriptionById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
    }

  } else {
    cerr << "mdcExhibitMdl::getExhibitDescriptionById() - Database is unusable." << endl;
  }
}

void mdcExhibitMdl::getExhibitModelPathById( char ** path, int id ) {
  const char   *   query = "SELECT model_path FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;

  if ( dbUsable ) {

    rc = sqlite3_prepare_v2( db, query, 45, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitModelPathById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *path = NULL;
      return;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitModelPathById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *path = NULL;
      return;
    }

    rc = sqlite3_step( ppStmt );

    if ( rc != SQLITE_ROW && rc != SQLITE_DONE ) {

    	cerr << rc << " mdcExhibitMdl::getExhibitModelPathById() - Failed to get an exhibit from the database: " << sqlite3_errmsg( db ) << endl;
    	*path = NULL;
    	return;
    }

    *path = ( char * )malloc( sizeof( char ) * strlen( reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) ) + 1 );
    strcpy( *path, reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitModelPathById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
    }

  } else {
    cerr << "mdcExhibitMdl::getExhibitModelPathById() - Database is unusable." << endl;
  }
}

void mdcExhibitMdl::getExhibitPhotoPathById( char ** path, int id ) {
  const char   *   query = "SELECT photo_path FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;

  if ( dbUsable ) {

    rc = sqlite3_prepare_v2( db, query, 45, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitPhotoPathById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *path = NULL;
      return;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitPhotoPathById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      *path = NULL;
      return;
    }

    rc = sqlite3_step( ppStmt );

    if ( rc != SQLITE_ROW && rc != SQLITE_DONE ) {
    	cerr << "mdcExhibitMdl::getExhibitPhotoPathById() - Failed to get an exhibit from the database: " << sqlite3_errmsg( db ) << endl;
    	*path = NULL;
    	return;
    }

    *path = ( char * )malloc( sizeof( char ) * strlen( reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) ) + 1 );
    strcpy( *path, reinterpret_cast< const char * >( sqlite3_column_text( ppStmt, 0 ) ) );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getExhibitPhotoPathById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
    }

  } else {
    cerr << "mdcExhibitMdl::getExhibitPhotoPathById() - Database is unusable." << endl;
  }
}

// Geometric transformation getters.
void mdcExhibitMdl::getTranslationById( vec3_t & trans, int id ) {
  const char   *   query = "SELECT translation_x, translation_y, translation_z FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;

  if ( dbUsable ) {
    rc = sqlite3_prepare_v2( db, query, 78, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getTranslationById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      trans.x = BAD_VALUE;
      trans.y = BAD_VALUE;
      trans.z = BAD_VALUE;
      sqlite3_finalize( ppStmt );
      return;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getTranslationById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      trans.x = BAD_VALUE;
      trans.y = BAD_VALUE;
      trans.z = BAD_VALUE;
      sqlite3_finalize( ppStmt );
      return;
    }

    rc = sqlite3_step( ppStmt );

    trans.x = ( float )sqlite3_column_double( ppStmt, 0 );
    trans.y = ( float )sqlite3_column_double( ppStmt, 1 );
    trans.z = ( float )sqlite3_column_double( ppStmt, 2 );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getTranslationById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
      trans.x = BAD_VALUE;
      trans.y = BAD_VALUE;
      trans.z = BAD_VALUE;
    }

  } else {
    cerr << "mdcExhibitMdl::getTranslationById() - Database is unusable." << endl;
    trans.x = BAD_VALUE;
	trans.y = BAD_VALUE;
	trans.z = BAD_VALUE;
  }
}

void mdcExhibitMdl::getRotationById( vec3_t & rot, int id ) {
  const char   *   query = "SELECT rotation_x, rotation_y, rotation_z FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;

  if ( dbUsable ) {
    rc = sqlite3_prepare_v2( db, query, 69, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getRotationById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      rot.x = BAD_VALUE;
      rot.y = BAD_VALUE;
      rot.z = BAD_VALUE;
      sqlite3_finalize( ppStmt );
      return;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getRotationById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      rot.x = BAD_VALUE;
      rot.y = BAD_VALUE;
      rot.z = BAD_VALUE;
      sqlite3_finalize( ppStmt );
      return;
    }

    rc = sqlite3_step( ppStmt );

    rot.x = ( float )sqlite3_column_double( ppStmt, 0 );
    rot.y = ( float )sqlite3_column_double( ppStmt, 1 );
    rot.z = ( float )sqlite3_column_double( ppStmt, 2 );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getRotationById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
      rot.x = BAD_VALUE;
      rot.y = BAD_VALUE;
      rot.z = BAD_VALUE;
    }

  } else {
    cerr << "mdcExhibitMdl::getRotationById() - Database is unusable." << endl;
    rot.x = BAD_VALUE;
    rot.y = BAD_VALUE;
	rot.z = BAD_VALUE;
  }
}

float mdcExhibitMdl::getRotationAmountById( int id ) {
  const char   *   query = "SELECT rotation_amout FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;
  float            value;

  if ( dbUsable ) {
    rc = sqlite3_prepare_v2( db, query, 69, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getRotationAmountById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      return BAD_VALUE;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getRotationAmountById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      sqlite3_finalize( ppStmt );
      return BAD_VALUE;
    }

    rc = sqlite3_step( ppStmt );

    value = ( float )sqlite3_column_double( ppStmt, 0 );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getRotationAmountById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
      return BAD_VALUE;
    }

    return value;

  } else {
    cerr << "mdcExhibitMdl::getRotationAmountById() - Database is unusable." << endl;
  }

  return BAD_VALUE;
}

void mdcExhibitMdl::getScalingById( vec3_t & scal, int id ) {
  const char   *   query = "SELECT scaling_x, scaling_y, scaling_z FROM exhibits WHERE id = ?";
  sqlite3_stmt *   ppStmt;
  int              rc;

  if ( dbUsable ) {
    rc = sqlite3_prepare_v2( db, query, 66, &ppStmt, NULL);

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getScalingById() - Failed to prepare query: " << sqlite3_errmsg( db ) << endl;
      scal.x = BAD_VALUE;
      scal.y = BAD_VALUE;
      scal.z = BAD_VALUE;
      sqlite3_finalize( ppStmt );
      return;
    }

    rc = sqlite3_bind_int( ppStmt, 1, id );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getScalingById() - Failed to bind parameters for query: " << sqlite3_errmsg( db ) << endl;
      scal.x = BAD_VALUE;
      scal.y = BAD_VALUE;
      scal.z = BAD_VALUE;
      sqlite3_finalize( ppStmt );
      return;
    }

    rc = sqlite3_step( ppStmt );

    scal.x = ( float )sqlite3_column_double( ppStmt, 0 );
    scal.y = ( float )sqlite3_column_double( ppStmt, 1 );
    scal.z = ( float )sqlite3_column_double( ppStmt, 2 );

    rc = sqlite3_finalize( ppStmt );

    if ( rc != SQLITE_OK ) {
      cerr << "mdcExhibitMdl::getScalingById() - Error finalizing query: " << sqlite3_errmsg( db ) << endl;
      scal.x = BAD_VALUE;
      scal.y = BAD_VALUE;
      scal.z = BAD_VALUE;
    }
  } else {
    cerr << "mdcExhibitMdl::ScalingById() - Database is unusable." << endl;
    scal.x = BAD_VALUE;
    scal.y = BAD_VALUE;
	scal.z = BAD_VALUE;
  }
}

void swap( int & a, int & b ){
  int temp = a;
  a = b;
  b = temp;
}
