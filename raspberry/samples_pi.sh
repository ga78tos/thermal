#!/bin/bash

sudo echo "userspace" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
sudo echo "userspace" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
sudo echo "userspace" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor

sudo echo 600000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_setspeed
sudo echo 600000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_setspeed
sudo echo 600000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_setspeed

for i in 0 1 2 3 4 5 6 7 8 9
do

	for j in 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14
	do

		for mode in g p
		do

			for x in 1 2
			do
				sudo echo "userspace" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
				sudo echo 600000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed


				temp=99999
				while [ "$temp" -ge 45500 ]
				do
					sudo sh -c 'echo 255 > /sys/class/leds/led0/brightness'
					sleep 2
					sudo sh -c 'echo 0 > /sys/class/leds/led0/brightness'
					sleep 2
					sudo sh -c 'echo 255 > /sys/class/leds/led0/brightness'
					sleep 2
					sudo sh -c 'echo 0 > /sys/class/leds/led0/brightness'
					sleep 2
					sudo sh -c 'echo 255 > /sys/class/leds/led0/brightness'
					sleep 2
					temp="$(sudo cat /sys/class/thermal/thermal_zone0/temp)"
					echo "    $temp"
					echo "    $i"
				done

				sudo sh -c 'echo 0 > /sys/class/leds/led0/brightness'

				date

				sleep 10

				sudo ./main ./schedules/${mode}mpt${x}/sched_${j}.xml

				cp ./example_temprature_trace.csv ./schedules/${mode}mpt${x}/${j}/trace_${i}.csv

			done
		done
	done
done

sudo halt
