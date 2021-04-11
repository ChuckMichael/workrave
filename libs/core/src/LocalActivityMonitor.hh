// ActivityMonitor.hh --- ActivityMonitor functionality
//
// Copyright (C) 2001, 2002, 2003, 2004, 2006, 2007, 2010, 2013 Rob Caelers <robc@krandor.nl>
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

#ifndef ACTIVITYMONITOR_HH
#define ACTIVITYMONITOR_HH

#include "IActivityMonitor.hh"
#include "input-monitor/IInputMonitor.hh"
#include "input-monitor/IInputMonitorListener.hh"
#include "utils/Mutex.hh"

#include "utils/Diagnostics.hh"

class ActivityListener;
class IInputMonitor;

class ActivityMonitor
  : public workrave::input_monitor::IInputMonitorListener
  , public IActivityMonitor
{
public:
  ActivityMonitor();
  ~ActivityMonitor() override;

  void terminate() override;
  void suspend() override;
  void resume() override;
  void force_idle() override;
  void shift_time(int delta);

  ActivityState get_current_state() override;

  void set_parameters(int noise, int activity, int idle, int sensitivity);
  void get_parameters(int &noise, int &activity, int &idle, int &sensitivity);

  void set_listener(ActivityMonitorListener *l) override;

  void action_notify() override;
  void mouse_notify(int x, int y, int wheel = 0) override;
  void button_notify(bool is_press) override;
  void keyboard_notify(bool repeat) override;

private:
  void call_listener();

private:
  //! The actual monitoring driver.
  workrave::input_monitor::IInputMonitor::Ptr input_monitor;

  //! the current state.
  TracedField<ActivityState> activity_state{"monitor.activity_state", ACTIVITY_IDLE, true};

  //! Internal locking
  Mutex lock;

  //! Previous X coordinate
  int prev_x{-10};

  //! Previous Y coordinate
  int prev_y{-10};

  //! Is the button currently pressed?
  bool button_is_pressed{false};

  //! Last time activity was detected
  gint64 last_action_time{0};

  //! First time the \c ACTIVITY_IDLE state was left.
  gint64 first_action_time{0};

  //! The noise threshold
  TracedField<gint64> noise_threshold{"monitor.noise_threshold", 0};

  //! The activity threshold.
  TracedField<gint64> activity_threshold{"monitor.activity_threshold", 0};

  //! The idle threshold.
  TracedField<gint64> idle_threshold{"monitor.idle_threshold", 0};

  //! Mouse sensitivity
  TracedField<int> sensitivity{"monitor.sensitivity", 3};

  //! Activity listener.
  ActivityMonitorListener *listener{nullptr};
};

#endif // ACTIVITYMONITOR_HH
