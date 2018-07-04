#include <boost/mpl/list.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/in_state_reaction.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <iostream>

using namespace std;
namespace sc = boost::statechart;
namespace mpl = boost::mpl;

struct event_pos_abs : sc::event<event_pos_abs> {};

struct event_jog_pos : sc::event<event_jog_pos> {};

struct event_homing : sc::event<event_homing> {};

struct event_reset : sc::event<event_reset> {};

struct event_jog_neg : sc::event<event_jog_neg> {};

struct event_move_velocity : sc::event<event_move_velocity> {};

struct event_pos_rel : sc::event<event_jog_neg> {};

struct standstill;

class S120 : public sc::state_machine<S120, standstill> {
 public:
  S120() {}
  ~S120() {}

  auto homing() { this->process_event(event_homing()); }
  auto reset_error() { return this->process_event(event_reset()); }
  auto move_jog_pos() { return this->process_event(event_jog_pos()); }
  auto move_jog_neg() { return this->process_event(event_jog_neg()); }
  auto move_velocity() { return this->process_event(event_move_velocity()); }
  auto pos_absolute() { return this->process_event(event_pos_abs()); }
  auto pos_relative() { return this->process_event(event_pos_rel()); }
};

struct operational;
struct standstill : sc::simple_state<standstill, S120> {
  standstill() { cout << "standstill::ctor\n"; }
  ~standstill() { cout << "standstill::dtor\n"; }
  typedef sc::transition<event_pos_abs, operational> reactions;
};

struct operational : sc::simple_state<operational, S120> {
  operational() { cout << "operational::ctor\n"; }
  ~operational() { cout << "operational::dtor\n"; }
  typedef mpl::list<sc::custom_reaction<event_pos_abs>,
		    sc::transition<event_jog_pos, standstill>>
      reactions;
  sc::result react(const event_pos_abs &) { cout << "operational::react()\n"; }
};

int main(int argc, char *argv[]) {
  S120 my_drive;
  my_drive.initiate();
  my_drive.move_jog_pos();
  my_drive.pos_absolute();
  my_drive.pos_absolute();
  return 0;
}
