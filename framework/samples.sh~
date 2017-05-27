#!/bin/bash

sudo echo "userspace" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
sudo echo "userspace" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
sudo echo "userspace" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor

sudo echo 768000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_setspeed
sudo echo 768000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_setspeed
sudo echo 768000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_setspeed

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

 sudo ./run_program.sh
 #change path to wherever you want the results to be saved
 cp ./example_temprature_trace.csv ../theraml_samples/0/trace_${i}.csv
 cp ./out.png ../thermal_samples/0/trace_${i}.png
done
