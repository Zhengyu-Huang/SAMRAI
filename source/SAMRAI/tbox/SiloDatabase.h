/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   A database structure that stores Silo format data.
 *
 ************************************************************************/

#ifndef included_tbox_SiloDatabase
#define included_tbox_SiloDatabase

#include "SAMRAI/SAMRAI_config.h"

/*
 ************************************************************************
 *  THIS CLASS WILL BE UNDEFINED IF THE LIBRARY IS BUILT WITHOUT Silo
 ************************************************************************
 */
#ifdef HAVE_SILO

#include "SAMRAI/tbox/Database.h"
#include "SAMRAI/tbox/Array.h"
#include "SAMRAI/tbox/DatabaseBox.h"
#include "SAMRAI/tbox/Complex.h"
#include "SAMRAI/tbox/List.h"
#include "SAMRAI/tbox/PIO.h"

#include "silo.h"

#include <boost/shared_ptr.hpp>
#include <string>

namespace SAMRAI {
namespace tbox {

/**
 * Class SiloDatabase implements the interface of the Database
 * class to store data in the Silo (Hierarchical Data Format) data format.
 *
 * It is assumed that all processors will access the database in the same
 * manner.  Error reporting is done using the SAMRAI error reporting macros.
 *
 * @see tbox::Database
 */

class SiloDatabase:public Database
{
public:
   /**
    * The Silo database constructor creates an empty database with the
    * specified name.  By default the database will not be associated
    * with a file until it is mounted, using the mount() member function.
    *
    * When assertion checking is active, the name string must be non-empty.
    */
   explicit SiloDatabase(
      const std::string& name);

   /**
    * The database destructor closes the Silo group or data file.
    */
   virtual ~SiloDatabase();

   /**
    * Return true if the specified key exists in the database
    * and false otherwise.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual bool
   keyExists(
      const std::string& key);

   /**
    * Return an array of all keys in the current database.  Note that
    * no keys from subdatabases contained in the current database
    * will appear in the array.  To get the keys of any other
    * database, you must call this routine for that database.
    */
   virtual Array<std::string>
   getAllKeys();

   /**
    * @brief Return the type of data associated with the key.
    *
    * If the key does not exist, then INVALID is returned
    *
    * @param key Key name in database.
    */
   virtual enum DataType
   getArrayType(
      const std::string& key);

