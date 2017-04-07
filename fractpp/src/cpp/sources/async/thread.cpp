#include <async/thread.hpp>
#include <debug.hpp>

#define FPP_LOG_TAG "Async::Thread"

namespace FPP {

    namespace Async {

        template<typename Arg>
        static void ptf_create(pthread_t &thread, void *(*func)(void *), Arg arg) {
            int res = pthread_create(&thread, 0, func, reinterpret_cast<void *>(arg));
            FPP_ERR_IF(res, "Unable to start thread (code %d)", res);
        }

        static void ptf_join(pthread_t &thread) {
            int res = pthread_join(thread, 0);
            FPP_ERR_IF(res, "Unable to join thread (code %d)", res);
        }

        Thread::Thread(Task *task) : task(task) {}

        Thread::~Thread() {}

        void Thread::start() {
            ptf_create(thread, run, task);
        }

        void Thread::join() {
            ptf_join(thread);
        }

        Task *Thread::getTask() {
            return task;
        }

        void *Thread::run(void *arg) {
            Task *task = reinterpret_cast<Task *>(arg);
            task->onPerform();
            return 0;
        }

        void *LoopThread::run(void *arg) {
            LoopThread &loopThread = *reinterpret_cast<LoopThread *>(arg);
            LoopTask &task = *loopThread.task;
            if (!task.onStart()) {
                bool running = true;
                while (running) {
                    loopThread.mutex.lock();
                    if (!loopThread.running) {
                        task.onStop();
                        running = false;
                    } else {
                        if (loopThread.paused) {
                            if (task.onPause())
                                running = false;
                            else {
                                while (loopThread.paused)
                                    loopThread.cond.wait(loopThread.mutex);
                                if (task.onResume())
                                    running = false;
                            }
                        }
                        if (running && task.onLoop())
                            running = false;
                    }
                    loopThread.mutex.unlock();
                }
            }
            return 0;
        }

        LoopThread::LoopThread(LoopTask *task)
                : task(task) {
        }

        LoopThread::~LoopThread() {}

        void LoopThread::start() {
            running = true;
            paused = false;
            ptf_create(thread, run, this);
        }

        void LoopThread::stop() {
            mutex.lock();
            if (paused) {
                paused = false;
                cond.signal();
            }
            running = false;
            mutex.unlock();
            ptf_join(thread);
        }

        void LoopThread::pause() {
            mutex.lock();
            paused = true;
            mutex.unlock();
        }

        void LoopThread::resume() {
            mutex.lock();
            paused = false;
            cond.signal();
            mutex.unlock();
        }

    }

}
