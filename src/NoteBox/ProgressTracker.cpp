//
// Created by robertvokac on 4/26/25.
//

#include "NoteBox/Core/ProgressTracker.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

namespace NoteBox {
    std::string ProgressTracker::getProgressBar() {
        std::string progressBar = "[";
        int percentProgress = static_cast<int>(getProgress() * 10);

        for (int i = 1; i <= 10; ++i) {
            progressBar += (i <= percentProgress) ? "#" : " ";
        }

        progressBar += "]";
        return progressBar;
    }

    long ProgressTracker::getElapsedSecondsSinceStart() {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    }

    long ProgressTracker::getElapsedMillisecondsSinceStart() {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    }

    long ProgressTracker::getRemainingSecondsUntilEnd() {
        if (done == 0) return 0;
        double secondsPerTask = static_cast<double>(getElapsedSecondsSinceStart()) / done;

        long remainsCount = total - done;
        return static_cast<long>(secondsPerTask * remainsCount);
    }

    ProgressTracker::ProgressTracker(int total) : total(total), done(0) {
        start();
    }

    void ProgressTracker::start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void ProgressTracker::nextDone() {
        if (done < total) {
            ++done;
        } else {
            std::cerr << "done is greater than total: done=" << done << ", total=" << total << std::endl;
        }

    }

    double ProgressTracker::getProgress() const {
        return static_cast<double>(done) / total;
    }

    std::string ProgressTracker::getProgressAsPrettyString() {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << (getProgress() * 100) << "%";
        return oss.str();
    }

    std::string ProgressTracker::currentStatus() {
        std::ostringstream oss;
        oss << "Done " << done << "/" << total << " " << getProgressAsPrettyString()
                << " " << getProgressBar();
        return oss.str();
    }
    ;
}
