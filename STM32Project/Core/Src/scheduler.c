/** Idea :
 * Setup timer interrupt 10 ms.
 * Implement an array of scheduler instances with the
	expectation of achieving O (1) complexity in the
	SCH_Update function .
 * If there is a task to be executed , the delay of that
	task is calculated after the previous task is executed .
 * = > In function SCH_Update , we only need implement task
	at index = 0; SCH_Update is called in function
	HAL_TIM_PeriodElapsedCallback () every 10 ms.
 * In function SCH_Add_Task , we find the satisfying
	locations to add the new task and recalculate time delay
	of all task in the scheduler .
 * With this idea , function SCH_Dispatch_Tasks () only need
	check RunMe of the first task in the scheduler .
*/

# include "scheduler.h"
# include "main.h"
// Declare variable count is the current number of tasks in the scheduler
int count = 0;
void SCH_Init ( void ) {
	unsigned char i ;
	for ( i = 0; i < SCH_MAX_TASKS ; i ++) {
		// Delete all tasks
		SCH_Delete_Task ( i) ;
	}
}

void SCH_Update ( void ) {
	// Check if there is a task at this location
	if ( SCH_tasks_G [0]. pTask ) {
		if ( SCH_tasks_G [0]. Delay == 0) {
			// The task is due to run
			// Inc. the ’RunMe ’ flag
			SCH_tasks_G [0]. RunMe += 1;
			if ( SCH_tasks_G [0]. Period ) {
				// Schedule periodic tasks to run again
				SCH_tasks_G [0]. Delay = SCH_tasks_G [0]. Period ;
			}
		} else {
			// Not yet ready to run : just decrement the delay
			SCH_tasks_G [0]. Delay -= 1;
		}
	}
}

unsigned char SCH_Add_Task ( void (* pFunction ) () , unsigned int DELAY , unsigned int PERIOD ){
	unsigned char Index = 0;
	if ( count == 0)
	{
		// Add task at the index = 0 if the scheduler is empty
		SCH_tasks_G [0]. pTask = pFunction ;
		SCH_tasks_G [0]. Delay = DELAY ;
		SCH_tasks_G [0]. Period = PERIOD ;
		SCH_tasks_G [0]. RunMe = 0;
		count ++;
	}
	else if ( count > 0)
	{
		// Find satisfying locations to add new task
		while (( Index < count ) && count < SCH_MAX_TASKS )
		{
			if ( SCH_tasks_G [ Index ]. Delay > DELAY )
			{
				for ( int i = count ; i > Index ; i--)
				{
					// Shift tasks
					SCH_tasks_G [i ] = SCH_tasks_G [i-1];
				}
				SCH_tasks_G [ Index ]. pTask = pFunction ;
				SCH_tasks_G [ Index ]. Delay = DELAY ;
				SCH_tasks_G [ Index ]. Period = PERIOD ;
				SCH_tasks_G [ Index ]. RunMe = 0;
				count ++;
				for ( int j = Index + 1; j < count ; j ++) {
					SCH_tasks_G [j ]. Delay -= DELAY ;
				}
				break ;
			}
			else {
				DELAY -= SCH_tasks_G [ Index ]. Delay ;
			}
			Index++;
		}
		// If we ’re in the last of scheduler and scheduler is not full = > Insert new task here
		if( Index == count && ( count + 1 < SCH_MAX_TASKS ) ) {
			SCH_tasks_G [ Index ]. pTask = pFunction ;
			SCH_tasks_G [ Index ]. Delay = DELAY ;
			SCH_tasks_G [ Index ]. Period = PERIOD ;
			SCH_tasks_G [ Index ]. RunMe = 0;
			count ++;
		}
	}
	// Return position of task (to allow later deletion )
	return Index ;
}

void SCH_Dispatch_Tasks ( void )
{
	// Dispatches ( runs ) the next task (if one is ready )
	if ( SCH_tasks_G [0]. RunMe > 0) {
		(* SCH_tasks_G [0]. pTask ) () ; // Run the task
		SCH_tasks_G [0]. RunMe -= 1; // Reset / reduce RunMe flag
		// Periodic tasks will automatically run again
		// - if this is a ’one shot ’ task , remove it from the array
		if ( SCH_tasks_G [0]. Period == 0)
		{
			SCH_Delete_Task (0) ;
		}
		else
		{
			sTask temp = SCH_tasks_G [0];
			SCH_Delete_Task (0) ;
			SCH_Shift_Task (0) ;
			SCH_Add_Task ( temp . pTask , temp . Delay , temp . Period ) ;
		}
	}
}

unsigned char SCH_Delete_Task ( const int TASK_INDEX ) {
	unsigned char Return_code ;
	Return_code = 0;
	SCH_tasks_G [ TASK_INDEX ]. pTask = 0x0000 ;
	SCH_tasks_G [ TASK_INDEX ]. Delay = 0;
	SCH_tasks_G [ TASK_INDEX ]. Period = 0;
	SCH_tasks_G [ TASK_INDEX ]. RunMe = 0;
	return Return_code ; // return status
}

void SCH_Shift_Task ( const int TASK_INDEX ) {
	if ( TASK_INDEX != count -1 ) {
		for ( int i = TASK_INDEX ; i < count -1; i ++) {
			SCH_tasks_G [i ] = SCH_tasks_G [ i +1];
		}
		SCH_tasks_G [ count -1]. pTask = 0x0000 ;
		SCH_tasks_G [ count -1]. Delay = 0;
		SCH_tasks_G [ count -1]. Period = 0;
		SCH_tasks_G [ count -1]. RunMe = 0;
	}
	count--;
}

