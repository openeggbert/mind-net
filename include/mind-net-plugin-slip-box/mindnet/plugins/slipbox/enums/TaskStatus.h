#ifndef TASKSTATUS_H
#define TASKSTATUS_H
#include <string>

#include "mindnet/model/EnumDefinition.h"

enum class TaskStatus
{
    OPEN = 0,
    IN_PROGRESS = 1,
    BLOCKED = 2,
    PAUSED = 3,
    DONE = 4,
    CANCELLED = 5,
    FAILED = 6
};

inline std::string task_status_to_string(TaskStatus status)
{
    switch (status)
    {
    case TaskStatus::OPEN:
        return "OPEN";
    case TaskStatus::IN_PROGRESS:
        return "IN_PROGRESS";
    case TaskStatus::BLOCKED:
        return "BLOCKED";
    case TaskStatus::PAUSED:
        return "PAUSED";
    case TaskStatus::DONE:
        return "DONE";
    case TaskStatus::CANCELLED:
        return "CANCELLED";
    case TaskStatus::FAILED:
        return "FAILED";
    default:
        return "Unknown";
    }
}

inline std::string task_status_to_string(int status)
{
    return task_status_to_string(static_cast<TaskStatus>(status));
}

inline mindnet::model::EnumDefinition task_status_to_enum_definition()
{
    return mindnet::model::EnumDefinition{
        task_status_to_string, 7, 0, 1, 2, 3, 4, 5, 6
    };
}
#endif // TASKSTATUS_H
