#include "async/sync.hpp"
#include <debug.hpp>

#define FPP_LOG_TAG "Async::Mutex"

namespace FPP {

    namespace Async {

        Mutex::Mutex() {
            int res = pthread_mutex_init(&mutex, 0);
            FPP_ERR_IF(res, "Unable to create mutex (code %d)", res);
        }

        Mutex::~Mutex() {
            int res = pthread_mutex_destroy(&mutex);
            FPP_ERR_IF(res, "Unable to destroy mutex (code %d)", res);
        }

        void Mutex::lock() {
            int res = pthread_mutex_lock(&mutex);
            FPP_ERR_IF(res, "Unable to lock mutex (code %d)", res);
        }

        void Mutex::unlock() {
            int res = pthread_mutex_unlock(&mutex);
            FPP_ERR_IF(res, "Unable to unlock mutex (code %d)", res);
        }

        Cond::Cond() {
            int res = pthread_cond_init(&cond, 0);
            FPP_ERR_IF(res, "Unable to create cond (code %d)", res);
        }

        Cond::~Cond() {
            int res = pthread_cond_destroy(&cond);
            FPP_ERR_IF(res, "Unable to destroy cond (code %d)", res);
        }

        void Cond::wait(Mutex &mutex) {
            int res = pthread_cond_wait(&cond, &mutex.mutex);
            FPP_ERR_IF(res, "Unable to wait for cond (code %d)", res);
        }

        void Cond::signal() {
            int res = pthread_cond_signal(&cond);
            FPP_ERR_IF(res, "Unable to signal cond (code %d)", res);
        }

    }

}