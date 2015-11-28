/**
 * \file
 * \brief DynamicThreadParameters class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-28
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICTHREADPARAMETERS_HPP_
#define INCLUDE_DISTORTOS_DYNAMICTHREADPARAMETERS_HPP_

#include "distortos/SchedulingPolicy.hpp"

#include <cstddef>

namespace distortos
{

/**
 * \brief DynamicThreadParameters struct is a helper with parameters for DynamicThread's constructor
 *
 * This struct is a replacement for overloads of DynamicThread's constructor and makeDynamicThread() which -
 * unfortunately - cannot be used, as they would lead to compilation errors due to ambiguity.
 *
 * \ingroup threads
 */

struct DynamicThreadParameters
{
	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] canReceiveSignalss selects whether reception of signals is enabled (true) or disabled (false) for
	 * this thread
	 * \param [in] queuedSignalss is the max number of queued signals for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
	 * \param [in] signalActionss is the max number of different SignalAction objects for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicyy is the scheduling policy of the thread
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const bool canReceiveSignalss,
			const size_t queuedSignalss, const size_t signalActionss, const uint8_t priorityy,
			const SchedulingPolicy schedulingPolicyy) :
			queuedSignals{queuedSignalss},
			signalActions{signalActionss},
			stackSize{stackSizee},
			canReceiveSignals{canReceiveSignalss},
			priority{priorityy},
			schedulingPolicy{schedulingPolicyy}
	{

	}

	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] canReceiveSignalss selects whether reception of signals is enabled (true) or disabled (false) for
	 * this thread
	 * \param [in] queuedSignalss is the max number of queued signals for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
	 * \param [in] signalActionss is the max number of different SignalAction objects for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const bool canReceiveSignalss,
			const size_t queuedSignalss, const size_t signalActionss, const uint8_t priorityy) :
			queuedSignals{queuedSignalss},
			signalActions{signalActionss},
			stackSize{stackSizee},
			canReceiveSignals{canReceiveSignalss},
			priority{priorityy},
			schedulingPolicy{SchedulingPolicy::RoundRobin}
	{

	}

	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicyy is the scheduling policy of the thread
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const uint8_t priorityy,
			const SchedulingPolicy schedulingPolicyy) :
			queuedSignals{},
			signalActions{},
			stackSize{stackSizee},
			canReceiveSignals{},
			priority{priorityy},
			schedulingPolicy{schedulingPolicyy}
	{

	}

	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const uint8_t priorityy) :
			queuedSignals{},
			signalActions{},
			stackSize{stackSizee},
			canReceiveSignals{},
			priority{priorityy},
			schedulingPolicy{SchedulingPolicy::RoundRobin}
	{

	}

	/// max number of queued signals for this thread, relevant only if \a canReceiveSignals == true, 0 to disable
	/// queuing of signals for this thread
	size_t queuedSignals;

	/// max number of different SignalAction objects for this thread, relevant only if \a canReceiveSignals == true, 0
	/// to disable catching of signals for this thread
	size_t signalActions;

	/// size of stack, bytes
	size_t stackSize;

	/// selects whether reception of signals is enabled (true) or disabled (false) for this thread
	bool canReceiveSignals;

	/// thread's priority, 0 - lowest, UINT8_MAX - highest
	uint8_t priority;

	/// scheduling policy of the thread
	SchedulingPolicy schedulingPolicy;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICTHREADPARAMETERS_HPP_
