#ifndef FPP_COORDINATOR_HPP
#define FPP_COORDINATOR_HPP

#include <jni.h>

namespace FPP {

    class Coordinator {

    private:

    public:

        Coordinator(jobject bridge);

        virtual ~Coordinator();

        void destroy();

        void setSurface(jobject surface);

    };

    /*class Coordinator {

    private:

        const jobject jniGame;
        pthread_t rendThread;
        pthread_mutex_t rendMutex;
        pthread_t phThread;
        pthread_mutex_t phMutex;
        bool rendWindowChanged;
        bool rendCancelled;
        bool phCancelled;
        ANativeWindow *window;

        static void *rendThreadCallback(void *arg);

        static void *phThreadCallback(void *arg);

    public:

        Coordinator(jobject jniGame);

        virtual ~Coordinator();

        void stop();

        void destroy();

        void setWindow(ANativeWindow *window);

    };*/
}

#endif
