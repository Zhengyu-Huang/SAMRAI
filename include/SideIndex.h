//
// File:	SideIndex.h
// Package:	SAMRAI patch data
// Copyright:	(c) 1997-2005 The Regents of the University of California
// Revision:	$Revision: 173 $
// Modified:	$Date: 2005-01-19 09:09:04 -0800 (Wed, 19 Jan 2005) $
// Description:	hier::Index for side centered patch data types
//

#ifndef included_pdat_SideIndex
#define included_pdat_SideIndex

#ifndef included_SAMRAI_config
#include "SAMRAI_config.h"
#endif
#ifndef included_hier_IntVector
#include "IntVector.h"
#endif
#ifndef included_hier_Index
#include "Index.h"
#endif

namespace SAMRAI {
    namespace pdat {

/**
 * Class SideIndex<DIM> implements a simple n-dimensional integer
 * vector for side centered variables.  Side indices contain an integer
 * index location in AMR index space along with the designated side axis
 * (X=0, Y=1, or Z=2).  See the side box geometry class for more information
 * about the mapping between the AMR index space and the side indices.
 *
 * @see hier::Index
 * @see pdat::SideData
 * @see pdat::SideGeometry
 * @see pdat::SideIterator
 */

template<int DIM> class SideIndex : public hier::Index<DIM>
{
public:
   /**
    * The default constructor for a side index creates an uninitialized index.
    */
   SideIndex();

   /**
    * Construct a side index from a regular index, axis, and side.  The axis
    * can be one of SideIndex<DIM>::X (0), SideIndex<DIM>::Y (1), or
    * SideIndex<DIM>::Z (2). The side argument can be one of the constants
    * SideIndex<DIM>::Lower (0) or SideIndex<DIM>::Upper (1).
    */
   SideIndex(const hier::Index<DIM>& rhs, const int axis, const int side);

   /**
    * The copy constructor creates a side index equal to the argument.
    */
   SideIndex(const SideIndex<DIM>& rhs);

   /**
    * The assignment operator sets the side index equal to the argument.
    */
   SideIndex<DIM>& operator=(const SideIndex<DIM>& rhs);

   /**
    * The side index destructor does nothing interesting.
    */
   ~SideIndex<DIM>();

   /**
    * Get the axis for which this side index is defined (X=0, Y=1, Z=2).
    */
   int getAxis() const;

   /**
    * Set the side axis (X=0, Y=1, Z=2).
    */
   void setAxis(const int axis);

   /**
    * Convert the side index into the index on the left hand side 
    * (argument side == 0) or the right hand side (argument side == 1).
    */
   hier::Index<DIM> toCell(const int side) const;

   /**
    * Plus-equals operator for a side index and an integer vector.
    */
   SideIndex<DIM>& operator+=(const hier::IntVector<DIM>& rhs);

   /**
    * Plus operator for a side index and an integer vector.
    */
   SideIndex<DIM> operator+(const hier::IntVector<DIM>& rhs) const;

   /**
    * Plus-equals operator for a side index and an integer.
    */
   SideIndex<DIM>& operator+=(const int rhs);

   /**
    * Plus operator for a side index and an integer.
    */
   SideIndex<DIM> operator+(const int rhs) const;

   /**
    * Minus-equals operator for a side index and an integer vector.
    */
   SideIndex<DIM>& operator-=(const hier::IntVector<DIM>& rhs);

   /**
    * Minus operator for a side index and an integer vector.
    */
   SideIndex<DIM> operator-(const hier::IntVector<DIM>& rhs) const;

   /**
    * Minus-equals operator for a side index and an integer.
    */
   SideIndex<DIM>& operator-=(const int rhs);

   /**
    * Minus operator for a side index and an integer.
    */
   SideIndex<DIM> operator-(const int rhs) const;

   /**
    * Times-equals operator for a side index and an integer vector.
    */
   SideIndex<DIM>& operator*=(const hier::IntVector<DIM>& rhs);

   /**
    * Times operator for a side index and an integer vector.
    */
   SideIndex<DIM> operator*(const hier::IntVector<DIM>& rhs) const;

   /**
    * Times-equals operator for a side index and an integer.
    */
   SideIndex<DIM>& operator*=(const int rhs);

   /**
    * Times operator for a side index and an integer.
    */
   SideIndex<DIM> operator*(const int rhs) const;

   /**
    * Returns true if two side index objects are equal.  All components
    * and the corresponding side axes must be the same for equality.
    */
   bool operator==(const SideIndex<DIM>& rhs) const;

   /**
    * Returns true if two side index objects are not equal.  Any of
    * the components or axes may be different for inequality.
    */
   bool operator!=(const SideIndex<DIM>& rhs) const;

   enum {
      X = 0,
      Y = 1,
      Z = 2,
      Lower = 0,
      Upper = 1
   };

private:
   int d_axis;
};

}
}
#ifndef DEBUG_NO_INLINE
#include "SideIndex.I"
#endif
#endif

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "SideIndex.C"
#endif