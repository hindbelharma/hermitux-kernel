/*
 * Copyright (c) 2010, Stefan Lankes, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <hermit/spinlock.h>

spinlock_t readwritev_spinlock = SPINLOCK_INIT;
spinlock_irqsave_t lwip_lock = SPINLOCK_IRQSAVE_INIT;

#define HERMITUX_HOSTNAME_LEN 65 /* Should not be greater than 65 to fit in struct
						   utsname */

const size_t hermitux_hostname_len = HERMITUX_HOSTNAME_LEN;
char hermitux_hostname[128] = "hermitux";

/* Timing syscalls (gettimeofday, time, clock_gettime) will return values
 * relative to the boot time stamp counter. This is called by the kernel at
 * boot time */
unsigned long long syscall_boot_tsc = 0;
unsigned long long syscall_freq = 0;

void syscall_timing_init(void) {
#if __aarch64__
    syscall_boot_tsc = get_cntpct();
    syscall_freq = get_cntfrq();
#else
	syscall_boot_tsc = get_rdtsc();
	syscall_freq = get_cpu_frequency() * 1000000ULL;
#endif
}

static spinlock_irqsave_t malloc_lock = SPINLOCK_IRQSAVE_INIT;

void __sys_malloc_lock(void)
{
	spinlock_irqsave_lock(&malloc_lock);
}

void __sys_malloc_unlock(void)
{
	spinlock_irqsave_unlock(&malloc_lock);
}

static spinlock_irqsave_t env_lock = SPINLOCK_IRQSAVE_INIT;

void __sys_env_lock(void)
{
	spinlock_irqsave_lock(&env_lock);
}

void __sys_env_unlock(void)
{
	spinlock_irqsave_unlock(&env_lock);
}
