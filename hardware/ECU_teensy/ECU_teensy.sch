EESchema Schematic File Version 4
LIBS:ECU_teensy-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	3400 4400 3600 4400
Wire Wire Line
	3600 2250 4550 2250
Wire Wire Line
	3400 4500 3700 4500
Wire Wire Line
	3700 4500 3700 3750
Wire Wire Line
	3700 3750 4550 3750
Wire Wire Line
	3400 4600 3700 4600
Wire Wire Line
	3700 4600 3700 4750
Wire Wire Line
	3700 4750 4550 4750
Wire Wire Line
	3400 4700 4100 4700
Wire Wire Line
	4100 4700 4100 4650
Wire Wire Line
	4100 4650 4550 4650
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 5C577F3B
P 3200 3850
F 0 "J2" H 3250 3850 50  0000 L CNN
F 1 "Light_Sensor" H 3000 3650 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3200 3850 50  0001 C CNN
F 3 "~" H 3200 3850 50  0001 C CNN
	1    3200 3850
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5C5781AB
P 3200 4600
F 0 "J1" H 3300 4500 50  0000 C CNN
F 1 "Screen" H 3120 4266 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3200 4600 50  0001 C CNN
F 3 "~" H 3200 4600 50  0001 C CNN
	1    3200 4600
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 3750 3700 3750
Connection ~ 3700 3750
Wire Wire Line
	3400 3850 3600 3850
Connection ~ 3600 3850
Wire Wire Line
	3600 3850 3600 3550
Wire Wire Line
	3400 3950 3800 3950
Wire Wire Line
	3800 3950 3800 4550
Wire Wire Line
	3800 4550 4550 4550
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5C578B0E
P 3200 3300
F 0 "J3" H 3300 3300 50  0000 C CNN
F 1 "Touch_sensor" H 3200 3100 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3200 3300 50  0001 C CNN
F 3 "~" H 3200 3300 50  0001 C CNN
	1    3200 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 3300 3900 3300
Wire Wire Line
	3900 3300 3900 4450
Wire Wire Line
	3900 4450 4550 4450
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 5C5792E9
P 3200 2700
F 0 "J4" H 3300 2650 50  0000 C CNN
F 1 "Distance_sensor" H 3120 2366 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3200 2700 50  0001 C CNN
F 3 "~" H 3200 2700 50  0001 C CNN
	1    3200 2700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 2500 3400 1850
Wire Wire Line
	3400 1850 7000 1850
Wire Wire Line
	7000 1850 7000 4250
Wire Wire Line
	7000 4250 6550 4250
Wire Wire Line
	3400 2600 4000 2600
Wire Wire Line
	4000 2600 4000 3550
Wire Wire Line
	4000 3550 4550 3550
Wire Wire Line
	3400 2700 4100 2700
Wire Wire Line
	4100 2700 4100 2550
Wire Wire Line
	4100 2550 4550 2550
Wire Wire Line
	3400 2800 3600 2800
Connection ~ 3600 2800
Wire Wire Line
	3600 2800 3600 2250
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 5C57A8E8
P 2500 3000
F 0 "J5" H 2420 2675 50  0000 C CNN
F 1 "Servo" H 2420 2766 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 2500 3000 50  0001 C CNN
F 3 "~" H 2500 3000 50  0001 C CNN
	1    2500 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	2700 2900 3600 2900
Connection ~ 3600 2900
Wire Wire Line
	3600 2900 3600 2800
Wire Wire Line
	2700 3000 2800 3000
Wire Wire Line
	2800 3000 2800 1850
Wire Wire Line
	2800 1850 3200 1850
Connection ~ 3400 1850
Wire Wire Line
	4100 3100 4100 3250
Wire Wire Line
	4100 3250 4550 3250
Wire Wire Line
	2700 3100 4100 3100
