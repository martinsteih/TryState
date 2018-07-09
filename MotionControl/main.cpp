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
struct event_pos_abs : sc::event<event_pos_abs> {
  explicit event_pos_abs(double position) : _position{position} {}
  double get_position() const { return _position; }

 private:
  double _position;
};

struct event_jog_pos : sc::event<event_jog_pos> {};

struct event_homing : sc::event<event_homing> {};

struct event_reset : sc::event<event_reset> {};

struct event_jog_neg : sc::event<event_jog_neg> {};

struct event_move_velocity : sc::event<event_move_velocity> {};

struct event_pos_rel : sc::event<event_jog_neg> {};

struct standstill;

class Drive {
 public:
  virtual void homing() = 0;
  virtual void reset_error() = 0;
  virtual void move_jog_pos() = 0;
  virtual void move_jog_neg() = 0;
  virtual void move_velocity() = 0;
  virtual void pos_relative() = 0;
  virtual void pos_absolute(double) = 0;
};

//*********************State Machine*************************
class S120 : public Drive, public sc::state_machine<S120, standstill> {
 public:
  S120() { this->initiate(); }
  ~S120() {}

  void homing() override { this->process_event(event_homing()); }
  void reset_error() override { return this->process_event(event_reset()); }
  void move_jog_pos() override { return this->process_event(event_jog_pos()); }
  void move_jog_neg() override { return this->process_event(event_jog_neg()); }
  void move_velocity() override {
    return this->process_event(event_move_velocity());
  }
  void pos_absolute(double position) override {
    return this->process_event(event_pos_abs(position));
  }
  void pos_relative() override { return this->process_event(event_pos_rel()); }
};

//*********************State definitions*************************
struct operational;
struct standstill : sc::simple_state<standstill, S120> {
  standstill() { cout << "standstill::ctor\n"; }
  ~standstill() { cout << "standstill::dtor\n"; }
  typedef sc::transition<event_pos_abs, operational> reactions;
};

struct operational : sc::state<operational, S120> {
  operational(my_context context) : my_base(context) {
    if (auto triggering(
	    dynamic_cast<const event_pos_abs *>(triggering_event()));
	triggering) {
      cout << "operational::ctor\n";
      cout << "moving to pos " << triggering->get_position() << endl;
    }
  }
  ~operational() { cout << "operational::dtor\n"; }
  typedef mpl::list<sc::custom_reaction<event_pos_abs>,
		    sc::transition<event_jog_pos, standstill>>
      reactions;
  sc::result react(const event_pos_abs &e) {
    cout << "operational::react(" << e.get_position() << ")\n";
  }
};

class DriveFactory {
 public:
  virtual unique_ptr<Drive> getDrive() const = 0;
};

class S120Factory : public DriveFactory {
 public:
  unique_ptr<Drive> getDrive() const override { return make_unique<S120>(); }
};

class Straightening {
 public:
  Straightening(DriveFactory &&drive_factory)
      : my_drive{drive_factory.getDrive()} {
    my_drive->homing();
    my_drive->pos_absolute(211.54);
    my_drive->pos_absolute(-11.5);
  }

 private:
  unique_ptr<Drive> my_drive;
};

//*********************Event definitions*************************
int main(int argc, char *argv[]) {
  Straightening str(move(S120Factory()));
  return 0;
}
