#ifndef FPP_ASYNC_THREAD_HPP
#define FPP_ASYNC_THREAD_HPP

#include <pthread.h>

namespace FPP {

    namespace Async {

        class Thread {

        private:

            pthread_t thread;

        public:

            virtual ~Thread();

            void create(void *(*func)(void *), void *arg);

            void *join();

        };

    }

}

#endif
