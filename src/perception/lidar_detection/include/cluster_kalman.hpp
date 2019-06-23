#ifndef __CLUSTER_KALMAN_H__
#define __CLUSTER_KALMAN_H__

#include <fstream>
#include <string>
#include <vector>
#include "Eigen/Dense"
#include "cluster.hpp"

#define PI  (3.141592653589793238)

class ClusterKalman
{
	private:
		// check whether the tracking toolbox was initialized or not (first measurement)
		bool m_bIsInitialize;

		// previous timestamp
		double m_dPreviousTimestamp;

		// tool object used to compute Jacobian and RMSE
		Eigen::MatrixXd m_R_laser;
		Eigen::MatrixXd m_H_laser;

		// state vector
		Eigen::VectorXd m_x;

		// state covariance matrix
		Eigen::MatrixXd m_P;

		// state transition matrix
		Eigen::MatrixXd m_F;

		// process covariance matrix
		Eigen::MatrixXd m_Q;

		// measurement matrix
		Eigen::MatrixXd m_H;

		// measurement covariance matrix
		Eigen::MatrixXd m_R;

	public:
		Cluster m_TrackingObject;

		ClusterKalman();
		virtual ~ClusterKalman();

		// Run the whole flow of the Kalman Filter from here.
		void ProcessMeasurement(const MeasurementPackage &measurement_pack);

		/**
		 * Init Initializes Kalman filter
		 * @param x_in Initial state
		 * @param P_in Initial state covariance
		 * @param F_in Transition matrix
		 * @param H_in Measurement matrix
		 * @param R_in Measurement covariance matrix
		 * @param Q_in Process covariance matrix
		 */
		void Init(Eigen::VectorXd &x_in, Eigen::MatrixXd &P_in, Eigen::MatrixXd &F_in,
				Eigen::MatrixXd &H_in, Eigen::MatrixXd &R_in, Eigen::MatrixXd &Q_in);

		/**
		 * Prediction Predicts the state and the state covariance
		 * using the process model
		 * @param delta_T Time between k and k+1 in s
		 */
		void predict();

		/**
		 * Updates the state by using standard Kalman Filter equations
		 * @param z The measurement at k+1
		 */
		void update(const Eigen::VectorXd &z);
};

#endif // __CLUSTER_KALMAN_H__
