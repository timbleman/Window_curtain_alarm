EESchema Schematic File Version 4
LIBS:curtain_wiring-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Curtain alarm wiring"
Date "2021-10-29"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ESP8266:NodeMCU1.0(ESP-12E) U1
U 1 1 617C27C4
P 3200 2850
F 0 "U1" H 3200 3937 60  0000 C CNN
F 1 "NodeMCU1.0(ESP-12E)" H 3200 3831 60  0000 C CNN
F 2 "" H 2600 2000 60  0000 C CNN
F 3 "" H 2600 2000 60  0000 C CNN
	1    3200 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 617C382C
P 4150 1300
F 0 "D1" H 4143 1045 50  0000 C CNN
F 1 "LED" H 4143 1136 50  0000 C CNN
F 2 "" H 4150 1300 50  0001 C CNN
F 3 "~" H 4150 1300 50  0001 C CNN
	1    4150 1300
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR08
U 1 1 617C41F8
P 4300 1300
F 0 "#PWR08" H 4300 1050 50  0001 C CNN
F 1 "GND" H 4305 1127 50  0000 C CNN
F 2 "" H 4300 1300 50  0001 C CNN
F 3 "" H 4300 1300 50  0001 C CNN
	1    4300 1300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 617C548F
P 5350 1600
F 0 "SW1" H 5350 1885 50  0000 C CNN
F 1 "SW_WOS_CURTAINXOR_CALIBRATE" H 5750 1800 50  0000 C CNN
F 2 "" H 5350 1800 50  0001 C CNN
F 3 "~" H 5350 1800 50  0001 C CNN
	1    5350 1600
	1    0    0    -1  
$EndComp
$Comp
L Motor:Stepper_Motor_bipolar M1
U 1 1 617C5AA3
P 9000 3150
F 0 "M1" H 9188 3274 50  0000 L CNN
F 1 "Stepper_Motor_bipolar" H 9188 3183 50  0000 L CNN
F 2 "" H 9010 3140 50  0001 C CNN
F 3 "http://www.infineon.com/dgdl/Application-Note-TLE8110EE_driving_UniPolarStepperMotor_V1.1.pdf?fileId=db3a30431be39b97011be5d0aa0a00b0" H 9010 3140 50  0001 C CNN
	1    9000 3150
	1    0    0    -1  
$EndComp
$Comp
L SilentStepStick-TMC2209_v20-cache:SilentStepStick-TMC2209_v20-eagle-import_TMC2209-LA IC1
U 1 1 617C6BF1
P 7650 2650
F 0 "IC1" H 7650 4136 59  0000 C CNN
F 1 "SilentStepStick-TMC2209_v20" H 7650 4031 59  0000 C CNN
F 2 "" H 7650 2650 50  0001 C CNN
F 3 "" H 7650 2650 50  0001 C CNN
	1    7650 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 3250 8700 3250
Wire Wire Line
	8450 3050 8700 3050
Wire Wire Line
	8450 2850 8900 2850
Wire Wire Line
	9100 2850 9100 2650
Wire Wire Line
	9100 2650 8450 2650
$Comp
L power:GND #PWR017
U 1 1 617D1811
P 6600 1900
F 0 "#PWR017" H 6600 1650 50  0001 C CNN
F 1 "GND" H 6605 1727 50  0000 C CNN
F 2 "" H 6600 1900 50  0001 C CNN
F 3 "" H 6600 1900 50  0001 C CNN
	1    6600 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 1900 6600 1850
Wire Wire Line
	6600 1850 6850 1850
$Comp
L power:+3.3V #PWR016
U 1 1 617D2026
P 6600 1400
F 0 "#PWR016" H 6600 1250 50  0001 C CNN
F 1 "+3.3V" H 6615 1573 50  0000 C CNN
F 2 "" H 6600 1400 50  0001 C CNN
F 3 "" H 6600 1400 50  0001 C CNN
	1    6600 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR06
U 1 1 617D2F71
P 4150 3550
F 0 "#PWR06" H 4150 3400 50  0001 C CNN
F 1 "+3.3V" H 4100 3500 50  0000 C CNN
F 2 "" H 4150 3550 50  0001 C CNN
F 3 "" H 4150 3550 50  0001 C CNN
	1    4150 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 3550 4000 3550
$Comp
L power:+3.3V #PWR02
U 1 1 617D3826
P 2200 3150
F 0 "#PWR02" H 2200 3000 50  0001 C CNN
F 1 "+3.3V" H 2150 3300 50  0000 C CNN
F 2 "" H 2200 3150 50  0001 C CNN
F 3 "" H 2200 3150 50  0001 C CNN
	1    2200 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 3150 2400 3150
