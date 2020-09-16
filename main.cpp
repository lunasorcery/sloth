#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>
#include <thread>

struct mistake_type {
  enum mistake_mode {
    good,      // no mistake
    mistaking, // currently we're making a mistake
    fixing     // we're fixing the mistake
  };
  mistake_mode type = mistake_mode::good;
  std::size_t mistake_long = 0;
  std::size_t index = 0;
  std::uniform_int_distribution<std::mt19937::result_type> mistake_range{1, 10};
  const double mistake_chance = 0.1; // 1% chance of mistake

private:
  std::uniform_int_distribution<std::mt19937::result_type> mistake_change_dist{
      0, 100};

public:
  void start_making_mistake(std::size_t how_much) {
    type = mistake_type::mistaking;
    mistake_long = how_much;
    index = mistake_long;
  }

  void reset_index() { index = mistake_long; }

  void tick(std::mt19937& gen) {
    switch (type) {
    case mistaking:
      if (index == 0) { // let's go into fixing mode
        type = fixing;
        reset_index();
      } else {
        index--;
      }
      break;
    case fixing:
      if (index == 0) { // let's go the good mode
        type = good;
      } else {
        index--;
      }
      break;

    case good: {
      auto rand_percent = mistake_change_dist(gen);
      if (rand_percent <= mistake_chance) { // let's make a mistake
        start_making_mistake(mistake_range(gen));
      }
      break;
    }
    }
  }
};

struct duration_generator {
  using dur_t = std::chrono::duration<uint64_t, std::milli>;

private:
  std::mt19937 gen;
  std::uniform_int_distribution<std::mt19937::result_type> range{10, 300};
  std::uniform_int_distribution<std::mt19937::result_type> char_range{'a', 'z'};
  const dur_t base_interval;
  dur_t cur_interval;
  mistake_type mistake;

public:
  duration_generator(decltype(base_interval) base_interval)
      : gen(std::random_device{}()), base_interval(base_interval) {}

  static duration_generator parse(int argc, char **argv) {
    using namespace std::chrono;

    for (int i = 1; i < argc; ++i) {
      char *end;
      uint64_t const value = strtol(argv[i], &end, 10);

      if (value == 0)
        continue;

      if (strcmp(end, "") == 0 || strcmp(end, "ms") == 0)
        return duration_generator{milliseconds(value)};
      else if (strcmp(end, "s") == 0)
        return duration_generator{seconds(value)};
      else if (strcmp(end, "m") == 0)
        return duration_generator{minutes(value)};
      else if (strcmp(end, "h") == 0)
        return duration_generator{hours(value)};
    }

    return duration_generator{milliseconds(10)};
  }

  void next() {
    auto rand_val = range(gen);
    cur_interval = base_interval + dur_t{rand_val};
    mistake.tick(gen);
  }

  dur_t interval() const noexcept { return cur_interval; }

  int get_char() {
    switch (mistake.type) {
    case mistake_type::mistaking: {
      // let's generate a random mistake
      return char_range(gen);
    }

    case mistake_type::fixing: {
      return '\b';
    }

    case mistake_type::good: {
      return getchar();
    }
    }
    return getchar();
  }
};

int main(int argc, char **argv) {
  auto randomizer = duration_generator::parse(argc, argv);

  bool isInEscapeCode = false;
  for (;;) {
    int c = randomizer.get_char();
    if (c == -1)
      break;
    if (isInEscapeCode) {
      if (c != '[' && c >= 0x40 && c <= 0x7e) {
        isInEscapeCode = false;
      }
    } else if (c == 0x1b) {
      isInEscapeCode = true;
    } else {
      std::this_thread::sleep_for(randomizer.interval());
    }

    putchar(c);
    fflush(stdout);
    randomizer.next();
  }
}
