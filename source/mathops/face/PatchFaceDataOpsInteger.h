//
// File:	PatchFaceDataOpsInteger.h
// Package:	SAMRAI mathops
// Copyright:	(c) 1997-2005 The Regents of the University of California
// Revision:	$Revision: 173 $
// Modified:	$Date: 2005-01-19 09:09:04 -0800 (Wed, 19 Jan 2005) $
// Description:	Operations for integer face-centered patch data.
//

#ifndef included_math_PatchFaceDataOpsInteger
#define included_math_PatchFaceDataOpsInteger

#ifndef included_SAMRAI_config
#include "SAMRAI_config.h"
#endif
#ifndef included_iostream
#define included_iostream
#include <iostream>
using namespace std;
#endif
#ifndef included_math_PatchFaceDataBasicOps
#include "PatchFaceDataBasicOps.h"
#endif
#ifndef included_tbox_ArrayDataNormOpsInteger
#include "ArrayDataNormOpsInteger.h"
#endif
#ifndef included_hier_Box
#include "Box.h"
#endif
#ifndef included_hier_Patch
#include "Patch.h"
#endif
#ifndef included_pdat_FaceData
#include "FaceData.h"
#endif
#ifndef included_tbox_PIO
#include "tbox/PIO.h"
#endif
#ifndef included_tbox_Pointer
#include "tbox/Pointer.h"
#endif

namespace SAMRAI {
    namespace math {

/**
 * Class PatchFaceDataOpsInteger<DIM> provides a collection of operations
 * that may be used to manipulate integer face-centered patch data.  The
 * operations include basic arithmetic, min, max, etc.  With the exception 
 * of a few basic routines, this class inherits its interface (and 
 * thus its functionality) from the base class PatchFaceDataBasicOps<DIM>
 * from which it is derived.
 *
 * A more extensive set of operations is implemented for real (double and 
 * float) and complex patch data in the classes PatchFaceDataOpsReal<DIM> 
 * and PatchFaceDataOpsComplex<DIM>, repsectively. 
 *
 * @see math::PatchFaceDataBasicOps
 */

template<int DIM>
class PatchFaceDataOpsInteger : 
   public tbox::DescribedClass,
   public PatchFaceDataBasicOps<DIM,int>
{
public:
   /** 
    * Empty constructor and destructor.
    */
   PatchFaceDataOpsInteger();

   virtual ~PatchFaceDataOpsInteger<DIM>();

   /**
    * Return the number of data values for the face-centered data object
    * in the given box.  Note that it is assumed that the box refers to
    * the cell-centered index space corresponding to the patch hierarchy.
    */
   int numberOfEntries(const tbox::Pointer< pdat::FaceData<DIM,int> >& data,
                       const hier::Box<DIM>& box) const;

   /**
    * Copy dst data to src data over given box.
    */
   void copyData(tbox::Pointer< pdat::FaceData<DIM,int> >& dst,
                 const tbox::Pointer< pdat::FaceData<DIM,int> >& src,
                 const hier::Box<DIM>& box) const;

   /**
    * Swap pointers for patch data objects.  Objects are checked for 
    * consistency of depth, box, and ghost box.
    */
   void swapData(tbox::Pointer< hier::Patch<DIM> > patch,
                 const int data1_id,
                 const int data2_id) const;

   /**
    * Print data entries over given box to given output stream.
    */
   void printData(const tbox::Pointer< pdat::FaceData<DIM,int> >& data,
                  const hier::Box<DIM>& box,
                  ostream& s = tbox::plog) const;

   /**
    * Initialize data to given scalar over given box.
    */
   void setToScalar(tbox::Pointer< pdat::FaceData<DIM,int> >& dst,
                    const int& alpha,
                    const hier::Box<DIM>& box) const;

   /**
    * Set destination component to absolute value of source component.
    * That is, each destination entry is set to \f$d_i = \| s_i \|\f$.
    */
   void abs(tbox::Pointer< pdat::FaceData<DIM,int> >& dst,
            const tbox::Pointer< pdat::FaceData<DIM,int> >& src,
            const hier::Box<DIM>& box) const;

private:
   // The following are not implemented:
   PatchFaceDataOpsInteger(const PatchFaceDataOpsInteger<DIM>&);
   void operator=(const PatchFaceDataOpsInteger<DIM>&);

   ArrayDataNormOpsInteger<DIM> d_array_ops;

};

}
}
#endif

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "PatchFaceDataOpsInteger.C"
#endif