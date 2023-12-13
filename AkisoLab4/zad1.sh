#!/bin/bash

# set -x
# set -u
# set -e

echo "PPID" "PID" "COMM" "STATE" "TTY" "RSS" "PGID" "SID" "OPEN_FILES" >> tempzad1.txt

number=$(find "/proc" -maxdepth 1 -type d -regex '.*/[0-9]+$' -exec basename {} \;)


for pid in $number; do
    
    status_file="/proc/$pid/stat"

    # Check if the file exists
    if [ -e "/proc/$pid" ]; then
        # Get information from the status file
        ppid=$(awk '{print $4}' /proc/$pid/stat)
        comm=$(awk '{print $2}' /proc/$pid/stat)
        state=$(awk '{print $3}' /proc/$pid/stat)
        tty=$(awk '{print $7}' /proc/$pid/stat)
        rss=$(awk '{print $24}' /proc/$pid/stat)
        pgid=$(awk '{print $5}' /proc/$pid/stat)
        sid=$(awk '{print $6}' /proc/$pid/stat)

        # Count the number of open files
        open_fd=$(ls /proc/$pid/fd 2>/dev/null | wc -l)

        # Display information in one line
        if [ -e "/proc/$pid" ]; then
            echo "$ppid" "$pid" "$comm" "$state" "$tty" "$rss" "$pgid" "$sid" "$open_fd" >> tempzad1.txt
        fi
        echo "$ppid" "$pid" "$comm" "$state" "$tty" "$rss" "$pgid" "$sid" "$open_fd" >> tempzad1.txt
    fi
done

column tempzad1.txt -t -s " "

rm tempzad1.txt
