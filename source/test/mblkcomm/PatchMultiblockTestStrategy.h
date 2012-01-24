/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Base class for patch data test operations.
 *
 ************************************************************************/

#ifndef included_hier_PatchMultiblockTestStrategy
#define included_hier_PatchMultiblockTestStrategy

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/Array.h"
#include "SAMRAI/tbox/Database.h"
#include "SAMRAI/hier/Box.h"
#include "MultiblockTester.h"
#include "SAMRAI/hier/IntVector.h"
#include "SAMRAI/hier/Patch.h"
#include "SAMRAI/hier/PatchHierarchy.h"
#include "SAMRAI/hier/VariableContext.h"

#include <boost/shared_ptr.hpp>

using namespace SAMRAI;

//class MultiblockTester;

/**
 * Class PatchMultiblockTestStrategy defines an interface for testing specific
 * patch data transfer operations on individual patches when using
 * the MultiblockTester class.  This base class provides two member functions
 * for reading test input information.  These are:
 *
 * readVariableInput(): This function reads in a collection of databases,
 * each of which contains parameters for a single variable.  The names of
 * the sub-databases are arbitrary, but must be distinct.  Each variable
 * sub-database has the following input keys:
 *
 *
 *
 *
 *
 *    - \b  name         variable name string (required)
 *    - \b  depth        optional variable depth (default = 1)
 *    - \b  src_ghosts   optional comm source ghost width (default = 0,0,0)
 *    - \b  dst_ghosts   optional comm dest ghost width (default = 0,0,0)
 *    - \b  coarsen_operator   opt. coarsen op name (default = "NO_COARSEN")
 *    - \b  refine_operator    opt. refine op name (default = "NO_REFINE")
 *
 *
 *
 *
 *
 * readRefinementInput(): This function reads in a collection of box
 * arrays, each of which describes the region to refine on each level.
 * The key names of the box arrays are arbitrary, but must be distinct.
 * For example,
 *
 *
 *
 *
 *
 *    - \b  level0_boxes   boxes to refine on level zero.
 *    - \b  level1_boxes   boxes to refine on level one.
 *    - \b  ...            etc...
 *
 *
 *
 *
 *
 *
 * The pure virtual functions in this class that must be provided by
 * concrete test subclass:
 * \begin{enumerate}
 *    - [registerVariables(...)] register variables with MultiblockTester.
 *    - [initializeDataOnPatch(...)] set patch data on new patch.
 *    - [tagCellsToRefine(...)] tag cells on patch to refine.
 *    - [verifyResults(...)] check results of communication operations.
 * \end{enumerate}
 *
 * The following virtual functions are given default non-operations in this
 * class so that concrete test subclass can either implement them to test
 * specific functionality or simply ignore.  They are pure virtual in the
 * coarsen and refine patch strategy classes:
 * \begin{enumerate}
 *    - [setPhysicalBoundaryConditions(...)]
 *    - [preprocessRefine(...)]
 *    - [postprocessRefine(...)]
 *    - [preprocessCoarsen(...)]
 *    - [postprocessCoarsen(...)]
 * \end{enumerate}
 */

class PatchMultiblockTestStrategy
{
public:
   /**
    * The constructor initializes variable data arrays to zero length.
    */
   PatchMultiblockTestStrategy(
      const tbox::Dimension& dim);

   /**
    * Virtual destructor for PatchMultiblockTestStrategy.
    */
   virtual ~PatchMultiblockTestStrategy();

   void setGridGeometry(
      boost::shared_ptr<hier::GridGeometry>& grid_geom)
   {
      d_grid_geometry = grid_geom;
   }

   boost::shared_ptr<hier::GridGeometry>&
   getGridGeometry()
   {
      return d_grid_geometry;
   }

   /**
    * Utility functions for managing patch data context.
    */
   void setDataContext(
      boost::shared_ptr<hier::VariableContext> context)
   {
#ifdef DEBUG_CHECK_ASSERTIONS
      TBOX_ASSERT(context);
#endif
      d_data_context = context;
   }

   void setDestinationContext(
      boost::shared_ptr<hier::VariableContext> context)
   {
#ifdef DEBUG_CHECK_ASSERTIONS
      TBOX_ASSERT(context);
#endif
      d_dst_context = context;
   }

   void setScratchContext(
      boost::shared_ptr<hier::VariableContext> context)
   {
#ifdef DEBUG_CHECK_ASSERTIONS
      TBOX_ASSERT(context);
#endif
      d_scr_context = context;
   }

   ///
   boost::shared_ptr<hier::VariableContext> getDataContext() const
   {
      return d_data_context;
   }

   ///
   boost::shared_ptr<hier::VariableContext> getDestinationContext() const
   {
      return d_dst_context;
   }

   ///
   boost::shared_ptr<hier::VariableContext> getScratchContext() const
   {
      return d_scr_context;
   }

