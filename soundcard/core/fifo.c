///*
 //* fifo.c
 //*
 //* Created: 27.02.2015 17:29:32
 //*  Author: ale
 //*/ 
//
//#include "fifo.h"
////#include "module/uart.h"
//#include <string.h>
//
//void fifo_init(fifo_t *fifo, T_fifo *buffer, uint32_t size) {
	//fifo->_in = 0;
	//fifo->_out = 0;
	//fifo->_lock = 0;
	//fifo->_avalable = 0;
	//fifo->_size = size;
	//fifo->buffer = buffer;
//}
//
//uint32_t fifo_put(fifo_t *fifo, T_fifo *arry, uint32_t size) {
	//if(!__fifo_lock(fifo)) return 0;
	//
	//uint32_t s = (size < (fifo->_size - fifo->_in)) ? (size) : (fifo->_size - fifo->_in);
	//memcpy( fifo->buffer + fifo->_in, arry, s * sizeof(T_fifo));
	//
	//fifo->_in += s;
	//if(fifo->_in >= fifo->_size) fifo->_in = 0;
	//
	//fifo->_avalable += s;
	//if(fifo->_avalable > fifo->_size) {
		//fifo->_avalable = fifo->_size;
		//fifo->_out = fifo->_in;
	//}
	//
	//__fifo_unlock(fifo);
	//return s;
//}
//
//uint32_t fifo_get(fifo_t *fifo, T_fifo *arry, uint32_t size) {
	//if(!__fifo_lock(fifo)) return 0;
//
////	uint32_t s = (size < (fifo->_size - fifo->_in)) ? (size) : (fifo->_size - fifo->_in);
////	memcpy( arry, fifo->buffer + fifo->_in, s * sizeof(T_fifo));
//
	//__fifo_unlock(fifo);
//
	//return 0;
//}
//
//void fifo_push(fifo_t *fifo, T_fifo val) {
	//while(__fifo_is_locking(fifo));
	//__fifo_lock(fifo);
	//if(++fifo->_in == fifo->_size) fifo->_in = 0;
	//*(fifo->buffer + fifo->_in) = val;
	//if(++fifo->_avalable >= fifo->_size) {
		//fifo->_avalable = fifo->_size;
		//fifo->_out++;
	//}
	//__fifo_unlock(fifo);
//}
//
//uint8_t fifo_pull(fifo_t *fifo, T_fifo *val) {
	//while(__fifo_is_locking(fifo));
	//if((fifo->_in < 5000) || (fifo->_avalable == 0) ) return 0;
	//__fifo_lock(fifo);
	//if(++fifo->_out == fifo->_size) fifo->_out = 0;
	//*val = *(fifo->buffer + fifo->_out);
	//fifo->_avalable--;
	//__fifo_unlock(fifo);
	//return 1;
//}
//
//uint8_t __fifo_lock(fifo_t *fifo) {
	//return (fifo->_lock) ? 0 : (fifo->_lock = 1);	
//}
//
//void __fifo_unlock(fifo_t *fifo) {
	//fifo->_lock = 0;
//}
//
//uint8_t __fifo_is_locking(fifo_t *fifo) {
	//return fifo->_lock;
//}