#!/bin/sh

grep remaining  /proc/acpi/battery/C0FF/state
grep "present rate"	/proc/acpi/battery/C0FF/state
