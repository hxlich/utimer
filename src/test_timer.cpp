#include <stdio.h>
#include "test_timer.h"



void init_timers( void ){
	ret_t ret;
	utimer_int();

	ret = utimer_reg(UTIMER_ID_TIEMR_1, 10, timer_1_cb, UTIMER_TYPE_PERIODICAL);
	if( UTIMER_NO_ERROR != ret ){
		printf("\nTimer init error %d", ret);
		return;
	}

	ret = utimer_reg(UTIMER_ID_TIEMR_2, 20, timer_2_cb, UTIMER_TYPE_TIME_OUT);
	if( UTIMER_NO_ERROR != ret ){
		printf("\nTimer init error %d", ret);
		return;
	}

	ret = utimer_reg(UTIMER_ID_TIEMR_3, 30, timer_3_cb, UTIMER_TYPE_PERIODICAL);
	if( UTIMER_NO_ERROR != ret ){
		printf("\nTimer init error %d", ret);
		return;
	}
}






static ret_t timer_1_cb( uint32_t id, uint32_t timeout ){
	return printf("%s called, id = %u, timeout = %u\n",__FUNCTION__, id, timeout);
}

static ret_t timer_2_cb( uint32_t id, uint32_t timeout ){
	return printf("%s called, id = %u, timeout = %u\n",__FUNCTION__, id, timeout);
}

static ret_t timer_3_cb( uint32_t id, uint32_t timeout ){
	return printf("%s called, id = %u, timeout = %u\n",__FUNCTION__, id, timeout);
}