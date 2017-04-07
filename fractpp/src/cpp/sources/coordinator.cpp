#include <coordinator.hpp>
#include <debug.hpp>
#include <async/task.hpp>
#include <async/tasker.hpp>
#include <unistd.h>


#define FPP_LOG_TAG "Coordinator"

namespace FPP {

    class MyTask : public Async::LoopTask {

    private:

        int i, n;

    public:
        virtual bool onStart() override {
            FPP_LOGD("Task: onStart");
            i = 0;
            n = 0;
            return false;
        }

        virtual bool onLoop() override {
            if (n++ > 10000000) {
                FPP_LOGD("Task: onLoop %d", i);
                n = 0;
                i++;
            }
            return i > 50;
        }

        virtual void onStop() override {
            FPP_LOGD("Task: onStop");
        }

        virtual bool onPause() override {
            FPP_LOGD("Task: onPause");
            return false;
        }

        virtual bool onResume() override {
            FPP_LOGD("Task: onResume");
            i = 45;
            return false;
        }

    };

    Coordinator::Coordinator(jobject bridge) {
        MyTask *task = new MyTask();

        Async::LoopTasker thread(task);
        FPP_LOGD("Starting thread");
        thread.start();
        sleep(2);
        FPP_LOGD("Pausing thread");
        thread.pause();
        sleep(2);
        FPP_LOGD("Resuming thread");
        thread.resume();
        sleep(2);
        FPP_LOGD("Stopping thread");
        thread.stop();
        FPP_LOGD("Stopped thread");
        /*
        Async::Tasker thread(task);
        thread.start();
        thread.join();*/

        /* Async::CustomThread<int> thread;
         int b = 18;
         thread.create([](int *a) {
             for (int i = 0; i < 10; i++) {
                 FPP_LOGI("Thread is working! (a=%d)", (*a)++);
             }
         }, &b);
         thread.join();
         FPP_LOGI("Thread exited (a=%d)", b);*/
    }

    Coordinator::~Coordinator() {

    }

    void Coordinator::destroy() {

    }

    void Coordinator::setSurface(jobject surface) {

    }

}


/*#include <Matz/coordinator.hpp>
#include <Matz/log.hpp>
#include <exception>
#include <Matz/renderer.hpp>
#include <Matz/phworld.hpp>

#define MATZ_LOG_TAG "Coordinator"

namespace Matz {


    Coordinator::Coordinator(jobject jniGame)
            : jniGame(jniGame), rendWindowChanged(false), rendCancelled(false), window(nullptr),
              phCancelled(false) {
        pthread_mutex_init(&rendMutex, 0);
        pthread_create(&rendThread, 0, rendThreadCallback, this);
        pthread_mutex_init(&phMutex, 0);
        pthread_create(&phThread, 0, phThreadCallback, this);
    }

    void Coordinator::stop() {

    }

    void Coordinator::setWindow(ANativeWindow *window) {
        pthread_mutex_lock(&rendMutex);
        if (this->window)
            ANativeWindow_release(this->window);
        rendWindowChanged = true;
        this->window = window;
        pthread_mutex_unlock(&rendMutex);
    }

    void Coordinator::destroy() {
        pthread_mutex_lock(&rendMutex);
        rendCancelled = true;
        if (window)
            ANativeWindow_release(window);
        window = nullptr;
        rendWindowChanged = true;
        pthread_mutex_unlock(&rendMutex);
        pthread_join(rendThread, 0);
    }

    Coordinator::~Coordinator() {
        pthread_mutex_destroy(&rendMutex);
    }

    void *Coordinator::rendThreadCallback(void *arg) {
        LOGD("Rendering thread started");
        Coordinator *coord = static_cast<Coordinator *> (arg);
        Renderer renderer(5000, 5000, 10);
        RenderClient client(renderer);
        PhWorld world;
        Bubble &b = world.addBubble(PositiveBubbleDef(0));
        bool rendering = true;
        int t = 0;
        while (rendering) {
            pthread_mutex_lock(&coord->rendMutex);
            if (coord->rendCancelled)
                rendering = false;
            else {
                if (coord->rendWindowChanged) {
                    coord->rendWindowChanged = false;
                    try {
                        renderer.setWindow(coord->window);
                        world.setSize(renderer.getWidth(), renderer.getHeight());
                    }
                    catch (std::exception &exception) {
                        LOGE("Exception while setting window: '%s'", exception.what());
                        throw;
                    }
                    LOGD("Rendering thread changed window");
                }
                if (coord->window) {
                    world.update();
                    world.flush();
                    b.render(client);
                    renderer.flush();
                }
            }
            pthread_mutex_unlock(&coord->rendMutex);
        }
        renderer.destroy();
        LOGD("Rendering thread returned");
        return 0;
    }

    void *Coordinator::phThreadCallback(void *arg) {
        LOGD("Physics thread started");
        LOGD("Physics thread returned");
        return 0;
    }

}*/