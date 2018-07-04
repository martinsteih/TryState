#include <boost/mpl/list.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <iostream>

using namespace std;
namespace sc = boost::statechart;
namespace mpl = boost::mpl;

//*********************Event definitions*************************
struct event_pos_abs : sc::event<event_pos_abs> {};
struct event_jog_pos : sc::event<event_jog_pos> {};
struct event_homing : sc::event<event_homing> {};
struct event_reset : sc::event<event_reset> {};
struct event_jog_neg : sc::event<event_jog_neg> {};
struct event_move_velocity : sc::event<event_move_velocity> {};
struct event_pos_rel : sc::event<event_pos_rel> {};

//*********************State Machine*************************
struct standstill;
class S120 : public sc::state_machine<S120, standstill> {
 public:
  S120() { this->initiate(); }
  ~S120() {}

  auto homing() { this->process_event(event_homing()); }
  auto reset_error() { return this->process_event(event_reset()); }
  auto move_jog_pos() { return this->process_event(event_jog_pos()); }
  auto move_jog_neg() { return this->process_event(event_jog_neg()); }
  auto move_velocity() { return this->process_event(event_move_velocity()); }
  auto pos_absolute() { return this->process_event(event_pos_abs()); }
  auto pos_relative() { return this->process_event(event_pos_rel()); }
};

//*********************State definitions*************************

struct idle;
struct standstill : sc::simple_state<standstill, S120, idle> {
  standstill() { cout << "standstill::ctor\n"; }
  ~standstill() { cout << "standstill::dtor\n"; }
};

struct homing;
struct move_jog;
struct pos_abs;
struct pos_rel;
struct move_velo;

struct operational : sc::simple_state<operational, S120, move_jog> {
  operational() { cout << "operational::ctor\n"; }
  ~operational() { cout << "operational::dtor\n"; }
};

struct idle : sc::simple_state<idle, standstill> {
  idle() { cout << "idle::ctor\n"; }
  ~idle() { cout << "idle::dtor\n"; }
  typedef mpl::list<sc::transition<event_homing, homing>,
		    sc::transition<event_jog_pos, move_jog>,
		    sc::transition<event_jog_neg, move_jog>,
		    sc::transition<event_pos_abs, pos_abs>,
		    sc::transition<event_pos_rel, pos_rel>,
		    sc::transition<event_move_velocity, move_velo>>
      reactions;
};

struct reset_error : sc::simple_state<reset_error, standstill> {
  reset_error() { cout << "reset_error::ctor\n"; }
  ~reset_error() { cout << "reset_error::dtor\n"; }
};

struct homing : sc::simple_state<homing, operational> {
  homing() { cout << "homing::ctor\n"; }
  ~homing() { cout << "homing::dtor\n"; }
};

struct move_jog : sc::simple_state<move_jog, operational> {
  move_jog() {
    cout << "move_jog::ctor "
	 << "\n";
  }
  ~move_jog() { cout << "move_jog::dtor\n"; }
};

struct pos_abs : sc::simple_state<pos_abs, operational> {
  pos_abs() { cout << "pos_abs::ctor\n"; }
  ~pos_abs() { cout << "pos_abs::dtor\n"; }
};

struct pos_rel : sc::simple_state<pos_rel, operational> {
  pos_rel() { cout << "pos_rel::ctor\n"; }
  ~pos_rel() { cout << "pos_rel::dtor\n"; }
};

struct move_velo : sc::simple_state<move_velo, operational> {
  move_velo() { cout << "move_velo::ctor\n"; }
  ~move_velo() { cout << "move_velo::dtor\n"; }
};

//*********************Event definitions*************************
int main(int argc, char *argv[]) {
  S120 my_drive;
  my_drive.pos_absolute();
  return 0;
}
