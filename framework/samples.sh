#!/bin/bash

sudo echo "userspace" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
sudo echo "userspace" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
sudo echo "userspace" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor

sudo echo 768000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_setspeed
sudo echo 768000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_setspeed
sudo echo 768000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_setspeed

for j in 00 01 02 03 04 05 06 07 08 09 10 11 12
do

	for mode in g p
	do

		for i in 0 1 2 3 4 5 6 7 8 9
		do

			sudo echo 768000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed

			temp=0
			while [ "$temp" -ne 44000 ]
			do
				sleep 5
				temp="$(sudo cat /sys/class/thermal/thermal_zone0/temp)"
				echo "$temp"
			done

			sudo ./main ./schedules/${mode}_sched${j}.xml
			python scr.py

			cp ./example_temprature_trace.csv ../${mode}mpt_scheds/${j}/trace_${i}.csv
			cp ./out.png ../${mode}mpt_scheds/${j}/trace_${i}.png
		done

	done

done
