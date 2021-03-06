/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Scheduler test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS 20

static struct semaphore *tsem = NULL;
static struct semaphore *barrier = NULL;

static
void
init_semaphores(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("schedulertest: tsem sem_create failed\n");
		}
	}

	if (barrier==NULL) {
		barrier = sem_create("barrier", 0);
		if (barrier == NULL) {
			panic("schedulertest: barrier sem_create failed\n");
		}
	}
}


static
void
loop(void *junk, unsigned long id) //Used to demonstrate sch2
{
	volatile int i;
	int num_loops = 200000;
	int ch = '0' + id;
	volatile int* crap = junk;

	P(barrier);
	V(barrier);

	(void)junk;

	for (i=0; i<num_loops; i++) {
		if(i%10000==0){
			if(id > 9){
				ch = 'A' + (id - 10);
				putch(ch);
			}
			else
				putch(ch);
		}
		crap++;
	}
	V(tsem);
}

/* Sch1 Code */

static
void
runthreads()
{
	char name[16];
	int i, result;
	unsigned int ID = 1;
	int pri[5] = {1, 2, 3, 4, 9};

	for (i=1; i<=NTHREADS; i++) {
		snprintf(name, sizeof(name), "thread_%d\n", i);
		result = thread_fork_priority(name, pri[ID], NULL, loop, NULL, ID);
		if (result) {
			panic("schedulertest: thread_fork failed %s)\n", strerror(result));
		}
		ID += 2;
    ID %= 5;
	}

    // increments barrier
	V(barrier);

	for (i=0; i<NTHREADS; i++) {
        // decrements tsem
		P(tsem);
	}
}


int
schedulertest(int nargs, char **args)
{
	(void)nargs;
	(void)args;

	init_semaphores();
	kprintf("Starting scheduler test...\n");
	runthreads();
	kprintf("\nScheduler test done.\n");

	return 0;
}


/* sch2 code */

static
void
runthreads2()
{
	char name[16];
	int i, result;
	unsigned int ID = 1;
	int pri[10] = {1, 2, 3, 4, 5, 14, 16, 18, 20, 22};

	for (i=1; i<=NTHREADS; i++) {
		snprintf(name, sizeof(name), "thread_%d\n", i);
		result = thread_fork_priority(name, pri[ID], NULL, loop, NULL, ID);
		if (result) {
			panic("schedulertest: thread_fork failed %s)\n", strerror(result));
		}
		ID++;
    ID %= 10;
	}

    // increments barrier
	V(barrier);

	for (i=0; i<NTHREADS; i++) {
        // decrements tsem
		P(tsem);
	}
}


int
schedulertest2(int nargs, char **args)
{
	(void)nargs;
	(void)args;

	init_semaphores();
	kprintf("Starting scheduler test...\n");
	runthreads2();
	kprintf("\nScheduler test done.\n");

	return 0;
}


/* sch3 code */


static
void
runthreads3()
{
	char name[16];
	int i, result;
	unsigned int ID = 1;
	int pri[10] = {3, 6, 14, 15, 25, 26, 27, 28, 29, 30};

	for (i=1; i<=NTHREADS; i++) {
		snprintf(name, sizeof(name), "thread_%d\n", i);
		result = thread_fork_priority(name, pri[ID], NULL, loop, NULL, ID);
		if (result) {
			panic("schedulertest: thread_fork failed %s)\n", strerror(result));
		}
		ID++;
    ID %= 10;
	}

    // increments barrier
	V(barrier);

	for (i=0; i<NTHREADS; i++) {
        // decrements tsem
		P(tsem);
	}
}


int
schedulertest3(int nargs, char **args)
{
	(void)nargs;
	(void)args;

	init_semaphores();
	kprintf("Starting scheduler test...\n");
	runthreads3();
	kprintf("\nScheduler test done.\n");

	return 0;
}
