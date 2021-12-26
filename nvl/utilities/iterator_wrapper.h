/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_STRUCTURES_ITERATOR_WRAPPER_H
#define NVL_STRUCTURES_ITERATOR_WRAPPER_H

#include <nvl/nuvolib.h>

namespace nvl {

/**
 * @brief Wrapper for an iterator. It allows to hide the function of the given container,
 * in a way the user can use only the iterators.
 */
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
