/**
 * \file
 * \brief ThreadSleepUntilTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-02
 */

#include "ThreadSleepUntilTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/ThisThread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {384};

/// offset added to sleepUntil time points - there is some time between configuring thread and actually starting it...
constexpr uint8_t timePointOffset {10};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(TickClock::time_point sleepUntil, SequenceAsserter& sequenceAsserter, unsigned int sequencePoint,
		TickClock::duration& timePointDeviation, int& sharedRet);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, thread, std::declval<TickClock::time_point>(),
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>(),
		std::ref(std::declval<TickClock::duration&>()), std::ref(std::declval<int&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Sleeps until requested time point and marks the sequence point in SequenceAsserter, also storing the deviation of
 * time point of waking up.
 *
 * \param [in] sleepUntil is the time point at which the thread will wake up to mark sequence point
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 * \param [out] timePointDeviation is a reference to variable for storing deviation of time point of waking up
 * \param [out] sharedRet is a reference to variable for storing return value of ThisThread::sleepUntil()
 */

void thread(const TickClock::time_point sleepUntil, SequenceAsserter& sequenceAsserter,
		const unsigned int sequencePoint, TickClock::duration& timePointDeviation, int& sharedRet)
{
	sharedRet = ThisThread::sleepUntil(sleepUntil);

	timePointDeviation = TickClock::now() - sleepUntil;
	sequenceAsserter.sequencePoint(sequencePoint);
}

/**
 * \brief Builder of TestThread objects.
 *
 * This test uses "priority" field of test parameters as a "reversed" amount of time to sleep.
 *
 * \param [in] now is the current time point
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [out] timePointDeviation is a reference to variable for storing deviation of time point of waking up
 * \param [out] sharedRet is a reference to variable for storing return value of ThisThread::sleepUntil()
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const TickClock::time_point now, const ThreadParameters& threadParameters,
		SequenceAsserter& sequenceAsserter, TickClock::duration& timePointDeviation, int& sharedRet)
{
	return makeStaticThread<testThreadStackSize>(UINT8_MAX, thread,
			now + TickClock::duration{UINT8_MAX - threadParameters.first + timePointOffset}, std::ref(sequenceAsserter),
			static_cast<unsigned int>(threadParameters.second), std::ref(timePointDeviation), std::ref(sharedRet));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadSleepUntilTestCase::run_() const
{
	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;
		std::array<TickClock::duration, totalThreads> timePointDeviations {{}};
		std::array<int, totalThreads> sharedRets {{}};
		const auto now = TickClock::now();

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(now, phase.first[phase.second[0]], sequenceAsserter, timePointDeviations[0],
						sharedRets[0]),
				makeTestThread(now, phase.first[phase.second[1]], sequenceAsserter, timePointDeviations[1],
						sharedRets[1]),
				makeTestThread(now, phase.first[phase.second[2]], sequenceAsserter, timePointDeviations[2],
						sharedRets[2]),
				makeTestThread(now, phase.first[phase.second[3]], sequenceAsserter, timePointDeviations[3],
						sharedRets[3]),
				makeTestThread(now, phase.first[phase.second[4]], sequenceAsserter, timePointDeviations[4],
						sharedRets[4]),
				makeTestThread(now, phase.first[phase.second[5]], sequenceAsserter, timePointDeviations[5],
						sharedRets[5]),
				makeTestThread(now, phase.first[phase.second[6]], sequenceAsserter, timePointDeviations[6],
						sharedRets[6]),
				makeTestThread(now, phase.first[phase.second[7]], sequenceAsserter, timePointDeviations[7],
						sharedRets[7]),
				makeTestThread(now, phase.first[phase.second[8]], sequenceAsserter, timePointDeviations[8],
						sharedRets[8]),
				makeTestThread(now, phase.first[phase.second[9]], sequenceAsserter, timePointDeviations[9],
						sharedRets[9]),
		}};

		{
			architecture::InterruptMaskingLock interruptMaskingLock;

			for (auto& thread : threads)
				thread.start();
		}

		for (auto& thread : threads)
			thread.join();

		if (sequenceAsserter.assertSequence(totalThreads) == false)
			return false;

		for (const auto& timePointDeviation : timePointDeviations)
			if (timePointDeviation != TickClock::duration{})
				return false;

		for (const auto& sharedRet : sharedRets)
			if (sharedRet != 0)
				return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