$Comp
L power:GND #PWR01
U 1 1 617D4547
P 1750 3100
F 0 "#PWR01" H 1750 2850 50  0001 C CNN
F 1 "GND" H 1755 2927 50  0000 C CNN
F 2 "" H 1750 3100 50  0001 C CNN
F 3 "" H 1750 3100 50  0001 C CNN
	1    1750 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 2850 2350 3050
Wire Wire Line
	2350 3050 2400 3050
Wire Wire Line
	1750 2850 1750 3100
$Comp
L power:GND #PWR07
U 1 1 617D51F1
P 4250 3650
F 0 "#PWR07" H 4250 3400 50  0001 C CNN
F 1 "GND" H 4255 3477 50  0000 C CNN
F 2 "" H 4250 3650 50  0001 C CNN
F 3 "" H 4250 3650 50  0001 C CNN
	1    4250 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3650 4250 3250
Wire Wire Line
	4250 3250 4050 3250
Wire Wire Line
	4050 3250 4050 3450
Wire Wire Line
	4050 3450 4000 3450
$Comp
L power:GND #PWR010
U 1 1 617D5E58
P 4400 2750
F 0 "#PWR010" H 4400 2500 50  0001 C CNN
F 1 "GND" H 4250 2700 50  0000 C CNN
F 2 "" H 4400 2750 50  0001 C CNN
F 3 "" H 4400 2750 50  0001 C CNN
	1    4400 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2750 4000 2750
$Comp
L power:+3.3V #PWR09
U 1 1 617D6966
P 4400 2650
F 0 "#PWR09" H 4400 2500 50  0001 C CNN
F 1 "+3.3V" H 4250 2750 50  0000 C CNN
F 2 "" H 4400 2650 50  0001 C CNN
F 3 "" H 4400 2650 50  0001 C CNN
	1    4400 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2650 4400 2650
$Comp
L power:GND #PWR012
U 1 1 617D7978
P 5650 1900
F 0 "#PWR012" H 5650 1650 50  0001 C CNN
F 1 "GND" H 5655 1727 50  0000 C CNN
F 2 "" H 5650 1900 50  0001 C CNN
F 3 "" H 5650 1900 50  0001 C CNN
	1    5650 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1600 5550 1600
$Comp
L Device:R R3
U 1 1 617D8A93
P 5000 1600
F 0 "R3" V 4793 1600 50  0000 C CNN
F 1 "1k" V 4884 1600 50  0000 C CNN
F 2 "" V 4930 1600 50  0001 C CNN
F 3 "~" H 5000 1600 50  0001 C CNN
	1    5000 1600
	0    1    1    0   
$EndComp
$Comp
L Device:C C1
U 1 1 617D915D
P 4800 1750
F 0 "C1" H 4915 1796 50  0000 L CNN
F 1 "100nF" H 4915 1705 50  0000 L CNN
F 2 "" H 4838 1600 50  0001 C CNN
F 3 "~" H 4800 1750 50  0001 C CNN
	1    4800 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 1600 4800 1600
$Comp
L power:GND #PWR011
U 1 1 617DA2CA
P 4800 1900
F 0 "#PWR011" H 4800 1650 50  0001 C CNN
F 1 "GND" H 4900 1800 50  0000 C CNN
F 2 "" H 4800 1900 50  0001 C CNN
F 3 "" H 4800 1900 50  0001 C CNN
	1    4800 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1600 5650 1900
Wire Wire Line
	4000 2450 4200 2450
$Comp
L Connector:Conn_01x03_Male J1
U 1 1 617F6DC1
P 3300 5750
F 0 "J1" H 3272 5682 50  0000 R CNN
F 1 "Conn_01x03_Male" H 3272 5773 50  0000 R CNN
F 2 "" H 3300 5750 50  0001 C CNN
F 3 "~" H 3300 5750 50  0001 C CNN
	1    3300 5750
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR05
U 1 1 617F7B11
P 3100 5850
F 0 "#PWR05" H 3100 5600 50  0001 C CNN
F 1 "GND" H 3105 5677 50  0000 C CNN
F 2 "" H 3100 5850 50  0001 C CNN
F 3 "" H 3100 5850 50  0001 C CNN
	1    3100 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 5750 3100 5750
$Comp
L power:+12V #PWR04
U 1 1 617FABBF
P 3100 5650
F 0 "#PWR04" H 3100 5500 50  0001 C CNN
F 1 "+12V" H 3115 5823 50  0000 C CNN
F 2 "" H 3100 5650 50  0001 C CNN
F 3 "" H 3100 5650 50  0001 C CNN
	1    3100 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 1400 6600 1450
Wire Wire Line
	6600 1450 6850 1450
