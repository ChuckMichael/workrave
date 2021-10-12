// Copyright (C) 2001 - 2010 Rob Caelers <robc@krandor.nl>
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

#ifndef TIMER_HH
#define TIMER_HH

#include <ctime>
#include <string>
#include <list>

#include "IActivityMonitor.hh"
#include "utils/Diagnostics.hh"

class TimePred;

enum TimerState
{
  STATE_INVALID,
  STATE_RUNNING,
  STATE_STOPPED
};

inline std::ostream &
operator<<(std::ostream &stream, TimerState state)
{
  switch (state)
    {
    case STATE_INVALID:
      stream << "invalid";
      break;
    case STATE_RUNNING:
      stream << "running";
      break;
    case STATE_STOPPED:
      stream << "stopped";
      break;
    }
  return stream;
}

enum InsensitiveMode
{
  INSENSITIVE_MODE_FOLLOW_IDLE,
  INSENSITIVE_MODE_IDLE_ON_LIMIT_REACHED,
  INSENSITIVE_MODE_IDLE_ALWAYS
};

inline std::ostream &
operator<<(std::ostream &stream, InsensitiveMode mode)
{
  switch (mode)
    {
    case INSENSITIVE_MODE_FOLLOW_IDLE:
      stream << "follow";
      break;
    case INSENSITIVE_MODE_IDLE_ON_LIMIT_REACHED:
      stream << "idle";
      break;
    case INSENSITIVE_MODE_IDLE_ALWAYS:
      stream << "always";
      break;
    }
  return stream;
}

//! Event generated by the timer.
enum TimerEvent
{
  //! No event occurred.
  TIMER_EVENT_NONE,

  //! The timer was reset back to 0 after the limit was reached.
  TIMER_EVENT_RESET,

  //! The timer was reset back to 0 before the limit was reached.
  TIMER_EVENT_NATURAL_RESET,

  //! The timer reached its limit.
  TIMER_EVENT_LIMIT_REACHED,
};

//! Status information of a timer.
struct TimerInfo
{
  //! Is this timer enabled?
  bool enabled;

  //! Event the timer generated.
  TimerEvent event;

  //! Total idle time of the timer.
  int64_t idle_time;

  //! Total elasped time of the timer.
  int64_t elapsed_time;
};

//! The Timer class.
/*!
 *  The Timer receives 'active' and 'idle' events from an activity monitor.
 *  Based on these events, the timer will start or stop the clock.
 *
 */
class Timer
{
public:
  struct TimerStateData
  {
    int64_t current_time;
    int64_t elapsed_time;
    int64_t elapsed_idle_time;
    int64_t last_pred_reset_time;
    int64_t total_overdue_time;
    int64_t last_limit_time;
    int64_t last_limit_elapsed;
    bool snooze_inhibited;
  };

public:
  using Ptr = std::shared_ptr<Timer>;

  // Construction/Destruction.
  Timer(const std::string &timer_id);
  virtual ~Timer();

  // Control
  void enable();
  void disable();
  void snooze_timer();
  void freeze_timer(bool f);

  // Timer processing.
  void process(ActivityState activityState, TimerInfo &info);

  // State inquiry
  int64_t get_elapsed_time() const;
  int64_t get_elapsed_idle_time() const;
  TimerState get_state() const;
  bool is_enabled() const;

  // Auto-resetting.
  void set_auto_reset(int t);
  void set_auto_reset(TimePred *predicate);
  void set_auto_reset(std::string predicate);
  void set_auto_reset_enabled(bool b);
  bool is_auto_reset_enabled() const;
  int64_t get_auto_reset() const;
  TimePred *get_auto_reset_predicate() const;
  int64_t get_next_reset_time() const;

  // Limiting.
  void set_limit(int t);
  void set_limit_enabled(bool b);
  bool is_limit_enabled() const;
  int64_t get_limit() const;
  int64_t get_next_limit_time() const;

  // Timer ID
  std::string get_id() const;

  // State serialization.
  std::string serialize_state() const;
  bool deserialize_state(const std::string &state, int version);
  void set_state(int elapsed, int idle, int overdue = -1);

  void set_state_data(const TimerStateData &data);
  void get_state_data(TimerStateData &data);
  void set_values(int64_t elapsed, int64_t idle);

  // Misc
  void set_snooze(int64_t time);
  int64_t get_snooze() const;
  void set_snooze_interval(int64_t time);
  void inhibit_snooze();
  void set_activity_monitor(IActivityMonitor *am);
  IActivityMonitor *get_activity_monitor() const;
  bool has_activity_monitor() const;

  int64_t get_total_overdue_time() const;
  void daily_reset_timer();

  void shift_time(int delta);

  void start_timer();
  void stop_timer();
  void reset_timer();

  bool get_activity_sensitive();
  void set_activity_sensitive(bool a);
  void force_idle();
  void force_active();
  void set_insensitive_mode(InsensitiveMode mode);
  bool is_running() const;

private:
  //! Id of the timer.
  std::string timer_id;

  //! Is this timer enabled ?
  TracedField<bool> timer_enabled;

  //! Is the timer frozen? A frozen timer only counts idle time.
  TracedField<bool> timer_frozen;

  //! State of the state monitor.
  TracedField<ActivityState> activity_state;

  //! State of the timer.
  TracedField<TimerState> timer_state;

  //! Default snooze time
  int64_t snooze_interval{60};

  //! Snooze on active time instead of on actual time.
  bool snooze_on_active{true};

  //! Don't snooze til next reset or changes.
  bool snooze_inhibited{false};

  //! Is the timer limit enabled?
  bool limit_enabled{true};

  //! Timer limit interval.
  int64_t limit_interval{600};

  //! Is the timer auto reset enabled?
  bool autoreset_enabled{true};

  //! Automatic reset time interval.
  int64_t autoreset_interval{120};

  //! Auto reset time predicate. (or NULL if not used)
  TimePred *autoreset_interval_predicate{nullptr};

  //! Elapsed time.
  int64_t elapsed_time{0};

  //! Elapsed Idle time.
  int64_t elapsed_idle_time{0};

  //! Last time the limit was reached.
  int64_t last_limit_time{0};

  //! The total elapsed time the last time the limit was reached.
  int64_t last_limit_elapsed{0};

  //! Time when the timer was last started.
  int64_t last_start_time{0};

  //! Time when the timer was last reset.
  int64_t last_reset_time{0};

  //! Time when the timer was last stopped.
  int64_t last_stop_time{0};

  //! Next automatic reset time.
  int64_t next_reset_time{0};

  //! Time when the timer was last reset because of a predicate.
  int64_t last_pred_reset_time{0};

  //! Next automatic predicate reset time.
  int64_t next_pred_reset_time{0};

  //! Next limit time.
  int64_t next_limit_time{0};

  //! Total overdue time.
  int64_t total_overdue_time{0};

  //! Activity Mobnitor to use.
  IActivityMonitor *activity_monitor{nullptr};

  //!  Is this timer sensitive for activity
  TracedField<bool> activity_sensitive;

  //!
  TracedField<InsensitiveMode> insensitive_mode;

private:
  void compute_next_limit_time();
  void compute_next_reset_time();
  void compute_next_predicate_reset_time();
};

#include "Timer.icc"

#endif // TIMER_HH
