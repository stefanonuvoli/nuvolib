#ifndef NVL_NUVOLIB_H
#define NVL_NUVOLIB_H

#include "assert.h"

#include <stddef.h>
#include <limits>

#ifdef NVL_EIGEN_LOADED
#include <Eigen/Core>
#endif

/**
 * @brief Suppresses unused variable warning.
 */
#define NVL_SUPPRESS_UNUSEDVARIABLE(a) (void)a

/**
 * @brief Inline keyword define. It can allow us to switch header-only/source mode.
 */
#define NVL_INLINE inline

namespace nvl {

class NullType {

};

/**
 * @brief Default size of std containers.
 */
typedef size_t Size;

/**
 * @brief Default index type of std containers.
 */
typedef size_t Index;

/**
 * @brief Maximum index in a default container
 */
const Size NULL_ID = std::numeric_limits<Index>::max();

#ifdef NVL_EIGEN_LOADED
/**
 * @brief Default index type of Eigen.
 */
typedef int EigenId;

#endif

}

#endif //NVL_NUVOLIB_H
