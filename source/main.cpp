// Copyright [year] <Copyright Owner
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

const int number_philosophers = 5;

class DiningPhilosophers;

int get_random(int low, int high) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(low, high);
  return distribution(gen);
}

class Waitress {
 public:
  Waitress();
  virtual ~Waitress();
};

struct Philosopher {
  Philosopher(unsigned int id, std::string name, unsigned int number_actions,
              DiningPhilosophers* table)
      : id_(id), name_(name), number_actions_(number_actions), table_(table) {}

  void operator()() {
    for (unsigned int i = 0; i < number_actions_; ++i) {
      if (get_random(1, 2) < 2) {
        Think();
      } else {
        Eat();
      }
    }
  }

  void Think();
  void Eat();

  unsigned int id_;
  std::string name_;
  unsigned int number_actions_;
  DiningPhilosophers* table_;
};

class DiningPhilosophers {
 public:
  DiningPhilosophers() {
    std::thread t1(Philosopher(0, "Socrates", 10, this));
    std::thread t2(Philosopher(1, "Plato", 10, this));
    std::thread t3(Philosopher(2, "Aristotle", 10, this));
    std::thread t4(Philosopher(3, "Descartes", 10, this));
    std::thread t5(Philosopher(4, "Tomas", 10, this));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
  }

  void PickFork(unsigned int id) {
    forks_[id].lock();
    forks_[(id + 1) % number_philosophers].lock();
  }

  void LeaveFork(unsigned int id) {
    forks_[id].unlock();
    forks_[(id + 1) % number_philosophers].unlock();
  }

 private:
  std::mutex forks_[number_philosophers];
};

void Philosopher::Think() {
  std::cout << name_ + " is Thinking." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(get_random(1, 6)));
}

void Philosopher::Eat() {
  table_->PickFork(id_);
  std::cout << name_ + " is Eating" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(get_random(1, 6)));
  table_->LeaveFork(id_);
}

int main() {
  DiningPhilosophers table;
  return 0;
}
