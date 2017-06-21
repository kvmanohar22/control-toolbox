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

#ifndef DYNAMICRICCATIEQUATION_HPP_
#define DYNAMICRICCATIEQUATION_HPP_

namespace ct {
namespace optcon {


/*!
 * \ingroup LQR
 *+
 * \brief Dynamic Riccati Equation
 *
 * solves the Dynamic Algebraic Riccati Equation
 *
 * @tparam STATE_DIM the system state dimension
 * @tparam CONTROL_DIM the system control input dimension
 */
template <size_t STATE_DIM, size_t CONTROL_DIM>
class DynamicRiccatiEquation {

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	typedef Eigen::Matrix<double, STATE_DIM, STATE_DIM> state_matrix_t;
	typedef Eigen::Matrix<double, CONTROL_DIM, CONTROL_DIM> control_matrix_t;
	typedef Eigen::Matrix<double, CONTROL_DIM, 1> control_vector_t;
	typedef Eigen::Matrix<double, CONTROL_DIM, STATE_DIM> control_state_matrix_t;
	typedef Eigen::Matrix<double, STATE_DIM, CONTROL_DIM> control_gain_matrix_t;
	typedef Eigen::Matrix<double, CONTROL_DIM, STATE_DIM> control_feedback_t;


	DynamicRiccatiEquation() :
		epsilon_(1e-5),
		eigenvalueSolver_(CONTROL_DIM)
	{};

	~DynamicRiccatiEquation() {};

	// Naive, direct literal implementation
	// computes P = Q + A^T ( P - P B ( R + B^T P B )^-1 B^T P) A.
	//            = Q + A^T ( P - P B H_inverse B^T P) A
	//            = Q + A^T S A
	// where H_inverse = (R + B^T P B)^-1 and S = P - P B H_inverse B^T * P
	// i.e. P is altered and H_inverse is returned for convenience and use in other functions
	void iterateNaive(
		const state_matrix_t& Q_n,
		const control_matrix_t& R_n,
		const state_matrix_t& A_n,
		const control_gain_matrix_t& B_n,
		state_matrix_t& P_np1,
		control_feedback_t& K_n)
	{
		control_matrix_t H_n = R_n;
		H_n.noalias() += B_n.transpose() * P_np1 * B_n;
		control_matrix_t H_n_inverse = H_n.inverse();

		//std::cout << "H_n (naive)" << std::endl << H_n << std::endl;
		//std::cout << "H_n_inverse (naive)" << std::endl << H_n.inverse() << std::endl;

		K_n.noalias() = -1.0 * H_n_inverse * B_n.transpose() * P_np1 * A_n;

		// here we compute P_n
		P_np1 = Q_n + (A_n.transpose() * P_np1 * A_n);
		P_np1.noalias() -= K_n.transpose() * H_n * K_n;

		P_np1 = (P_np1 + P_np1.transpose()).eval()/2.0;
	}


	void iterateRobust(
		const state_matrix_t& Q_n,
		const control_matrix_t& R_n,
		const state_matrix_t& A_n,
		const control_gain_matrix_t& B_n,
		state_matrix_t& P_np1,
		control_feedback_t& K_n
	)
	{
		control_matrix_t H_n = R_n;
		H_n.noalias() += B_n.transpose() * P_np1 * B_n;
		H_n = (H_n + H_n.transpose()).eval() / 2.0;

	 	// compute eigenvalues with eigenvectors enabled
		eigenvalueSolver_.compute(H_n, true);
	 	const control_matrix_t& V = eigenvalueSolver_.eigenvectors().real();
        const control_vector_t& lambda = eigenvalueSolver_.eigenvalues().real();
		assert(eigenvalueSolver_.eigenvectors().imag().norm() < 1e-7 &&
					"Eigenvectors not real");
		assert(eigenvalueSolver_.eigenvalues().imag().norm() < 1e-7 &&
					"Eigenvalues is not real");
		 // Corrected Eigenvalue Matrix
         control_matrix_t D = control_matrix_t::Zero();
		 // make D positive semi-definite (as described in IV. B.)
		 D.diagonal() = lambda.cwiseMax(control_vector_t::Zero()) + epsilon_ * control_vector_t::Ones();

		 assert((V.transpose() - V.inverse()).norm() <  1e-5 &&
		  		"WARNING: V transpose and V inverse are not similar!");

		 // reconstruct H
		 H_n.noalias() = V * D * V.transpose();

		 // invert D
		 control_matrix_t D_inverse = control_matrix_t::Zero();
		 // eigenvalue-wise inversion
		 D_inverse.diagonal() = D.diagonal().cwiseInverse();
		 control_matrix_t H_n_inverse = V * D_inverse * V.transpose();

		 K_n.noalias() = -1.0 * H_n_inverse * (B_n.transpose() * P_np1 * A_n);

		 // here we compute P_n
		 P_np1 = Q_n + (A_n.transpose() * P_np1 * A_n);
		 P_np1.noalias() -= K_n.transpose() * H_n * K_n;

		 P_np1 = (P_np1 + P_np1.transpose()).eval()/2.0;
	}


private:
	double epsilon_;
	Eigen::EigenSolver<control_matrix_t> eigenvalueSolver_;

};


} // namespace ricatti
} // namespace lqr

#endif /* DYNAMICRICCATIEQUATION_HPP_ */
