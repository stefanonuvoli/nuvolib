/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_SPARSE_MATRIX_H
#define NVL_MATH_SPARSE_MATRIX_H

#include <nvl/nuvolib.h>

#ifdef NVL_EIGEN_LOADED

#include <Eigen/Sparse>

namespace nvl {

template<class T> using SparseMatrix = Eigen::SparseMatrix<T>;

/* Common typedefs */

typedef SparseMatrix<float> SparseMatrixf;
typedef SparseMatrix<double> SparseMatrixd;
typedef SparseMatrix<int> SparseMatrixi;
typedef SparseMatrix<long> SparseMatrixl;

}

#endif

#endif // NVL_MATH_SPARSE_MATRIX_H
