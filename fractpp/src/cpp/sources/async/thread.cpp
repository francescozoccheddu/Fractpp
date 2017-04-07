#include <debug.hpp>
#include <async/thread.hpp>

#define FPP_LOG_TAG "Async::Thread"

namespace FPP {

    namespace Async {

        void Thread::create(void *(*func)(void *), void *arg) {
            int res = pthread_create(&thread, 0, func, arg);
            FPP_ERR_IF(res, "Unable to start thread (code %d)", res);
        }

        void *Thread::join() {
            void *returnValue = nullptr;
            int res = pthread_join(thread, &returnValue);
            FPP_ERR_IF(res, "Unable to join thread (code %d)", res);
            return returnValue;
        }

        Thread::~Thread() {}

    }

}