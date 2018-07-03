#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <iostream>

using namespace std;
namespace sc = boost::statechart;

struct event_pos_abs : sc::event<event_pos_abs> {};

struct event_jog_pos : sc::event<event_jog_pos> {};

struct manual_mode;

class S120 : public sc::state_machine<S120, manual_mode> {};

struct auto_mode;
struct manual_mode : sc::simple_state<manual_mode, S120> {
  manual_mode() { cout << "manual_mode::ctor\n"; }
  ~manual_mode() { cout << "manual_mode::dtor\n"; }
  typedef sc::transition<event_pos_abs, auto_mode> reactions;
};

struct auto_mode : sc::simple_state<auto_mode, S120> {
  auto_mode() { cout << "auto_mode::ctor\n"; }
  ~auto_mode() { cout << "auto_mode::dtor\n"; }
  typedef sc::transition<event_jog_pos, manual_mode> reactions;
};

int main(int argc, char *argv[]) {
  S120 my_drive;
  my_drive.initiate();
  my_drive.process_event(event_pos_abs());
  my_drive.process_event(event_jog_pos());
  my_drive.process_event(event_jog_pos());
  return 0;
}
