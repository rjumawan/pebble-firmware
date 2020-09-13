/*
 *  Copyright (c) 2017, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file includes definitions for locator class for OpenThread objects.
 */

#ifndef LOCATOR_HPP_
#define LOCATOR_HPP_

#include "openthread-core-config.h"

#include <openthread/platform/toolchain.h>

#include "utils/wrap_stdint.h"

namespace ot {

class Instance;

#if !OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
extern uint64_t gInstanceRaw[];
#endif

/**
 * @addtogroup core-locator
 *
 * @brief
 *   This module includes definitions for OpenThread instance locator.
 *
 * @{
 *
 */

/**
 * This class implements a locator for an OpenThread Instance object.
 *
 * The `InstanceLocator` is used as base class of almost all other OpenThread classes. It provides a way for an object
 * to get to its owning/parent OpenThread `Instance` and also any other `Type` within the `Instance` member variable
 * property hierarchy (using `Get<Type>()` method).
 *
 * If multiple-instance feature is supported, the owning/parent OpenThread `Instance` is tracked as a reference. In the
 * single-instance case, this class becomes an empty base class.
 *
 */
class InstanceLocator
{
public:
    /**
     * This method returns a reference to the parent OpenThread Instance.
     *
     * @returns A reference to the parent otInstance.
     *
     */
#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
    Instance &GetInstance(void) const { return mInstance; }
#else
    Instance &GetInstance(void) const { return *reinterpret_cast<Instance *>(&gInstanceRaw); }
#endif

    /**
     * This template method returns a reference to a given `Type` object belonging to the OpenThread instance.
     *
     * For example, `Get<MeshForwarder>()` returns a reference to the `MeshForwarder` object of the instance.
     *
     * Note that any `Type` for which the `Get<Type>` is defined MUST be uniquely accessible from the OpenThread
     * `Instance` through the member variable property hierarchy.
     *
     * @returns A reference to the `Type` object of the instance.
     *
     */
    template <typename Type> inline Type &Get(void) const; // Implemented in `locator-getters.hpp`.

protected:
    /**
     * This constructor initializes the object.
     *
     * @param[in]  aInstance  A pointer to the otInstance.
     *
     */
    explicit InstanceLocator(Instance &aInstance)
#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
        : mInstance(aInstance)
#endif
    {
        OT_UNUSED_VARIABLE(aInstance);
    }

#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
private:
    Instance &mInstance;
#endif
};

/**
 * This class implements a locator for owner of an object.
 *
 * This is used as the base class for objects that provide a callback (e.g., `Timer` or `Tasklet`).
 *
 */
class OwnerLocator
{
public:
    /**
     * This template method returns a reference to the owner object.
     *
     * The caller needs to provide the `OwnerType` as part of the template type.
     *
     * @returns A reference to the owner of this object.
     *
     */
    template <typename OwnerType> OwnerType &GetOwner(void)
#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
    {
        return *static_cast<OwnerType *>(mOwner);
    }
#else
    // Implemented in `locator-getters.hpp`
    ;
#endif

protected:
    /**
     * This constructor initializes the object.
     *
     * @param[in]  aOwner   A pointer to the owner object (as `void *`).
     *
     */
    explicit OwnerLocator(void *aOwner)
#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
        : mOwner(aOwner)
#endif
    {
        OT_UNUSED_VARIABLE(aOwner);
    }

#if OPENTHREAD_ENABLE_MULTIPLE_INSTANCES
private:
    void *mOwner;
#endif
};

/**
 * @}
 *
 */

} // namespace ot

#endif // LOCATOR_HPP_