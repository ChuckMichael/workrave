// InputMonitorFactory.hh --- Factory to create input monitors.
//
// Copyright (C) 2007 Rob Caelers <robc@krandor.nl>
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef INPUTMONITORFACTORY_HH
#define INPUTMONITORFACTORY_HH

#include <cstdlib>
#include <string>

#include "IInputMonitorFactory.hh"

//! Factory to create input monitors.
class InputMonitorFactory
{
public:
  static void init(const char *display);
  static IInputMonitor *get_monitor(IInputMonitorFactory::MonitorCapability capability);

private:
  static IInputMonitorFactory *factory;
};

#endif // INPUTMONITORFACTORY_HH
