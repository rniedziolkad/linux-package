#
# Regular cron jobs for the gimp package
#
0 4	* * *	root	[ -x /usr/bin/gimp_maintenance ] && /usr/bin/gimp_maintenance