Connection ~ 3600 2250
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5C57DA93
P 3600 1650
F 0 "#FLG0101" H 3600 1725 50  0001 C CNN
F 1 "PWR_FLAG" V 3600 1778 50  0000 L CNN
F 2 "" H 3600 1650 50  0001 C CNN
F 3 "~" H 3600 1650 50  0001 C CNN
	1    3600 1650
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 1650 3600 2250
NoConn ~ 4550 2350
NoConn ~ 4550 2450
NoConn ~ 4550 2850
NoConn ~ 4550 2950
NoConn ~ 4550 3050
NoConn ~ 4550 3150
NoConn ~ 4550 3350
NoConn ~ 4550 3450
NoConn ~ 6550 3950
NoConn ~ 6550 4050
NoConn ~ 6550 4150
NoConn ~ 6550 4350
NoConn ~ 6550 4450
NoConn ~ 6550 4550
NoConn ~ 6550 4650
NoConn ~ 6550 4750
NoConn ~ 6550 4850
NoConn ~ 4550 4350
NoConn ~ 4550 4150
NoConn ~ 4550 3950
NoConn ~ 4550 4050
NoConn ~ 4550 3650
NoConn ~ 4550 4850
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5C59BF8C
P 3200 1850
F 0 "#FLG0102" H 3200 1925 50  0001 C CNN
F 1 "PWR_FLAG" H 3200 2024 50  0000 C CNN
F 2 "" H 3200 1850 50  0001 C CNN
F 3 "~" H 3200 1850 50  0001 C CNN
	1    3200 1850
	1    0    0    -1  
$EndComp
Connection ~ 3200 1850
Wire Wire Line
	3200 1850 3400 1850
Wire Wire Line
	4550 3850 3600 3850
Connection ~ 2800 3000
$Comp
L can_transceiver:CAN_transceiver U2
U 1 1 5C779CA9
P 2550 3650
F 0 "U2" H 3175 3235 50  0000 C CNN
F 1 "CAN_transceiver" H 3175 3326 50  0000 C CNN
F 2 "CAN_transceiver:CAN_transceiver" H 2500 3650 50  0001 C CNN
F 3 "" H 2500 3650 50  0001 C CNN
	1    2550 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3600 3850 3600 4150
Wire Wire Line
	2800 3850 2450 3850
Wire Wire Line
	2800 3000 2800 3850
Wire Wire Line
	2450 3550 3600 3550
Connection ~ 3600 3550
Wire Wire Line
	2450 3650 4400 3650
Wire Wire Line
	4400 3650 4400 2750
Wire Wire Line
	4400 2750 4550 2750
Wire Wire Line
	2450 3750 3000 3750
Wire Wire Line
	3000 3750 3000 3500
Wire Wire Line
	3000 3500 4300 3500
Wire Wire Line
	4300 3500 4300 2650
Wire Wire Line
	4300 2650 4550 2650
Wire Wire Line
	1100 3600 1250 3600
Wire Wire Line
	1250 3600 1250 3550
Wire Wire Line
	1250 3550 1400 3550
Wire Wire Line
	1100 3700 1250 3700
Wire Wire Line
	1250 3700 1250 3750
Wire Wire Line
	1250 3750 1400 3750
Wire Wire Line
	3600 2900 3600 3400
Wire Wire Line
	3400 3200 3700 3200
Wire Wire Line
	3700 3200 3700 3750
Wire Wire Line
	3400 3400 3600 3400
Connection ~ 3600 3400
Wire Wire Line
	3600 3400 3600 3550
$Comp
L teensy:Teensy3.2_simple U1
U 1 1 5C79AD6F
P 5600 3350
F 0 "U1" H 5550 4715 50  0000 C CNN
F 1 "Teensy3.2_simple" H 5550 4624 50  0000 C CNN
F 2 "teensy:Teensy30_31_32_LC_simple" H 5350 4700 50  0001 C CNN
F 3 "" H 5350 4700 50  0001 C CNN
	1    5600 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J7
U 1 1 5C7C6D70
P 2800 4250
F 0 "J7" H 2720 3925 50  0000 C CNN
F 1 "Mode" H 2720 4016 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2800 4250 50  0001 C CNN
F 3 "~" H 2800 4250 50  0001 C CNN
	1    2800 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	3000 4150 3600 4150
Connection ~ 3600 4150
Wire Wire Line
	3600 4150 3600 4400
Wire Wire Line
	3000 4250 4550 4250
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 5C81627E
P 900 3600
F 0 "J6" H 820 3275 50  0000 C CNN
F 1 "CAN" H 820 3366 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 900 3600 50  0001 C CNN
F 3 "~" H 900 3600 50  0001 C CNN
	1    900  3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	1100 3500 1250 3500
Wire Wire Line
	1250 3500 1250 2250
Wire Wire Line
	1250 2250 3600 2250
$EndSCHEMATC
