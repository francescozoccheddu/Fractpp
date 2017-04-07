#ifndef FPP_TIME_HPP
#define FPP_TIME_HPP


namespace FPP {

    namespace Time {

        typedef long long ttime;

        ttime now();

        double elapsed(ttime from);

        double elapsed(ttime from, ttime to);

    }

}

#endif
