/***********************************************************************************
Copyright (c) 2017, Michael Neunert, Markus Giftthaler, Markus Stäuble, Diego Pardo,
Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of ETH ZURICH nor the names of its contributors may be used
      to endorse or promote products derived from this software without specific
      prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL ETH ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************************/

// some checks
#if !defined(STATE_DIM_PRESPEC)
	static_assert(true, "STATE_DIM_PRESPEC not defined");
#endif

#if !defined(CONTROL_DIM_PRESPEC)
	static_assert(true, "CONTROL_DIM_PRESPEC not defined");
#endif

#ifndef SCALAR_PRESPEC
#define SCALAR_PRESPEC double
#endif

#ifndef POS_DIM_PRESPEC
#define POS_DIM_PRESPEC STATE_DIM_PRESPEC/2
#endif

#ifndef VEL_DIM_PRESPEC
#define VEL_DIM_PRESPEC STATE_DIM_PRESPEC/2
#endif

// default definition of MPC solver template
#ifndef MPC_SOLVER_PRESPEC
#define MPC_SOLVER_PRESPEC ct::optcon::NLOptConSolver<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, POS_DIM_PRESPEC, VEL_DIM_PRESPEC, SCALAR_PRESPEC>
#endif

// the default definition of the MPC policy-type is defined as the policy of the chosen MPC-solver
#ifndef MPC_POLICY_PRESPEC
#define MPC_POLICY_PRESPEC MPC_SOLVER_PRESPEC::Policy_t
#endif


// constraint terms
template class ct::optcon::ConstraintBase<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::ControlInputConstraint<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::tpl::ObstacleConstraint<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::StateConstraint<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::TerminalConstraint<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;

// constraints
template class ct::optcon::ConstraintContainerBase<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::LinearConstraintContainer<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::ConstraintContainerAD<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::ConstraintContainerAnalytical<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;

// lqr
template class ct::optcon::CARE<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC>;
template class ct::optcon::DARE<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::LQR<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC>;
template class ct::optcon::FHDTLQR<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;

// problem
template class ct::optcon::LQOCProblem<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::OptConProblem<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;

// nloc
template class ct::optcon::NLOCBackendBase<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, POS_DIM_PRESPEC, VEL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::NLOCBackendST<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, POS_DIM_PRESPEC, VEL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::NLOCBackendMP<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, POS_DIM_PRESPEC, VEL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::GNMS<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, POS_DIM_PRESPEC, VEL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::iLQR<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, POS_DIM_PRESPEC, VEL_DIM_PRESPEC, SCALAR_PRESPEC>;

// solver
template class ct::optcon::GNRiccatiSolver<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::NLOptConSolver<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, POS_DIM_PRESPEC, VEL_DIM_PRESPEC, SCALAR_PRESPEC>;

// mpc
template class ct::optcon::MPC<MPC_SOLVER_PRESPEC>;
template class ct::optcon::tpl::MpcTimeHorizon<SCALAR_PRESPEC>;
template class ct::optcon::PolicyHandler<MPC_POLICY_PRESPEC, STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
template class ct::optcon::StateFeedbackPolicyHandler<STATE_DIM_PRESPEC, CONTROL_DIM_PRESPEC, SCALAR_PRESPEC>;
