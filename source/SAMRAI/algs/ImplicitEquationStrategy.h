/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2016 Lawrence Livermore National Security, LLC
 * Description:   Interface between implicit integrator and equations to solve.
 *
 ************************************************************************/

#ifndef included_algs_ImplicitEquationStrategy
#define included_algs_ImplicitEquationStrategy

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/solv/SAMRAIVectorReal.h"

#include "boost/shared_ptr.hpp"

namespace SAMRAI {
namespace algs {

/*!
 * @brief @b Abstract base class defining the interface between
 * ImplicitIntegrator object and routines
 * specific to the system of equations to be solved.
 *
 * This is an @b abstract base class defining the interface between
 * ImplicitIntegrator object and routines
 * specific to the system of equations to be solved.  These methods include
 * defining the components which comprise the state of the nonlinear solution
 * vector, computing time increments used in the implicit integration process,
 * testing whether a newly computed solution is acceptable, etc.  The
 * interface follows the Strategy design pattern.  The methods declared
 * in the interface are provided in a user-supplied concrete equation class
 * derived from this base class.
 *
 * @see ImplicitIntegrator
 */
class ImplicitEquationStrategy
{
public:
   /**
    * Empty constructor for ImplicitEquationStrategy.
    */
   ImplicitEquationStrategy();

   /**
    * Empty destructor for ImplicitEquationStrategy
    */
   virtual ~ImplicitEquationStrategy();

   /**
    * Add components to the solution vector to define the state of the
    * solution for the implicit time integrator.
    */
   virtual void
   setupSolutionVector(
      const boost::shared_ptr<solv::SAMRAIVectorReal<double> >& solution) = 0;

   /**
    * Return time increment for advancing the solution at the first timestep.
    */
   virtual double
   getInitialDt() = 0;

   /**
    * Return time increment for next solution advance based on the
    * arguments provided.  The boolean argument good_solution is the
    * return value from the checkNewSolution() routine.  It should be
    * assumed that that routine is invoked prior to a call to this function.
    * The integer argument solver_retcode is the return code generated
    * by the nonlinear solver package that computed the solution.  It is
    * the responsibility of the subclass of this base class to interpret
    * the arguments properly when determining the time step.
    */
   virtual double
   getNextDt(
      const bool good_solution,
      const int solver_retcode) = 0;

   /**
    * Set initial guess for time advanced solution vector.  The first_step
    * is true if the subsequent time advance is the first in the simulation,
    * or if it immediately follows a regridding of the patch hierarchy.  In
    * either case, extrapolation of the solution may not be possible since
    * there may be no old solution to use.  Otherwise, first_step is false.
    * The current_time argument is the current solution time.  The
    * current_dt and old_dt arguments represent the time increments
    * used in the cuurent and previous advances, respectively.  Thus, the
    * new integration time to which we will attempt to advance the solution
    * will be current_time + current_dt.
    */
   virtual void
   setInitialGuess(
      const bool first_step,
      const double current_time,
      const double current_dt,
      const double old_dt) = 0;

   /**
    * Return true if the most recently computed time advanced solution
    * is acceptable.  Otherwise, return false.  The integer argument
    * is the return code generated by the nonlinear solver package
    * that computed the solution.  It is the responsibility of the
    * subclass of this base class to interpret that return code properly.
    */
   virtual bool
   checkNewSolution(
      const int solver_retcode) = 0;

   /**
    * Assuming that an acceptable time advanced solution has been found,
    * update the state of the solution (e.g., reset solution pointers)
    * and all dependent variables.  The argument is the integration time
    * to which the new solution corresponds.
    */
   virtual void
   updateSolution(
      const double new_time) = 0;

};

}
}

#endif
