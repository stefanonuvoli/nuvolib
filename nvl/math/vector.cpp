/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "vector.h"

namespace nvl {

template<class T, EigenId D>
Vector<T,D>::Vector()
{

}

template<class T, EigenId D>
template<typename... Ts>
Vector<T,D>::Vector(Ts... args)
    : Matrix<T,D,1>(args...)
{

}

template<class T, EigenId D>
template<typename OtherDerived>
Vector<T,D>& Vector<T,D>::operator=(const Matrix<T,D,1>& other)
{
    Matrix<T,D,1>::operator=(other);
    return *this;
}

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

template<class T, EigenId D>
bool Vector<T,D>::operator<=(const Vector<T,D>& vec) const
{
    if (*this < vec)
        return true;

    return *this == vec;
}

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

template<class T, EigenId D>
bool Vector<T,D>::operator>=(const Vector<T,D>& vec) const
{
    if (*this > vec)
        return true;

    return *this == vec;
}

}
