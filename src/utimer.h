// This is a univeral timer template for embedded systems
// by Longjia
// hxlich@gmail.com
// 
#ifndef UTIMER_H
#define UTIMER_H

#include <stdint.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif 
#define LIMITED_V		1

#define X_MACRO()	
	






enum{
	UTIMER_ID_TIEMR_1 = 2001,
	UTIMER_ID_TIEMR_2 = 2002,
	UTIMER_ID_TIEMR_3 = 2003,
};

#define UTIMER_INSTANCE_TABLE_SIZE		10

enum{	
	UTIMER_NO_ERROR = 0,
	// regular error code
	UTIMER_RET_BUFFER_FULL,
	UTIMER_RET_OUT_RANGE,
	
	// callback error code
	UTIMER_CB_INVALID_ID,
	// add new return values here

	UTIMER_RET_VALUE_MAX
};


// modify the interrupt stop function to your system
#define ENTER_CRIT		(void)0
#define LEAVE_CRIT		(void)0

// the data typedef for timer
typedef uint32_t		utimer_t;
typedef uint8_t		    utimer_bool_t;

// modify to the boolean values in your system
#define UTIMER_TRUE		(utimer_bool_t)1
#define UTIMER_FALSE	(utimer_bool_t)0


// the max ticks( maybe time )
#define MAX_TICK		ULONG_MAX


enum{
	UTIMER_TYPE_PERIODICAL,
	UTIMER_TYPE_TIME_OUT,
	UTIMER_TYPE_ALL_TICK,
	UTIMER_TYPE_EVENT_TIMER
};

typedef int ret_t;

typedef ret_t ( *timeout_cb_f )( uint32_t, uint32_t );

typedef struct{
	utimer_t		timer_id;
	utimer_t		timestart;
	utimer_t		timeout;
	timeout_cb_f	timeout_cb;
	uint8_t			type;
}utimer_config_t;


ret_t utimer_int( void );

ret_t utimer_reg( uint32_t id, uint32_t timeout, timeout_cb_f timeout_cb, uint8_t type );

ret_t utimer_unreg( uint32_t id );

ret_t utimer_handler( void );

ret_t utimer_reset( void );

ret_t utimer_destroy( void );

ret_t utimer_test( void );

#ifdef __cplusplus
}  /* extern "C" */
#endif 
#endif//UTIMER_H