#include "segment.h"

namespace nvl {

template<class P>
Segment<P>::Segment()
{

}

template<class P>
Segment<P>::Segment(const P& p1, const P& p2)
{
    this->vP1 = p1;
    this->vP2 = p2;

    swapIfNecessary();
}

template<class P>
const P& Segment<P>::p1() const
{
    return vP1;
}

template<class P>
P& Segment<P>::p1()
{
    return vP1;
}

template<class P>
void Segment<P>::setP1(const P& value)
{
    vP1 = value;

    swapIfNecessary();
}

template<class P>
const P& Segment<P>::p2() const
{
    return vP2;
}

template<class P>
P& Segment<P>::p2()
{
    return vP2;
}

template<class P>
void Segment<P>::setP2(const P& value)
{
    vP2 = value;

    swapIfNecessary();
}


template<class P>
bool Segment<P>::operator<(const Segment<P>& seg) const
{
    if (this->vP1 < seg.vP1)
        return true;
    if (seg.vP1 < this->vP1)
        return false;

    return seg.vP2 < this->vP2;
}

template<class P>
bool Segment<P>::operator<=(const Segment<P>& seg) const
{
    if (*this < seg)
        return true;

    return *this == seg;
}

template<class P>
bool Segment<P>::operator>(const Segment<P>& seg) const
{
    if (this->vP1 > seg.vP1)
        return true;
    if (seg.vP1 > this->vP1)
        return false;

    return seg.vP2 > this->vP2;
}

template<class P>
bool Segment<P>::operator>=(const Segment<P>& seg) const
{
    if (*this > seg)
        return true;

    return *this == seg;
}

template<class P>
bool Segment<P>::operator==(const Segment<P>& seg) const
{
    return this->vP1 == seg.vP1 && this->vP2 == seg.vP2;
}

template<class P>
void Segment<P>::swapIfNecessary()
{
    if (vP2 < vP1)
        std::swap(vP1, vP2);

}


}
