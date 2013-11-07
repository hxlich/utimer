#include <stddef.h>
#include <stdio.h>

#include "utimer.h"

#if LIMITED_V
	static volatile utimer_config_t utimer_config_table[UTIMER_INSTANCE_TABLE_SIZE]={0};
#else
	static volatile utimer_config_t * utimer_config_table = NULL;
#endif

static volatile utimer_t s_utimer_tick = 0;
static volatile uint8_t s_reg_timer_count = 0;
static ret_t utimer_set_tick( uint32_t new_tick){
	
	if( new_tick > MAX_TICK){
		return UTIMER_RET_OUT_RANGE;
	}
	
	ENTER_CRIT;
	s_utimer_tick = new_tick;
	LEAVE_CRIT;

	return UTIMER_NO_ERROR;
}

static void utimer_clr_timer( uint8_t index ){
	if( index > UTIMER_INSTANCE_TABLE_SIZE){
		// Assert()
	}
	utimer_config_table[index].timer_id = 0;
}



ret_t utimer_int( void ){
	uint8_t i;
	for( i=0 ; i<UTIMER_INSTANCE_TABLE_SIZE; i++){
		if( 0 != utimer_config_table[i].timer_id ){
			ENTER_CRIT;
			s_reg_timer_count += 1;
			LEAVE_CRIT;
		}
	}

	return UTIMER_NO_ERROR;
}

ret_t utimer_reg( uint32_t id, uint32_t timeout, timeout_cb_f timeout_cb, uint8_t type ){
//ret_t utimer_reg( utimer_config_t * new_config){
#if LIMITED_V
	uint8_t i;

	if( UTIMER_INSTANCE_TABLE_SIZE == s_reg_timer_count ){
		return UTIMER_RET_BUFFER_FULL;
	}

	for(i=0;i< UTIMER_INSTANCE_TABLE_SIZE ; i++){
		if( 0 == utimer_config_table[i].timer_id){
			ENTER_CRIT;
			s_reg_timer_count++;
			utimer_config_table[i].timestart = s_utimer_tick;
			utimer_config_table[i].timer_id = id;		// todo check id?
			utimer_config_table[i].timeout = timeout;
			utimer_config_table[i].timeout_cb = timeout_cb;
			utimer_config_table[i].type = type;
			LEAVE_CRIT;
			break;
		}
	}
	return UTIMER_NO_ERROR;
#else
#endif
}

ret_t utimer_unreg( uint32_t id ){
	int i;
	for(i=0;i<UTIMER_INSTANCE_TABLE_SIZE;i++){
		if( id == utimer_config_table[i].timer_id ){
			ENTER_CRIT;
			utimer_config_table[i].timer_id = 0;
			s_reg_timer_count--;
			LEAVE_CRIT;
		}
	}

	
	return UTIMER_NO_ERROR;
}


static utimer_bool_t utimer_periodical_check( utimer_t * start, utimer_t timeout ){
	utimer_t		res;
	utimer_t		current = s_utimer_tick;
	utimer_bool_t	ret = UTIMER_FALSE;

	ENTER_CRIT;
	res = ( current < *start ) ? ( MAX_TICK-current + *start ) : ( current - *start ) ;

	if( res > timeout ){
		*start += timeout; // safe for overflow, since the buffer is 
						   // used as ring buffer
		ret = UTIMER_TRUE;
	}
	LEAVE_CRIT;
	return ret;
}

static utimer_bool_t utimer_timeout_check( utimer_t * start, utimer_t timeout ){
	utimer_t		res;
	utimer_t		current = s_utimer_tick;
	
	ENTER_CRIT;
	res = ( current < *start  ? ( MAX_TICK-current + *start ) : ( current - *start )) ;
	LEAVE_CRIT;
	
	if( res > timeout ){
		return UTIMER_TRUE;
	}

	return UTIMER_FALSE;
}


ret_t utimer_handler( void ){
	int i, count;
	utimer_t current_tick = s_utimer_tick;
	utimer_t *time_start_p;
	utimer_t timeout;
	ret_t ret = UTIMER_NO_ERROR;
	
	for( i=0, count=0 ; i< UTIMER_INSTANCE_TABLE_SIZE && count != s_reg_timer_count; i++){
		ENTER_CRIT;
		time_start_p = ( utimer_t * )&(utimer_config_table[i].timestart);
		timeout    = ( utimer_t )utimer_config_table[i].timeout;
		LEAVE_CRIT;

		if( 0 ==  utimer_config_table[i].timer_id ){
			continue;
		}else{
			count++;
		}

		switch( utimer_config_table[i].type ){
			case UTIMER_TYPE_PERIODICAL:
				if( UTIMER_TRUE == utimer_periodical_check(time_start_p,timeout)){
					ret = utimer_config_table[i].timeout_cb(utimer_config_table[i].timer_id, timeout);
				}
				break;
			case UTIMER_TYPE_TIME_OUT:
				if( UTIMER_TRUE == utimer_timeout_check(time_start_p,timeout)){
					ret = utimer_config_table[i].timeout_cb(utimer_config_table[i].timer_id, timeout);
					utimer_clr_timer(i);
				}
				break;
			case UTIMER_TYPE_ALL_TICK:
				ret = utimer_config_table[i].timeout_cb(utimer_config_table[i].timer_id, timeout);
				break;
			case UTIMER_TYPE_EVENT_TIMER:
				break;
			default:
				break;
		}
	}

	return ret;
}

ret_t utimer_reset( void ){
	ret_t ret;
	ret = utimer_set_tick( 0 );
	if( UTIMER_NO_ERROR != ret ){
		return ret;
	}
	return UTIMER_NO_ERROR;
}

ret_t utimer_destroy( void ){
	return UTIMER_NO_ERROR;
}

// Your timer isr needs to update s_utimer_tick
ret_t utimer_test( void ){
	return ++s_utimer_tick;
}


