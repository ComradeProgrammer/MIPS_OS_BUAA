#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion statrting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
extern struct Env_list env_sched_list[2];
extern int cur_sched;
int remaining_time;


void sched_yield(void)
{
	struct Env* e;
	remaining_time--;
//	printf("sched_yield!\n");

	if(remaining_time<=0){
		e=LIST_FIRST(&(env_sched_list[cur_sched]))	;
		if (e==NULL){
			cur_sched=1-cur_sched;
			e=LIST_FIRST(&(env_sched_list[cur_sched]))	;
			if(e==NULL){
				printf("no more envs:dead loop\n");
				for(;;);
				return;
			}

		} 
		remaining_time=e->env_pri;
		LIST_REMOVE(e,env_sched_link);
		LIST_INSERT_HEAD(&(env_sched_list[1-cur_sched]),e,env_sched_link)	;	
	}
	else{
		e=curenv;
	}
	env_run(e);
	//printf("sched_yield:to%x\n",e->env_id);

}

/*
void sched_yield(void)
{
	static int count = 0;
	while (1){
		count = (count+1)%NENV;
		if (envs[count].env_status==ENV_RUNNABLE) {
			env_run(envs+count);
			break;
		} 
	}

}*/

