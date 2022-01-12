/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "segment.h"

namespace nvl {

/**
 * @brief Default constructor
 */
template<class P>
Segment<P>::Segment()
{

}

/**
 * @brief Constructor with end points
 * @param p1 Start point
 * @param p2 End point
 */
template<class P>
Segment<P>::Segment(const P& p1, const P& p2)
{
    this->vP1 = p1;
    this->vP2 = p2;

    sortEndpoints();
}

/**
 * @brief Get the first point
 * @return First point
 */
template<class P>
const P& Segment<P>::p1() const
{
    return vP1;
}

/**
 * @brief Get the first point
 * @return First point
 */
template<class P>
P& Segment<P>::p1()
{
    return vP1;
}

/**
 * @brief Set the first point
 * @param value First point
 */
template<class P>
void Segment<P>::setP1(const P& value)
{
    vP1 = value;

    sortEndpoints();
}

/**
 * @brief Get the second point
 * @return Second point
 */
template<class P>
const P& Segment<P>::p2() const
{
    return vP2;
}

/**
 * @brief Get the second point
 * @return Second point
 */
template<class P>
P& Segment<P>::p2()
{
    return vP2;
}

/**
 * @brief Set the second point
 * @param value Second point
 */
template<class P>
void Segment<P>::setP2(const P& value)
{
    vP2 = value;

    sortEndpoints();
}

/**
 * @brief Operator overload <
 * @param seg Other segment
 * @return True if the segment is less than the other segment, else otherwise
 */
template<class P>
bool Segment<P>::operator<(const Segment<P>& seg) const
{
    if (this->vP1 < seg.vP1)
        return true;
    if (seg.vP1 < this->vP1)
        return false;

    return seg.vP2 < this->vP2;
}

/**
 * @brief Operator overload <=
 * @param seg Other segment
 * @return True if the segment is less or equal than the other segment, else otherwise
 */
template<class P>
bool Segment<P>::operator<=(const Segment<P>& seg) const
{
    if (*this < seg)
        return true;

    return *this == seg;
}

/**
 * @brief Operator overload >
 * @param seg Other segment
 * @return True if the segment is greater than the other segment, else otherwise
 */
template<class P>
bool Segment<P>::operator>(const Segment<P>& seg) const
{
    if (this->vP1 > seg.vP1)
        return true;
    if (seg.vP1 > this->vP1)
        return false;

    return seg.vP2 > this->vP2;
}

/**
 * @brief Operator overload >=
 * @param seg Other segment
 * @return True if the segment is greater or equal than the other segment, else otherwise
 */
template<class P>
bool Segment<P>::operator>=(const Segment<P>& seg) const
{
    if (*this > seg)
        return true;

    return *this == seg;
}

/**
 * @brief Operator overload ==
 * @param seg Other segment
 * @return True if the segment is equal with the other segment, else otherwise
 */
template<class P>
bool Segment<P>::operator==(const Segment<P>& seg) const
{
    return this->vP1 == seg.vP1 && this->vP2 == seg.vP2;
}

/**
 * @brief Swap the end points if the second is less than the first
 */
template<class P>
void Segment<P>::sortEndpoints()
{
    if (vP2 < vP1)
        std::swap(vP1, vP2);
}

}
