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

/* Default Tunable Values */
#define AMPERAGE		"amperage"
#define MAXCORES 		4
#define DEF_MIN_CPUS_ONLINE	1
#define DEF_MAX_CPUS_ONLINE 	4
#define DEF_PLUG_PERCENT	100
#define DEF_PLUGGING_THRESHOLD	1497000
#define DEF_ENABLE_POWERSAVE	1
#define DEF_MINMAX_MODE		1

/* Set Tunables */
static struct hotplug_tunables {
	unsigned int min_cpus;
	unsigned int max_cpus;
	unsigned int plugging_rate;
	unsigned int plugging_threshold;
	unsigned int enable_powersave;
	unsigned int minmax_mode;
} tunables = {
	.min_cpus = DEF_MIN_CPUS_ONLINE,
	.max_cpus = DEF_MAX_CPUS_ONLINE,
	.plugging_rate = DEF_PLUG_PERCENT,
	.plugging_threshold = DEF_PLUGGING_THRESHOLD,
	.enable_powersave = DEF_ENABLE_POWERSAVE,
	.minmax_mode = DEF_MINMAX_MODE,
};

static unsigned long get_freq1(int cpu = 0) {
	return cpufreq1_get(cpu);
}

static unsigned long get_freq2(int cpu = 1) {
	return cpufreq2_get(cpu);
}

static unsigned long get_freq3(int cpu = 2) {
	return cpufreq3_get(cpu);
}

static inline void plug_one_cpu(void)]
{
	get_freq1();
	get_freq2();
	get_freq3();
	
	for (cpu = 1; cpu++) {
		if (cpu_is_offline(cpu)){
			if (cpufreq1_get >= tunables.plugging_threshold){
				cpu_up(cpu);
			}
		}
	}
	
	for (cpu = 2; cpu++) {
		if (cpu_is_offline(cpu)){
			if (cpufreq2_get >= tunables.plugging_threshold){
				cpu_up(cpu);
			}
		}
	}
	
	for (cpu = 3; cpu++) {
		if (cpu_is_offline(cpu)){
			if (cpufreq3_get >= tunables.plugging_threshold){
				cpu_up(cpu);
			}
		}
	}
}

static inline void powersave_cores(void)
{ 
	unsigned int cores;
	unsigned int cpu;
	unsigned int pluggable_cores;
	
	if (tunables.enable_powersave == 1){
		/* Powersave Algorithm */
		if ((tunables.plugging_rate > 0) && (tunables.plugging_rate !> 100)) {
			pluggable_cores = (MAXCORES - 1) * (1 / (100 - tunables.plugging_rate);
			cores = ceil(pluggable_cores);
			pr_info("%s: calculated cores to plug\n", AMPERAGE);
		}
	} else {
		cores = cpu;
	}
	return core
}

static inline void cpus_online(void)
{
	powersave_cores();

	for (cpu = 1; cpu <= MAXCORES; cpu++) {
		if (cpu_is_offline(cores)){
			cpu_up(cores);
		}
	}

	pr_info("%s: some cpus were put online\n", AMPERAGE);
}

static inline void cpus_offline_all(void)
{
	powersave_cores();

	for (cpu = MAXCORES - 1; cpu > 0; cpu--) {
		if (cpu_online(cores)){
			if (pluggable_cores != 0;){
				cpu_down(cores);
			} else {
				cpu_down(cpu);
			}
		}
	}

	pr_info("%s: all cpus were unplugged\n", AMPERAGE);
}

static void amperage_suspend(struct power_suspend *h)
{
	cpus_offline_all();

	pr_info("%s: suspend\n", AMPERAGE);
}

static void __ref amperage_resume(struct power_suspend *h)
{
	if (tunables.minmax_mode != 1)
	{
		plug_one_cpu();
	} else {
		cpus_online();
	}

	pr_info("%s: resume\n", AMPERAGE);
}

static struct power_suspend amperage_power_suspend_handler = 
	{
		.suspend = amperage_suspend,
		.resume = amperage_resume,
	};

static int __init start_amperage(void)
{
	int ret = 0;
	register_power_suspend(&amperage_power_suspend_handler);

	pr_info("%s: start driver\n", AMPERAGE);

	return ret;
}

static void __exit exit_amperage(void)
{
	pr_info("%s: exit driver\n", AMPERAGE);
	unregister_power_suspend(&amperage_power_suspend_handler);
}

MODULE_LICENSE("GPL and additional rights");
MODULE_AUTHOR("Joe Maples <frap129@gmail.com>");
MODULE_DESCRIPTION("Battery saving hotplug");
late_initcall(start_amperage);
module_exit(exit_amperage);
