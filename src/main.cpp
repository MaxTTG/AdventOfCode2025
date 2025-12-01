#include <cstdlib>
#include <iostream>

#define DECLARE_DAY(n) \
    namespace day##n { \
        void solve();  \
    }

DECLARE_DAY(01)

void run_day(int day) {
    switch (day) {
    case 1:
        day01::solve();
        break;
    default:
        std::cerr << "Day " << day << " is not implemented yet." << std::endl;
        std::exit(1);
    }
}

int main(int argc, char *argv[]) {
    int day = 1;
    run_day(day);
    return 0;
}
