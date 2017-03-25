#
# file: printers.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

from PrettyPrinters.estd.registerEstdPrettyPrinters import registerEstdPrettyPrinters

def registerPrettyPrinters(obj):
	'Register pretty-printers from this tree'

	registerEstdPrettyPrinters(obj)
