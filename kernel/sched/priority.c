#include "lugh.h"

// JPL Rule 13: Limited scope for data and functions
// SEI CERT DCL30-C: Declare objects with appropriate storage duration

extern scheduler_ops_t rr_scheduler;

int priority_schedule(task_t* tasks, int num_tasks, uint32_t* next_task_id){
    // JPL Rule 15: Validate parameters at start of public functions
    // SEI CERT ARR30-C: Validate all array indices (not shown, but should check tasks array)
    // placeholder: Priority based scheduling
    // Find the highest priority READY task
    int best_idx = -1;
    int best_priority = 11; // Lower is higher priority (0 is highest)
    for (int i = 0; i < num_tasks; i++) {
        if (tasks[i].state == TASK_READY && tasks[i].priority < best_priority) {
            best_priority = tasks[i].priority;
            best_idx = i;
        }
    }
    if (best_idx >= 0 && next_task_id) {
        *next_task_id = tasks[best_idx].task_id;
        return 0;
    }
    return -1;
}

scheduler_ops_t priority_scheduler;

void priority_scheduler_init(void) {
    priority_scheduler.name = "priority";
    priority_scheduler.init = rr_scheduler.init;
    priority_scheduler.schedule = priority_schedule;
    priority_scheduler.add_task = rr_scheduler.add_task;
    priority_scheduler.remove_task = rr_scheduler.remove_task;
    priority_scheduler.get_state = rr_scheduler.get_state;
    priority_scheduler.set_state = rr_scheduler.set_state;
    priority_scheduler.prepare_swap = rr_scheduler.prepare_swap;
    priority_scheduler.finalize_swap = rr_scheduler.finalize_swap;
}