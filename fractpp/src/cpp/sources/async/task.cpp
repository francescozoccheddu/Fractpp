#include "async/task.hpp"

namespace FPP {

    namespace Async {

        Task::~Task() {}

        void LoopTask::onPerform() {
            if (!onStart())
                while (!onLoop());
        }

        bool LoopTask::onStart() {
            return false;
        }

        bool LoopTask::onPause() {
            return false;
        }

        bool LoopTask::onResume() {
            return false;
        }

        void LoopTask::onStop() {}

    }

}

