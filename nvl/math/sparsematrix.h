#ifndef NVL_MATH_SPARSE_MATRIX_H
#define NVL_MATH_SPARSE_MATRIX_H

#include <nvl/nuvolib.h>

#include <Eigen/Sparse>

namespace nvl {

template<class T> using SparseMatrix = Eigen::SparseMatrix<T>;

/* Common typedefs */

typedef SparseMatrix<float> SparseMatrixf;
typedef SparseMatrix<double> SparseMatrixd;
typedef SparseMatrix<int> SparseMatrixi;
typedef SparseMatrix<long> SparseMatrixl;

}

#endif // NVL_MATH_SPARSE_MATRIX_H
