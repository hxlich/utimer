#include <stdio.h>

#include "utimer.h"
#include "test_timer.h"


int main( void ){
	init_timers();
	for(;;){
		utimer_test();
		utimer_handler();
	}
	return 0;
}