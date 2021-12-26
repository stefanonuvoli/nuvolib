/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "iterator_wrapper.h"

namespace nvl {

/**
 * @brief Default constructor. No container.
 */
template<class C, class I>
IteratorWrapper<C,I>::IteratorWrapper() : vContainer(nullptr)
{

}

/**
 * @brief Constructor with the container
 * @param container Container
 */
template<class C, class I>
IteratorWrapper<C,I>::IteratorWrapper(C* container) : vContainer(container)
{

}

/**
 * @brief Get the begin iterator of the container
 * @return Begin iterator
 */
template<class C, class I>
I IteratorWrapper<C,I>::begin()
{
    return vContainer->begin();
}

/**
 * @brief Get the end iterator of the container
 * @return End iterator
 */
template<class C, class I>
I IteratorWrapper<C,I>::end()
{
    return vContainer->end();
}

}
