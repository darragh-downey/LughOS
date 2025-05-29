#include "lugh.h"

int priority_schedule(task_t* tasks, int num_tasks, uint32_t* next_task_id){
    // placeholder: Priority based scheduling
    return -1;
}

scheduler_ops_t priority_scheduler = {
    .name = "priority",
    .init = rr_init,
    .schedule = priority_schedule,
    .add_task = rr_add_task,
    .remove_task = rr_remove_task,
    .get_state = rr_get_state,
    .set_state = rr_set_state,
    .prepare_swap = rr_prepare_swap,
    .finalize_swap = rr_finalize_swap,
}