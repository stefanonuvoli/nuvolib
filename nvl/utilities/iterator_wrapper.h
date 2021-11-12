#ifndef NVL_STRUCTURES_ITERATOR_WRAPPER_H
#define NVL_STRUCTURES_ITERATOR_WRAPPER_H

#include <nvl/nuvolib.h>

namespace nvl {

template<class C, class I>
class IteratorWrapper
{

public:
    IteratorWrapper();
    IteratorWrapper(C* container);

    I begin();
    I end();

private:

    C* vContainer;
};

}

#include "iterator_wrapper.cpp"

#endif // NVL_STRUCTURES_CONTAINER_ITERATOR_WRAPPER_H
