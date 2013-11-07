#include "utimer.h"



void init_timers( void );
static ret_t timer_1_cb( uint32_t id, uint32_t timeout );
static ret_t timer_2_cb( uint32_t id, uint32_t timeout );
static ret_t timer_3_cb( uint32_t id, uint32_t timeout );