#include <time.hpp>
#include <time.h>

namespace FPP {

    namespace Time {

        ttime now() {
            struct timespec res;
            clock_gettime(CLOCK_MONOTONIC, &res);
            return res.tv_sec * 1000000000 + res.tv_nsec;
        }

        double elapsed(ttime from) {
            return elapsed(from, now());
        }

        double elapsed(ttime from, ttime to) {
            return (to - from) / 1000000000.0;
        }

    }
}