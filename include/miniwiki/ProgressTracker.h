//
// Created by robertvokac on 4/26/25.
//

#ifndef PROGRESSTRACKER_H
#define PROGRESSTRACKER_H

#include <chrono>

#include "ProgressTracker.h"

#include <iomanip>
#include <string>

namespace MiniWiki {
    class ProgressTracker {
    private:
        std::chrono::high_resolution_clock::time_point startTime;
        long total;
        long done;

    public:
        ProgressTracker(int total);

        std::string getProgressBar();

        void start();

        void nextDone();

        double getProgress() const;

        std::string getProgressAsPrettyString();

        std::string currentStatus();

        long getElapsedSecondsSinceStart();
        long getElapsedMillisecondsSinceStart();
        long getRemainingSecondsUntilEnd();
    };
}


#endif //PROGRESSTRACKER_H
