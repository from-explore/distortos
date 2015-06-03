/**
 * \file
 * \brief FpuThreadTestCase class implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-31
 */

#include "ARMv7-M-FpuThreadTestCase.hpp"

#include "distortos/chip/CMSIS-proxy.h"

#if __FPU_PRESENT == 1 && __FPU_USED == 1

#include "checkFpuRegisters.hpp"
#include "setFpuRegisters.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

namespace distortos
{

namespace test
{

#if __FPU_PRESENT == 1 && __FPU_USED == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// number of iterations of sequence in test thread
constexpr uint32_t sequenceIterations {10};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/// number of test threads
constexpr size_t totalThreads {10};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread function
using TestThreadFunction = void(uint32_t, bool&);

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, std::declval<TestThreadFunction>(),
		std::declval<uint32_t>(), std::ref(std::declval<bool&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Saves test value to all FPU registers (including FPSCR), causes context switch to next test thread and - after
 * return - checks whether all modified registers still have the same value. This sequence is repeated
 * \a sequenceIterations times with increasing test value (or until first failure).
 *
 * In case of failure \a sharedResult is set to "false", otherwise it is not modified.
 *
 * \param [in] value is the first test value
 * \param [out] sharedResult is a reference to variable with shared result of the test
 */

void thread(uint32_t value, bool& sharedResult)
{
	for (uint32_t iteration {}; iteration < sequenceIterations && sharedResult == true; ++iteration)
	{
		const auto fpscr = setFpuRegisters(value, true);
		ThisThread::yield();	// cause context switch
		const auto result = checkFpuRegisters(value, fpscr);
		if (result == false)
			sharedResult = result;

		value++;
	}
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] value is the first test value
 * \param [out] sharedResult is a reference to variable with shared result of the test
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const uint32_t value, bool& sharedResult)
{
	return makeStaticThread<testThreadStackSize>(1, thread, static_cast<uint32_t>(value), std::ref(sharedResult));
}

}	// namespace

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FpuThreadTestCase::run_() const
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	bool sharedResult {true};
	std::array<TestThread, totalThreads> threads
	{{
			makeTestThread(0x3c6f0686, sharedResult),
			makeTestThread(0x42f94e89, sharedResult),
			makeTestThread(0x77f24bc2, sharedResult),
			makeTestThread(0x2924b4c4, sharedResult),
			makeTestThread(0xcbd6c082, sharedResult),
			makeTestThread(0x59c00e8a, sharedResult),
			makeTestThread(0x20fb443b, sharedResult),
			makeTestThread(0x7421f59b, sharedResult),
			makeTestThread(0x509d19ca, sharedResult),
			makeTestThread(0xc25198ec, sharedResult),
	}};

	for (auto& thread : threads)
		thread.start();

	for (auto& thread : threads)
		thread.join();

	constexpr auto totalContextSwitches = totalThreads * sequenceIterations + 2 * totalThreads;
	if (statistics::getContextSwitchCount() - contextSwitchCount != totalContextSwitches)
		return false;

	return sharedResult;

#else

	return true;

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

}	// namespace test

}	// namespace distortos