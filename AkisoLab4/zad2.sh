#!/bin/bash
# set -x
convert_bytes() {
    local bytes=$1
    if ((bytes < 1024)); then
        echo "${bytes}B"
    elif ((bytes < 1048576)); then
        echo "$((bytes / 1024))KB"
    else
        echo "$((bytes / 1048576))MB"
    fi
}

convert_time() {
    local seconds=$(LC_NUMERIC=C printf "%.0f" $1)

    local days=$((seconds / 86400))
    seconds=$((seconds % 86400))

    local hours=$((seconds / 3600))
    seconds=$((seconds % 3600))

    local minutes=$((seconds / 60))
    seconds=$((seconds % 60))

    echo "Days: $days, Hours: $hours, Minutes: $minutes, Seconds: $seconds"
}

printCpuInfo() {
    # CPU usage
    echo -e "\n=== CPU Usage ==="
    cores=$(awk 'NR==13 {print $4}' /proc/cpuinfo)

    for ((i = 1; i <= cores; i++)); do
        if [ ! -e "/sys/devices/system/cpu/cpu$i/cpuf" ]; then
            echo "Core$i: NO DATA"
        else
            core_frequency=$(cat "/sys/devices/system/cpu/cpu$i/cpufreq/scaling_cur_freq")
            echo "Core$i Frequency: $(convert_bytes $core_frequency)"
        fi
    done
}

print

recv_arr=(0 0 0 0 0 0 0 0 0 0)
sent_arr=(0 0 0 0 0 0 0 0 0 0)

recv_arr[0]=$(awk 'NR==4 {print $2}' /proc/net/dev)
sent_arr[0]=$(awk 'NR==4 {print $10}' /proc/net/dev)

sleep 1

for ((i=1; 1; i++)); do
    clear
    temp_n=$((i % 10))
    temp_nn=$(((i-1) % 10))

    # Network speed
    recv_arr[$temp_n]=$(awk 'NR==4 {print $2}' /proc/net/dev)
    sent_arr[$temp_n]=$(awk 'NR==4 {print $10}' /proc/net/dev)

    echo "=== Network Speed ==="

    temp_recv=$((recv_arr[$temp_n] - recv_arr[$temp_nn]))
    temp_sent=$((sent_arr[$temp_n] - sent_arr[$temp_nn]))

    echo -e "Receiving: $(convert_bytes $temp_recv)/s"
    echo -e "Sending: $(convert_bytes $temp_sent)/s"
    

    echo "$(printCpuInfo)"

    # Uptime
    echo -e "\n=== Uptime ==="
    uptime_seconds=$(cut -d' ' -f1 /proc/uptime)
    echo -e "$(convert_time $uptime_seconds)"

    # Battery status
    echo -e "\n=== Battery Status ==="
    battery_percent=$(cat "/sys/class/power_supply/BAT0/uevent" | grep "POWER_SUPPLY_CAPACITY=" | cut -d'=' -f2)
    echo -e "Battery Level: $battery_percent%"

    # System load
    echo -e "\n=== System Load ==="
    cat /proc/loadavg

    # Memory usage
    echo -e "\n=== Memory Usage ==="
    head -n 3 /proc/meminfo

    sleep 1
done
