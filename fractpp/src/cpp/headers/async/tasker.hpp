#ifndef FPP_ASYNC_TASKER_HPP
#define FPP_ASYNC_TASKER_HPP

#include <pthread.h>
#include <async/task.hpp>
#include <async/sync.hpp>
#include <async/thread.hpp>

namespace FPP {

    namespace Async {

        class Tasker {

        private:

            Task *task;
            Thread thread;

            static void *run(void *arg);

        public:

            Tasker(Task *task);

            virtual ~Tasker();

            void start();

            void join();

            Task *getTask();

        };

        class LoopTasker {

        private:

            LoopTask *task;
            Thread thread;
            bool running, paused;
            Mutex mutex;
            Cond cond;

            static void *run(void *arg);

        public:

            LoopTasker(LoopTask *task);

            virtual ~LoopTasker();

            void start();

            void stop();

            void pause();

            void resume();

            LoopTask *getTask();

        };

    }

}

#endif