$Comp
L power:GND #PWR018
U 1 1 617FE919
P 6750 3450
F 0 "#PWR018" H 6750 3200 50  0001 C CNN
F 1 "GND" H 6850 3350 50  0000 C CNN
F 2 "" H 6750 3450 50  0001 C CNN
F 3 "" H 6750 3450 50  0001 C CNN
	1    6750 3450
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR020
U 1 1 6180272F
P 8600 1400
F 0 "#PWR020" H 8600 1250 50  0001 C CNN
F 1 "+12V" H 8615 1573 50  0000 C CNN
F 2 "" H 8600 1400 50  0001 C CNN
F 3 "" H 8600 1400 50  0001 C CNN
	1    8600 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 1450 8600 1450
Wire Wire Line
	8600 1450 8600 1400
$Comp
L Device:C C3
U 1 1 6180434C
P 8750 1600
F 0 "C3" H 8865 1646 50  0000 L CNN
F 1 "100uF" H 8865 1555 50  0000 L CNN
F 2 "" H 8788 1450 50  0001 C CNN
F 3 "~" H 8750 1600 50  0001 C CNN
	1    8750 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 1450 8750 1450
Connection ~ 8600 1450
$Comp
L power:GND #PWR021
U 1 1 61805D25
P 8750 1750
F 0 "#PWR021" H 8750 1500 50  0001 C CNN
F 1 "GND" H 8755 1577 50  0000 C CNN
F 2 "" H 8750 1750 50  0001 C CNN
F 3 "" H 8750 1750 50  0001 C CNN
	1    8750 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3450 6850 3450
Wire Wire Line
	4000 3050 5200 3050
Wire Wire Line
	5200 3050 5200 3150
Wire Wire Line
	5050 3150 5200 3150
$Comp
L Device:R R2
U 1 1 61815640
P 4900 3150
F 0 "R2" V 5100 3150 50  0000 C CNN
F 1 "1k" V 5000 3150 50  0000 C CNN
F 2 "" V 4830 3150 50  0001 C CNN
F 3 "~" H 4900 3150 50  0001 C CNN
	1    4900 3150
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 6181C3A7
P 4000 1800
F 0 "R1" H 4070 1846 50  0000 L CNN
F 1 "100" H 4070 1755 50  0000 L CNN
F 2 "" V 3930 1800 50  0001 C CNN
F 3 "~" H 4000 1800 50  0001 C CNN
	1    4000 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 1950 4000 2150
Wire Wire Line
	4200 1600 4700 1600
Wire Wire Line
	4200 1600 4200 2450
Connection ~ 4800 1600
Wire Wire Line
	4000 1300 4000 1650
Wire Wire Line
	5200 3650 5200 3150
Connection ~ 5200 3150
Wire Wire Line
	4000 3150 4750 3150
$Comp
L power:+3.3V #PWR013
U 1 1 617FDEA7
P 5850 4700
F 0 "#PWR013" H 5850 4550 50  0001 C CNN
F 1 "+3.3V" H 5800 4850 50  0000 C CNN
F 2 "" H 5850 4700 50  0001 C CNN
F 3 "" H 5850 4700 50  0001 C CNN
	1    5850 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 617FDADF
P 5850 4850
F 0 "R4" H 5920 4896 50  0000 L CNN
F 1 "10k" H 5920 4805 50  0000 L CNN
F 2 "" V 5780 4850 50  0001 C CNN
F 3 "~" H 5850 4850 50  0001 C CNN
	1    5850 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 5000 6850 5000
Wire Wire Line
	6500 5000 6600 5000
$Comp
L Device:R R5
U 1 1 617E92FF
P 6350 5000
F 0 "R5" V 6143 5000 50  0000 C CNN
F 1 "1k" V 6234 5000 50  0000 C CNN
F 2 "" V 6280 5000 50  0001 C CNN
F 3 "~" H 6350 5000 50  0001 C CNN
	1    6350 5000
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 617E202D
P 6600 4800
F 0 "J2" V 6538 4612 50  0000 R CNN
F 1 "END_STOP" V 6447 4612 50  0000 R CNN
F 2 "" H 6600 4800 50  0001 C CNN
F 3 "~" H 6600 4800 50  0001 C CNN
	1    6600 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4000 2950 5550 2950
$Comp
L power:GND #PWR019
U 1 1 617E92F8
P 6850 5300
F 0 "#PWR019" H 6850 5050 50  0001 C CNN
F 1 "GND" H 6855 5127 50  0000 C CNN
F 2 "" H 6850 5300 50  0001 C CNN
F 3 "" H 6850 5300 50  0001 C CNN
	1    6850 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 5000 6150 5000
Wire Wire Line
	5850 5000 6150 5000
Connection ~ 6150 5000
$Comp
L Device:C C2
U 1 1 617E9305
P 6150 5150
F 0 "C2" H 6265 5196 50  0000 L CNN
F 1 "100nF" H 6265 5105 50  0000 L CNN
F 2 "" H 6188 5000 50  0001 C CNN
F 3 "~" H 6150 5150 50  0001 C CNN
	1    6150 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 617E930C
