#ifndef FPP_ASYNC_TASK_HPP
#define FPP_ASYNC_TASK_HPP

namespace FPP {

    namespace Async {

        class Task {

        public:

            virtual void onPerform() = 0;

            virtual ~Task();

        };

        class LoopTask : public Task{

        public:

            virtual void onPerform();

            virtual bool onStart();

            virtual bool onPause();

            virtual bool onResume();

            virtual bool onLoop() = 0;

            virtual void onStop();

        };

    }

}

#endif