   void clearDataContext()
   {
      d_data_context.reset();
   }

   void clearDestinationContext()
   {
      d_dst_context.reset();
   }

   void clearScratchContext()
   {
      d_scr_context.reset();
   }

   /**
    * Read variable parameters from input database.
    */
   void
   readVariableInput(
      boost::shared_ptr<tbox::Database> db);

   /**
    * Read arrays of refinement boxes from input database.
    */
   void
   readRefinementInput(
      boost::shared_ptr<tbox::Database> db);

   /**
    * Set tags in cells to refine based on boxes provided in input.
    */
   void
   tagCellsInInputBoxes(
      hier::Patch& patch,
      int level_number,
      int tag_index);

   /**
    * Virtual functions in interface to user-supplied boundary conditions,
    * coarsen and refine operations.
    */
   virtual void
   setPhysicalBoundaryConditions(
      hier::Patch& patch,
      const double time,
      const hier::IntVector& gcw) const;

   virtual void fillSingularityBoundaryConditions(
      hier::Patch& patch,
      const hier::PatchLevel& encon_level,
      const hier::Connector& dst_to_encon,
      const hier::Box& fill_box,
      const hier::BoundaryBox& boundary_box,
      const boost::shared_ptr<hier::GridGeometry>& grid_geometry)
   {
      NULL_USE(patch);
      NULL_USE(encon_level);
      NULL_USE(dst_to_encon);
      NULL_USE(fill_box);
      NULL_USE(boundary_box);
      NULL_USE(grid_geometry);
   }

   ///
   virtual void
   preprocessRefine(
      hier::Patch& fine,
      const hier::Patch& coarse,
      const boost::shared_ptr<hier::VariableContext>& context,
      const hier::Box& fine_box,
      const hier::IntVector& ratio) const;

   ///
   virtual void
   postprocessRefine(
      hier::Patch& fine,
      const hier::Patch& coarse,
      const boost::shared_ptr<hier::VariableContext>& context,
      const hier::Box& fine_box,
      const hier::IntVector& ratio) const;

   ///
   virtual void
   preprocessCoarsen(
      hier::Patch& coarse,
      const hier::Patch& fine,
      const boost::shared_ptr<hier::VariableContext>& context,
      const hier::Box& coarse_box,
      const hier::IntVector& ratio) const;

   ///
   virtual void
   postprocessCoarsen(
      hier::Patch& coarse,
      const hier::Patch& fine,
      const boost::shared_ptr<hier::VariableContext>& context,
      const hier::Box& coarse_box,
      const hier::IntVector& ratio) const;

   /**
    * This function is called from the MultiblockTester constructor.  Its
    * purpose is to register variables used in the patch data test
    * and appropriate communication parameters (ghost cell widths,
    * coarsen/refine operations) with the MultiblockTester object, which
    * manages the variable storage.
    */
   virtual void
   registerVariables(
      MultiblockTester* commtest) = 0;

   /**
    * Virtual function for setting data on new patch in hierarchy.
    *
    * @param src_or_dst Flag set to 's' for source or 'd' for destination
    *        to indicate variables to set data for.
    */
   virtual void
   initializeDataOnPatch(
      hier::Patch& patch,
      const boost::shared_ptr<hier::PatchHierarchy> hierarchy,
      const int level_number,
      const hier::BlockId& block_id,
      char src_or_dst) = 0;

   /**
    * Virtual function for tagging cells to refine on the patch.
    */
   virtual void
   tagCellsToRefine(
      hier::Patch& patch,
      const boost::shared_ptr<hier::PatchHierarchy> hierarchy,
      int level_number,
      int tag_index) = 0;

   /**
    * Virtual function for checking results of communication operations.
    */
   virtual bool
   verifyResults(
      const hier::Patch& patch,
      const boost::shared_ptr<hier::PatchHierarchy> hierarchy,
      const int level_number,
      const hier::BlockId& block_id) = 0;

protected:
   const tbox::Dimension d_dim;

   /*
    * Arrays of information read from input file describing test variables
    */
   tbox::Array<string> d_variable_src_name;
   tbox::Array<string> d_variable_dst_name;
   tbox::Array<int> d_variable_depth;
   tbox::Array<hier::IntVector> d_variable_src_ghosts;
   tbox::Array<hier::IntVector> d_variable_dst_ghosts;
   tbox::Array<string> d_variable_coarsen_op;
   tbox::Array<string> d_variable_refine_op;

   /*
    * Arrays of information read from input file describing test variables
    */
   tbox::Array<hier::BoxContainer> d_refine_level_boxes;

private:
   boost::shared_ptr<hier::GridGeometry> d_grid_geometry;

   boost::shared_ptr<hier::VariableContext> d_data_context;
   boost::shared_ptr<hier::VariableContext> d_dst_context;
   boost::shared_ptr<hier::VariableContext> d_scr_context;

};

#endif
