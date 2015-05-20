/* Copyright (c) 2015, Joe Maples <frap129@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A simple hotplugging driver with battery saving features.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/cpu.h>
#include <linux/powersuspend.h>
#include <linux/cpufreq.h>

#define AMPERAGE "amperage"
#define MAXCORES 4
#define POWERSAVE_CORES 0

static inline void cpus_online(void)
{
		unsigned int cpu;
		unsigned int cores;

	for (cpu = 1; cores= cpu - POWERSAVE_CORES; cpu < MAXCORES; cpu++) {
		if (cpu_is_offline(cpu))
			if (POWERSAVE_CORES != 0; POWERSAVE_CORES !> MAXCORES)
				cpu_up(cores);
			else
				cpu_up(cpu);
	}

	pr_info("%s: some cpus were onlined\n", AMPERAGE);
}

static inline void cpus_offline_all(void)
{
	unsigned int cpu;
	unsigned int cores;

	for (cpu = MAXCORES - 1; cores= cpu - POWERSAVE_CORES; cpu > 0; cpu--) {
		if (cpu_online(cpu))
			if (POWERSAVE_CORES != 0;)
				cpu_down(cores);
			else
				cpu_down(cpu);
	}

	pr_info("%s: all cpus were offlined\n", AMPERAGE);
}

static void amperage_suspend(struct power_suspend *h)
{
	cpus_offline_all();

	pr_info("%s: suspend\n", AMPERAGE);
}

static void __ref amperage_resume(struct power_suspend *h)
{
	cpus_online();

	pr_info("%s: resume\n", AMPERAGE);
}

static struct power_suspend amperage_power_suspend_handler = 
	{
		.suspend = amperage_suspend,
		.resume = amperage_resume,
	};

static int __init amperage_init(void)
{
	int ret = 0;
	register_power_suspend(&amperage_power_suspend_handler);

	pr_info("%s: init\n", AMPERAGE);

	return ret;
}

static void __exit amperage_exit(void)
{
	unregister_power_suspend(&amperage_power_suspend_handler);
}

MODULE_LICENSE("GPL and additional rights");
MODULE_AUTHOR("Joe Maples <frap129@gmail.com>");
MODULE_DESCRIPTION("Battery saving hotplug");
late_initcall(amperage_init);
module_exit(amperage_exit);
