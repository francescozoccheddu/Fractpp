#include <async/tasker.hpp>

#define FPP_LOG_TAG "Async::Tasker"

namespace FPP {

    namespace Async {

        Tasker::Tasker(Task *task) : task(task) {}

        Tasker::~Tasker() {}

        void Tasker::start() {
            thread.create(run, reinterpret_cast<void *>(task));
        }

        void Tasker::join() {
            thread.join();
        }

        Task *Tasker::getTask() {
            return task;
        }

        void *Tasker::run(void *arg) {
            Task *task = reinterpret_cast<Task *>(arg);
            task->onPerform();
            return 0;
        }

        void *LoopTasker::run(void *arg) {
            LoopTasker &loopTasker = *reinterpret_cast<LoopTasker *>(arg);
            Mutex &mutex = loopTasker.mutex;
            Cond &cond = loopTasker.cond;
            LoopTask &task = *loopTasker.task;
            if (!task.onStart()) {
                bool looping = true;
                while (looping) {
                    mutex.lock();
                    if (!loopTasker.running) {
                        task.onStop();
                        looping = false;
                    } else {
                        if (loopTasker.paused) {
                            if (task.onPause())
                                looping = false;
                            else {
                                while (loopTasker.paused)
                                    cond.wait(mutex);
                                if (task.onResume())
                                    looping = false;
                            }
                        }
                        if (looping && task.onLoop())
                            looping = false;
                    }
                    mutex.unlock();
                }
            }
            return 0;
        }

        LoopTasker::LoopTasker(LoopTask *task)
                : task(task) {
        }

        LoopTasker::~LoopTasker() {}

        void LoopTasker::start() {
            running = true;
            paused = false;
            thread.create(run, reinterpret_cast<void *>(this));
        }

        void LoopTasker::stop() {
            mutex.lock();
            if (paused) {
                paused = false;
                cond.signal();
            }
            running = false;
            mutex.unlock();
            thread.join();
        }

        void LoopTasker::pause() {
            mutex.lock();
            paused = true;
            mutex.unlock();
        }

        void LoopTasker::resume() {
            mutex.lock();
            paused = false;
            cond.signal();
            mutex.unlock();
        }

    }

}
