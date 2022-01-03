/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_MATRIX_H
#define NVL_MATH_MATRIX_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Dense>

namespace nvl {

template<class T> using MatrixBase = Eigen::MatrixBase<T>;

template<class T, EigenId D1, EigenId D2> using Matrix = Eigen::Matrix<T,D1,D2>;

template<class T> using MatrixXX = Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
template<class T, EigenId D1> using MatrixFX = Matrix<T, D1, Eigen::Dynamic>;
template<class T, EigenId D2> using MatrixXF = Matrix<T, Eigen::Dynamic, D2>;


/* Common typedefs */

template<class T>
using Matrix22 = Matrix<T,2,2>;
template<class T>
using Matrix33 = Matrix<T,3,3>;
template<class T>
using Matrix44 = Matrix<T,4,4>;

typedef Matrix22<float> Matrix22f;
typedef Matrix22<double> Matrix22d;
typedef Matrix22<int> Matrix22i;
typedef Matrix22<long> Matrix22l;

typedef Matrix33<float> Matrix33f;
typedef Matrix33<double> Matrix33d;
typedef Matrix33<int> Matrix33i;
typedef Matrix33<long> Matrix33l;

typedef Matrix44<float> Matrix44f;
typedef Matrix44<double> Matrix44d;
typedef Matrix44<int> Matrix44i;
typedef Matrix44<long> Matrix44l;

typedef MatrixXX<float> MatrixXXf;
typedef MatrixXX<double> MatrixXXd;
typedef MatrixXX<int> MatrixXXi;
typedef MatrixXX<long> MatrixXXl;

}

#endif

#endif // NVL_MATH_MATRIX_H
