#include "cluster_kalman.hpp"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::vector;

ClusterKalman::ClusterKalman()
{
	m_bIsInitialize = true;

	m_dPreviousTimestamp = 0;

	// initializing matrices
	m_R_laser = MatrixXd(2, 2);

	//the initial transition matrix m_F
	m_F = MatrixXd(4, 4);
	m_F <<	1, 0, 1, 0,
			0, 1, 0, 1,
			0, 0, 1, 0,
			0, 0, 0, 1;

	//state covariance matrix m_P
	m_P = MatrixXd(4, 4);
	m_P <<	1, 0, 0,	0,
			0, 1, 0,	0,
			0, 0, 1000, 0,
			0, 0, 0,	1000;
}

ClusterKalman::~ClusterKalman() {}

void ClusterKalman::ProcessMeasurement(const MeasurementPackage &measurement_pack) 
{
	/**
	 * Initialization
	 */
	if (m_bIsInitialize) {

		// first measurement
		m_x = VectorXd(4);

		// Initialize state.
		// set the state with the initial location and zero velocity
		m_x << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0, 0;

		// done initializing, no need to predict or update
		m_bIsInitialize = false;
		m_dPreviousTimestamp = measurement_pack.timestamp_;

		return;
	}

	/**
	 * Prediction
	 */

	/**
	 * TODO: Update the state transition matrix F according to the new elapsed time.
	 * Time is measured in seconds.
	 * TODO: Update the process noise covariance matrix.
	 * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
	 */
	//compute the time elapsed between the current and previous measurements
	float dt = (measurement_pack.timestamp_ - m_dPreviousTimestamp) / 1000000.0;	//dt - expressed in seconds
	m_dPreviousTimestamp = ros::Time::now().toSec();

	float dt_2 = dt   * dt;
	float dt_3 = dt_2 * dt;
	float dt_4 = dt_3 * dt;

	//Modify the F matrix so that the time is integrated
	m_F (0, 2) = dt;
	m_F (1, 3) = dt;

	//set the acceleration noise components
	float noise_ax = 9;
	float noise_ay = 9;

	//set the process covariance matrix Q
	m_Q = MatrixXd(4, 4);
	m_Q  <<	dt_4 / 4 * noise_ax,	0,						dt_3 / 2 * noise_ax,	0,
			0,						dt_4 / 4 * noise_ay,	0,						dt_3 / 2 * noise_ay,
			dt_3 / 2 * noise_ax,	0,						dt_2*noise_ax,			0,
			0,						dt_3 / 2 * noise_ay,	0,						dt_2*noise_ay;

	predict();

	/**
	 * Update
	 * - Update the state and covariance matrices.
	 */

	// Laser updates
	m_H = m_H_laser;
	m_R = m_R_laser;
	update(measurement_pack.raw_measurements_);


	// print the output
	cout << "m_x = " << m_x << endl;
	cout << "m_P = " << m_P << endl;
}


void ClusterKalman::predict() 
{
	// predict the state
	m_x = m_F * m_x;
	MatrixXd Ft = m_F.transpose();
	m_P = m_F * m_P * Ft + m_Q;
}

void ClusterKalman::update(const VectorXd &z) 
{
	// update the state by using Kalman Filter equations
	VectorXd z_pred = m_H * m_x;
	VectorXd y = z - z_pred;
	MatrixXd Ht = m_H.transpose();
	MatrixXd S = m_H * m_P * Ht + m_R;
	MatrixXd Si = S.inverse();
	MatrixXd PHt = m_P * Ht;
	MatrixXd K = PHt * Si;

	//new estimate
	m_x = m_x + (K * y);
	long x_size = m_x.size();
	MatrixXd I = MatrixXd::Identity(x_size, x_size);
	m_P = (I - K * m_H) * m_P;
}
