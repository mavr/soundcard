/*
 * IncFile1.h
 *
 * Created: 09.02.2016 16:16:46
 *  Author: ale
 */ 


#ifndef LIST_H_
#define LIST_H_

#define LIST_HEAD_INIT(value) { null }

#define LIST_HEAD(value) \
	struct list_head name = LIST_HEAD_INIT(name)
	
struct list_head {
	struct list_head *next;
};





#endif /* INCFILE1_H_ */