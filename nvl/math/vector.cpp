/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "vector.h"

namespace nvl {

/**
 * @brief Default constructor
 */
template<class T, EigenId D>
Vector<T,D>::Vector() : Matrix<T, D, 1>()
{

}

/**
 * @brief Variadic argument constructor
 * @param args Arguments
 */
template<class T, EigenId D>
template<typename... Ts>
Vector<T,D>::Vector(Ts... args)
    : Matrix<T,D,1>(args...)
{

}

/**
 * @brief Operator overload =
 * @param other Other vector
 * @return Reference to the current vector
 */
template<class T, EigenId D>
template<typename OtherDerived>
Vector<T,D>& Vector<T,D>::operator=(const Matrix<T,D,1>& vec)
{
    Matrix<T,D,1>::operator=(vec);
    return *this;
}

/**
 * @brief Operator overload <
 * @param vec Other vector
 * @return True if it is less than the other vector
 */
template<class T, EigenId D>
bool Vector<T,D>::operator<(const Vector<T,D>& vec) const
{
    const Vector<T,D>& currentVec = *this;
    for (EigenId i = 0; i < D; ++i) {
        if (currentVec(i) < vec(i)) {
            return true;
        }
        else if (vec(i) < currentVec(i)) {
            return false;
        }
    }

    return false;
}

/**
 * @brief Operator overload <=
 * @param vec Other vector
 * @return True if it is less or equal than the other vector
 */
template<class T, EigenId D>
bool Vector<T,D>::operator<=(const Vector<T,D>& vec) const
{
    if (*this < vec)
        return true;

    return *this == vec;
}

/**
 * @brief Operator overload >
 * @param vec Other vector
 * @return True if it is greater than the other vector
 */
template<class T, EigenId D>
bool Vector<T,D>::operator>(const Vector<T,D>& vec) const
{
    const Vector<T,D>& currentVec = *this;
    for (EigenId i = 0; i < D; ++i) {
        if (currentVec(i) > vec(i)) {
            return true;
        }
        else if (vec(i) > currentVec(i)) {
            return false;
        }
    }

    return false;
}

/**
 * @brief Operator overload >=
 * @param vec Other vector
 * @return True if it is greater or equal than the other vector
 */
template<class T, EigenId D>
bool Vector<T,D>::operator>=(const Vector<T,D>& vec) const
{
    if (*this > vec)
        return true;

    return *this == vec;
}

}
