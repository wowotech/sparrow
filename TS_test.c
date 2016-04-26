/*
 * TS_test.c
 *
 * A test code for Thread Synchronization
 *
 * Author: wowo<www.wowotech.net>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define DELAY1				(1000)	/* ms */
#define DELAY2				(872)	/* ms */
#define DELAY3				(1093)	/* ms */

/*
 * v_s_wr, a variable will be accessed by write and read
 */
static volatile int			v_s_wr = 0;
/* spinlock is enough */
static						DEFINE_SPINLOCK(s_wr_lock);

static struct task_struct 	*TS_task1, *TS_task2, *TS_task3;

/* TS_thread1, add 1 to v_s_wr */
static int TS_thread1(void *data) {
	while (1) {
		printk("%s: add 1\n", __func__);

		spin_lock(&s_wr_lock);
		v_s_wr++;
		spin_unlock(&s_wr_lock);

		msleep(DELAY1);
	}
 
	return 0;
}

/* TS_thread2, read and print v_s_wr */
static int TS_thread2(void *data) {
	while (1) {
		spin_lock(&s_wr_lock);
		printk("%s: print---%d\n", __func__, v_s_wr);
		spin_unlock(&s_wr_lock);

		msleep(DELAY2);
	}
 
	return 0;
}

/* TS_thread3, dec 1  from v_s_wr */
static int TS_thread3(void *data) {
	while (1) {
		printk("%s: dec 1\n", __func__);

		spin_lock(&s_wr_lock);
		v_s_wr--;
		spin_unlock(&s_wr_lock);


		msleep(DELAY3);
	}
 
	return 0;
}

/*========================================================================
					The global functions provided to others
  =======================================================================*/
void TS_test_stop(void) {
	if (TS_task1) {
		kthread_stop(TS_task1);
		TS_task1 = NULL;
	}

	if (TS_task2) {
		kthread_stop(TS_task2);
		TS_task2 = NULL;
	}

	if (TS_task3) {
		kthread_stop(TS_task3);
		TS_task3 = NULL;
	}
}

int TS_test_start(void) {
	TS_task1 = kthread_create(TS_thread1, NULL, "TS_task1");
	TS_task2 = kthread_create(TS_thread2, NULL, "TS_task2");
	TS_task3 = kthread_create(TS_thread3, NULL, "TS_task3");
 
	if (IS_ERR(TS_task1) || IS_ERR(TS_task2) || IS_ERR(TS_task3)) {
		printk("Unable to start kernel thread.\n");
		TS_test_stop();
      	return -1;
    }
 
	wake_up_process(TS_task1);
	wake_up_process(TS_task2);
	wake_up_process(TS_task3);

	return 0;
}
