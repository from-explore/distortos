/**
 * \file
 * \brief SchedulingPolicy enum class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-28
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULINGPOLICY_HPP_
#define INCLUDE_DISTORTOS_SCHEDULINGPOLICY_HPP_

#include <cstdint>

namespace distortos
{

/**
 * \brief scheduling policy of the thread
 *
 * \ingroup threads
 */

enum class SchedulingPolicy : uint8_t
{
	/// FIFO scheduling policy
	Fifo,
	/// round-robin scheduling policy
	RoundRobin,
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULINGPOLICY_HPP_
