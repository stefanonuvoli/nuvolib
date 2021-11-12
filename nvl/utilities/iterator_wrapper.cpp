#include "iterator_wrapper.h"

namespace nvl {

template<class C, class I>
IteratorWrapper<C,I>::IteratorWrapper(C* container) : vContainer(container)
{

}

template<class C, class I>
IteratorWrapper<C,I>::IteratorWrapper() : vContainer(nullptr)
{

}

template<class C, class I>
I IteratorWrapper<C,I>::begin()
{
    return vContainer->begin();
}

template<class C, class I>
I IteratorWrapper<C,I>::end()
{
    return vContainer->end();
}

}
