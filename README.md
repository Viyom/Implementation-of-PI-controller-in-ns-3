# Implementation of PI Controller in ns-3
##Course Code: CS822
##Assignment:  #FP2 

###Overview

PI Controller [1] is an Active Queue Management (AQM) algorithm like RED [2], ARED [3], etc. PIE [4], the latest algorithm, is based on PI. PI Controller is already implemented in ns-2 [5]. This repository contains an implementation of PI controller in ns-3 [6].

###Path for PI Controller implementation 

To check the code for implementation of PI Controller, refer to the path given below:

`src/traffic-control/model/pi-queue-disc.h`

`src/traffic-control/model/pi-queue-disc.cc`

###PI Controller TestSuite

To find the code for TestSuite of PI Controller, refer to the path given below: 

`src/traffic-control/test/pi-queue-disc-test-suite.cc`

and should be executed as

`./test.py --suite=pi-queue-disc`


###References:


[1]  On designing improved controllers for AQM Routers supporting TCP flows (Link:   
http://ieeexplore.ieee.org/document/916670/)

[2]  S. Floyd and V. Jacobson, "Random Early Detection gateways for congestion avoidance", IEEE/ACM Transactions on Networking, vol. 1, no. 4, August 1997

[3]  S. Floyd, R. Gummadi, and S.Shenker. Adaptive RED: An Algorithm for Increasing the Rob ustness of RED. Technical Report, to appear, 2001

[4]  Pan, R., Natarajan, P. Piglione, C., Prabhu, M.S., Subramanian, V., Baker, F. Steeg and B. V., "PIE: A Lightweight Control Scheme to Address the Bufferbloat Problem", IEEE HPSR 2013

[5]  "ns-2 Network Simulator", [online] Available: http://www.isi.edu/nsnamlns/

[6]  The ns-3 network simulator. http://www.nsnam.org/