   /**
    * Return the size of the array associated with the key.  If the key
    * does not exist, then zero is returned.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual int
   getArraySize(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a database entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isDatabase(
      const std::string& key);

   /**
    * Create a new database with the specified key name and return a
    * pointer to it.  A new group with the key name is also created
    * in the data file.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual boost::shared_ptr<Database>
   putDatabase(
      const std::string& key);

   /**
    * Get the database with the specified key name.  If the specified
    * key does not represent a database entry in the database, then
    * an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual boost::shared_ptr<Database>
   getDatabase(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a boolean entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isBool(
      const std::string& key);

   /**
    * Create a boolean array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putBoolArray(
      const std::string& key,
      const bool * const data,
      const int nelements);

   /**
    * Get a boolean entry from the database with the specified key
    * name.  If the specified key does not exist in the database or
    * is not a boolean array, then an error message is printed and
    * the program exits.
    *
    * @param key Key name in database.
    */
   virtual Array<bool>
   getBoolArray(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a box entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isDatabaseBox(
      const std::string& key);

   /**
    * Create a box array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putDatabaseBoxArray(
      const std::string& key,
      const DatabaseBox * const data,
      const int nelements);

   /**
    * Get a box entry from the database with the specified key
    * name.  If the specified key does not exist in the database,
    * then an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual Array<DatabaseBox>
   getDatabaseBoxArray(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a char entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isChar(
      const std::string& key);

   /**
    * Create a character array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putCharArray(
      const std::string& key,
      const char * const data,
      const int nelements);

   /**
    * Get a character entry from the database with the specified key
    * name.  If the specified key does not exist in the database,
    * then an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual Array<char>
   getCharArray(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a complex entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isComplex(
      const std::string& key);

   /**
    * Create a complex array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putComplexArray(
      const std::string& key,
      const dcomplex * const data,
      const int nelements);

   /**
    * Get a complex entry from the database with the specified key
    * name.  If the specified key does not exist in the database
    * then an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual Array<dcomplex>
   getComplexArray(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a double entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isDouble(
      const std::string& key);

   /**
    * Create a double array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putDoubleArray(
      const std::string& key,
      const double * const data,
      const int nelements);

   /**
    * Get a double entry from the database with the specified key
    * name.  If the specified key does not exist in the database
    * then an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual Array<double>
   getDoubleArray(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a float entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isFloat(
      const std::string& key);

   /**
    * Create a float array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putFloatArray(
      const std::string& key,
      const float * const data,
      const int nelements);

   /**
    * Get a float entry from the database with the specified key
    * name.  If the specified key does not exist in the database
    * then an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual Array<float>
   getFloatArray(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents an integer entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isInteger(
      const std::string& key);

   /**
    * Create an integer array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putIntegerArray(
      const std::string& key,
      const int * const data,
      const int nelements);

   /**
    * Get an integer entry from the database with the specified key
    * name.  If the specified key does not exist in the database
    * then an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual Array<int>
   getIntegerArray(
      const std::string& key);

   /**
    * Return true or false depending on whether the specified key
    * represents a string entry.  If the key does not exist or if
    * the string is empty, then false is returned.
    */
   virtual bool
   isString(
      const std::string& key);

   /**
    * Create a string array entry in the database with the specified
    * key name.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual void
   putStringArray(
      const std::string& key,
      const std::string * const data,
      const int nelements);

   /**
    * Get a string entry from the database with the specified key
    * name.  If the specified key does not exist in the database
    * then an error message is printed and the program exits.
    *
    * When assertion checking is active, the key string must be non-empty.
    */
   virtual Array<std::string>
   getStringArray(
      const std::string& key);

   /**
    * @brief Returns the name of this database.
    *
    * The name for the root of the database is the name supplied when creating it.
    * Names for nested databases are the keyname of the database.
    *
    */
   virtual std::string
   getName();

   /**
    * Print contents of current database to the specified output stream.
    * If no output stream is specified, then data is written to stream pout.
    * Note that none of the subdatabases contained in the current database
    * will have their contents printed.  To view the contents of any other
    * database, you must call this print routine for that database.
    */
   virtual void
   printClassData(
      std::ostream& os = pout);

   /**
    * Create a new database file.
    *
    * Returns true if successful.
    *
    * @param name name of database. Normally a filename.
    */
   virtual bool
   create(
      const std::string& name);

   /**
    * Open an existing database file.
    *
    * Returns true if successful.
    *
    * @param name name of database. Normally a filename.
    *
    * @param read_write_mode Open the database in read-write
    * mode instead of read-only mode.  NOTE: This class currently
    * does not support read-only mode, so this flag must be true.
    */
   virtual bool
   open(
      const std::string& name,
      const bool read_write_mode = false);

   /**
    * @brief Attach the Database to an existing Silo file.
    *
    * If an application has an existing Silo file used for restart this
    * method can be used to write SAMRAI restart information to the
    * existing file instead of SAMRAI creating a distinct file.
    *
    * SAMRAI data will be written to the directory specified within
    * the Silo file.
    *
    * Returns true if attach was successful.
    */
   virtual bool
   attachToFile(
      DBfile * file,
      const std::string& directory);

   /**
    * Close the database.
    *
    * Returns true if successful.
    *
    * If the database is currently open then close it.  This should
    * flush all data to the file (if the database is on disk).
    */
   virtual bool
   close();

   using Database::putBoolArray;
   using Database::getBoolArray;
   using Database::putDatabaseBoxArray;
   using Database::getDatabaseBoxArray;
   using Database::putCharArray;
   using Database::getCharArray;
   using Database::putComplexArray;
   using Database::getComplexArray;
   using Database::putFloatArray;
   using Database::getFloatArray;
   using Database::putDoubleArray;
   using Database::getDoubleArray;
   using Database::putIntegerArray;
   using Database::getIntegerArray;
   using Database::putStringArray;
   using Database::getStringArray;

private:
   SiloDatabase(
      const SiloDatabase&);             // not implemented
   void
   operator = (
      const SiloDatabase&);                 // not implemented

   bool
   isSiloType(
      const std::string& key,
      DBObjectType type);

   bool
   isSiloSimpleType(
      const std::string& key,
      const int simple_type);

   bool
   putSiloSimpleType(
      const std::string& key,
      const void* data,
      const int nelements,
      const int simple_type);

   bool
   getSiloSimpleType(
      const std::string& key,
      void* data);

   int
   getSiloSimpleTypeLength(
      const std::string& key);

   /*!
    * @brief Convert/mangle SAMRAI name into characters acceptable to SILO naming conventions.
    *
    * SILO only supports alphanumeric and "_" as valid characters in names.  Other characters
    * are allowed by SAMRAI (such as " " and "-") so they are converted to a mangled
    * representation for SILO.
    */
   std::string
   nameMangle(
      std::string name);

   /*!
    * @brief Convert/unmangle SILO name with possible mangled characters back to SAMRAI name.
    */
   std::string
   nameDemangle(
      std::string name);

   /*!
    * @brief Determine if character is a valid SILO character.
    *
    * Allowed values are alphanumeric, "-" and "/".
    */
   bool
   IsValid(
      int i);

   /*
    * Private constructor used internally to create sub-databases.
    */
   SiloDatabase(
      const std::string& name,
      DBfile* file,
      const std::string& directory,
      const bool create_in_file);

   /*
    * Silo file and group id, boolean flag indicating whether database
    * is associated with a mounted file, and name of this database object.
    */
   /*!
    * @brief Whether database is mounted to a file
    */
   bool d_is_file;
   /*!
    * @brief ID of file attached to database
    *
    * Is either -1 (not mounted to file) or set to the return value from
    * opening a file.
    * Set to -1 on unmounting the file.
    */
   DBfile* d_file;

   /*!
    * @brief Full directory of the database
    *
    * A database object is always attached to a directory in the Silo file.
    * The directory is set in the constructor when constructing.
    * If the object is file, the directory is the root.
    */
   std::string d_directory;

   /*
    * Name of this database object;
    */
   std::string d_database_name;

   /*
    * Names used for storing complex SAMRAI objects.
    */
   static const std::string DATABASE_BOX_NAME;
   static const std::string STRING_ARRAY_NAME;
   static const std::string COMPLEX_ARRAY_NAME;

   /*
    * String identifier for a mangled character.  A mangled character is stored
    * as <mangleID><hex> where hex is the ASCI code in hexadecimal.
    */
   static const std::string mangleID;
};

}
}

#endif

#endif
