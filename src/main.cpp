#include <cstdlib>
#include <iostream>

#define DECLARE_DAY(n) \
    namespace day##n { \
        void solve();  \
    }

DECLARE_DAY(01)
DECLARE_DAY(02)
DECLARE_DAY(03)
DECLARE_DAY(04)
DECLARE_DAY(05)
DECLARE_DAY(06)
DECLARE_DAY(07)
DECLARE_DAY(08)
DECLARE_DAY(09)
// DECLARE_DAY(10)
DECLARE_DAY(11)

void run_day(int day) {
    std::cout << "Day " << day << std::endl;
    switch (day) {
    case 1:
        day01::solve();
        break;
    case 2:
        day02::solve();
        break;
    case 3:
        day03::solve();
        break;
    case 4:
        day04::solve();
        break;
    case 5:
        day05::solve();
        break;
    case 6:
        day06::solve();
        break;
    case 7:
        day07::solve();
        break;
    case 8:
        day08::solve();
        break;
    case 9:
        day09::solve();
        break;
    // case 10:
    //     day10::solve();
    //     break;
    case 11:
        day11::solve();
        break;
    default:
        std::cerr << "Day " << day << " is not implemented yet." << std::endl;
        std::exit(1);
    }
}

int main(int argc, char *argv[]) {
    int day = 11;
    run_day(day);
    return 0;
}
