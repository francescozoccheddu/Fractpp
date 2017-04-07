#ifndef FPP_ASYNC_SYNC_HPP
#define FPP_ASYNC_SYNC_HPP

#include <pthread.h>

namespace FPP {

    namespace Async {

        class Mutex {

            friend class Cond;

        private:

            pthread_mutex_t mutex;

        public:

            Mutex();

            virtual ~Mutex();

            void lock();

            void unlock();

        };

        class Cond {

        private:

            pthread_cond_t cond;

        public:

            Cond();

            virtual ~Cond();

            void wait(Mutex &mutex);

            void pause();

            void signal();

        };

    }

}

#endif