P 6150 5300
F 0 "#PWR014" H 6150 5050 50  0001 C CNN
F 1 "GND" H 6155 5127 50  0000 C CNN
F 2 "" H 6150 5300 50  0001 C CNN
F 3 "" H 6150 5300 50  0001 C CNN
	1    6150 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 5000 6850 5300
Connection ~ 5850 5000
Wire Wire Line
	5000 2850 5000 2450
Wire Wire Line
	4000 2850 5000 2850
$Comp
L power:+3.3V #PWR015
U 1 1 61800638
P 6550 3850
F 0 "#PWR015" H 6550 3700 50  0001 C CNN
F 1 "+3.3V" H 6450 4000 50  0000 C CNN
F 2 "" H 6550 3850 50  0001 C CNN
F 3 "" H 6550 3850 50  0001 C CNN
	1    6550 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2350 4800 2350
Wire Wire Line
	4800 2350 4800 2550
Wire Wire Line
	6550 3850 6850 3850
Wire Wire Line
	5550 5000 5850 5000
Wire Wire Line
	5550 2950 5550 5000
Wire Wire Line
	4000 2250 6850 2250
Wire Wire Line
	5000 2450 6850 2450
Wire Wire Line
	4800 2550 6850 2550
Wire Wire Line
	5200 3650 6850 3650
NoConn ~ 8450 3650
NoConn ~ 8450 3850
NoConn ~ 6850 3050
NoConn ~ 6850 3150
NoConn ~ 6850 3250
NoConn ~ 6850 2850
NoConn ~ 6850 2750
NoConn ~ 6850 2050
NoConn ~ 6850 1650
NoConn ~ 8450 1850
NoConn ~ 8450 2050
NoConn ~ 8450 2250
NoConn ~ 4000 2550
NoConn ~ 4000 3250
NoConn ~ 4000 3350
NoConn ~ 2400 2150
NoConn ~ 2400 2250
NoConn ~ 2400 2350
NoConn ~ 2400 2450
NoConn ~ 2400 2550
NoConn ~ 2400 2650
NoConn ~ 2400 2750
NoConn ~ 2400 2850
NoConn ~ 2400 2950
NoConn ~ 2400 3250
NoConn ~ 2400 3350
$Comp
L power:+3.3V #PWR022
U 1 1 617E88E9
P 4700 1250
F 0 "#PWR022" H 4700 1100 50  0001 C CNN
F 1 "+3.3V" H 4650 1400 50  0000 C CNN
F 2 "" H 4700 1250 50  0001 C CNN
F 3 "" H 4700 1250 50  0001 C CNN
	1    4700 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 617E88EF
P 4700 1400
F 0 "R6" H 4770 1446 50  0000 L CNN
F 1 "10k" H 4770 1355 50  0000 L CNN
F 2 "" V 4630 1400 50  0001 C CNN
F 3 "~" H 4700 1400 50  0001 C CNN
	1    4700 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 1550 4700 1600
Connection ~ 4700 1600
Wire Wire Line
	4700 1600 4800 1600
$Comp
L power:+VDC #PWR03
U 1 1 617EE083
P 1500 3550
F 0 "#PWR03" H 1500 3450 50  0001 C CNN
F 1 "+VDC" H 1500 3825 50  0000 C CNN
F 2 "" H 1500 3550 50  0001 C CNN
F 3 "" H 1500 3550 50  0001 C CNN
	1    1500 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3550 1950 3550
$Comp
L power:+VDC #PWR023
U 1 1 617F6538
P 2900 5750
F 0 "#PWR023" H 2900 5650 50  0001 C CNN
F 1 "+VDC" H 2900 6025 50  0000 C CNN
F 2 "" H 2900 5750 50  0001 C CNN
F 3 "" H 2900 5750 50  0001 C CNN
	1    2900 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 617FA5F5
P 1950 3400
F 0 "C?" H 2065 3446 50  0000 L CNN
F 1 "220uF" H 2065 3355 50  0000 L CNN
F 2 "" H 1988 3250 50  0001 C CNN
F 3 "~" H 1950 3400 50  0001 C CNN
	1    1950 3400
	1    0    0    -1  
$EndComp
Connection ~ 1950 3550
Wire Wire Line
	1950 3550 2400 3550
Wire Wire Line
	1950 3250 1950 2850
Wire Wire Line
	1750 2850 1950 2850
Connection ~ 1950 2850
Wire Wire Line
	1950 2850 2350 2850
Connection ~ 1950 3250
Wire Wire Line
	2350 3250 2350 3450
Wire Wire Line
	2350 3450 2400 3450
Wire Wire Line
	1950 3250 2350 3250
$EndSCHEMATC
