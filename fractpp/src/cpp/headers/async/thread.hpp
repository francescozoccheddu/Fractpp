#ifndef FPP_ASYNC_THREAD_HPP
#define FPP_ASYNC_THREAD_HPP

#include <pthread.h>
#include <async/task.hpp>
#include <async/sync.hpp>

namespace FPP {

    namespace Async {

        class Thread {

        private:

            Task *task;
            pthread_t thread;

            static void *run(void *arg);

        public:

            Thread(Task *task);

            virtual ~Thread();

            void start();

            void join();

            Task *getTask();

        };

        class LoopThread {

        private:

            LoopTask *task;
            pthread_t thread;
            bool running, paused;
            Mutex mutex;
            Cond cond;

            static void *run(void *arg);

        public:

            LoopThread(LoopTask *task);

            virtual ~LoopThread();

            void start();

            void stop();

            void pause();

            void resume();

            LoopTask *getTask();

        };

    }

}

#endif
