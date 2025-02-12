EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ESP-12E"
Date "2022-04-11"
Rev "Esp-8266 v.1.0"
Comp "TAYRIX "
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF_Module:ESP-12E U?
U 1 1 625FD25F
P 3800 2480
F 0 "U?" H 3440 3300 50  0000 C CNN
F 1 "ESP-12E" H 3460 3220 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 3800 2480 50  0001 C CNN
F 3 "http://wiki.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_v1.1.pdf" H 3450 2580 50  0001 C CNN
	1    3800 2480
	1    0    0    -1  
$EndComp
Text GLabel 3040 1880 0    50   Input ~ 0
RST
Text GLabel 3020 2080 0    50   Input ~ 0
EN
Text GLabel 3030 2280 0    50   Input ~ 0
ADC
Text GLabel 3000 2480 0    50   Input ~ 0
CSO
Text GLabel 3010 2580 0    50   Input ~ 0
MISO
Text GLabel 3010 2680 0    50   Input ~ 0
GPIO9
Text GLabel 3010 2780 0    50   Input ~ 0
GPIO10
Text GLabel 3010 2880 0    50   Input ~ 0
MOSI
Text GLabel 3010 2980 0    50   Input ~ 0
SCLK
Text GLabel 4570 1880 2    50   Input ~ 0
GPIO0
Text GLabel 4570 1980 2    50   Input ~ 0
TXD
Text GLabel 4560 2080 2    50   Input ~ 0
GPIO2
Text GLabel 4570 2180 2    50   Input ~ 0
RXD
Text GLabel 4570 2280 2    50   Input ~ 0
GPIO4
Text GLabel 4570 2380 2    50   Input ~ 0
GPIO5
Text GLabel 4570 2480 2    50   Input ~ 0
GPIO12
Text GLabel 4570 2580 2    50   Input ~ 0
GPIO13
Text GLabel 4570 2680 2    50   Input ~ 0
GPIO14
Text GLabel 4570 2780 2    50   Input ~ 0
GPIO15
Text GLabel 4570 2880 2    50   Input ~ 0
GPIO16
Wire Wire Line
	3040 1880 3200 1880
Wire Wire Line
	3020 2080 3200 2080
Wire Wire Line
	3030 2280 3200 2280
Wire Wire Line
	3000 2480 3200 2480
Wire Wire Line
	3010 2580 3200 2580
Wire Wire Line
	3010 2680 3200 2680
Wire Wire Line
	3010 2780 3200 2780
Wire Wire Line
	3010 2880 3200 2880
Wire Wire Line
	3010 2980 3200 2980
Wire Wire Line
	4400 2880 4570 2880
Wire Wire Line
	4400 2780 4570 2780
Wire Wire Line
	4400 2680 4570 2680
Wire Wire Line
	4400 2580 4570 2580
Wire Wire Line
	4400 2480 4570 2480
Wire Wire Line
	4400 2380 4570 2380
Wire Wire Line
	4400 2280 4570 2280
Wire Wire Line
	4400 2180 4570 2180
Wire Wire Line
	4400 2080 4560 2080
Wire Wire Line
	4400 1980 4570 1980
Wire Wire Line
	4400 1880 4570 1880
Wire Notes Line
	2480 3290 4980 3290
Wire Notes Line
	2480 1390 4980 1390
Wire Notes Line
	2480 1390 2480 3290
Text Notes 2500 3280 0    50   ~ 0
ESP-12E
$Comp
L power:GND #PWR?
U 1 1 625FD291
P 3910 3220
F 0 "#PWR?" H 3910 2970 50  0001 C CNN
F 1 "GND" V 3915 3092 50  0000 R CNN
F 2 "" H 3910 3220 50  0001 C CNN
F 3 "" H 3910 3220 50  0001 C CNN
	1    3910 3220
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 3180 3800 3220
Wire Wire Line
	3800 3220 3910 3220
Wire Wire Line
	3530 1520 3800 1520
Wire Wire Line
	3800 1520 3800 1680
$Comp
L Device:CP1 C?
U 1 1 625FD29B
P 4020 1520
F 0 "C?" V 3820 1540 50  0000 C CNN
F 1 "100uf" V 3890 1540 50  0000 C CNN
F 2 "" H 4020 1520 50  0001 C CNN
F 3 "~" H 4020 1520 50  0001 C CNN
	1    4020 1520
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3870 1520 3800 1520
Connection ~ 3800 1520
$Comp
L power:GND #PWR?
U 1 1 625FD2A3
P 4270 1520
F 0 "#PWR?" H 4270 1270 50  0001 C CNN
F 1 "GND" V 4275 1392 50  0000 R CNN
F 2 "" H 4270 1520 50  0001 C CNN
F 3 "" H 4270 1520 50  0001 C CNN
	1    4270 1520
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4170 1520 4270 1520
Text GLabel 5380 2030 0    50   Input ~ 0
GPIO0
Text GLabel 5380 2270 0    50   Input ~ 0
GPIO2
Text GLabel 5370 2440 0    50   Input ~ 0
EN
Text GLabel 6790 2440 2    50   Input ~ 0
RST
Text GLabel 5410 2800 0    50   Input ~ 0
GPIO15
$Comp
L Device:R R?
U 1 1 625FD2AF
P 5690 2030
F 0 "R?" V 5600 2020 50  0000 C CNN
F 1 "12K" V 5690 2020 50  0000 C CNN
F 2 "" V 5620 2030 50  0001 C CNN
F 3 "~" H 5690 2030 50  0001 C CNN
	1    5690 2030
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD2B5
P 5700 2270
F 0 "R?" V 5610 2260 50  0000 C CNN
F 1 "12K" V 5700 2260 50  0000 C CNN
F 2 "" V 5630 2270 50  0001 C CNN
F 3 "~" H 5700 2270 50  0001 C CNN
	1    5700 2270
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD2BB
P 5710 2440
F 0 "R?" V 5620 2430 50  0000 C CNN
F 1 "12K" V 5710 2430 50  0000 C CNN
F 2 "" V 5640 2440 50  0001 C CNN
F 3 "~" H 5710 2440 50  0001 C CNN
	1    5710 2440
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD2C1
P 6450 2440
F 0 "R?" V 6360 2430 50  0000 C CNN
F 1 "12K" V 6450 2430 50  0000 C CNN
F 2 "" V 6380 2440 50  0001 C CNN
F 3 "~" H 6450 2440 50  0001 C CNN
	1    6450 2440
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD2C7
P 5710 2800
F 0 "R?" V 5620 2790 50  0000 C CNN
F 1 "12K" V 5710 2790 50  0000 C CNN
F 2 "" V 5640 2800 50  0001 C CNN
F 3 "~" H 5710 2800 50  0001 C CNN
	1    5710 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	5380 2030 5540 2030
Wire Wire Line
	5380 2270 5550 2270
Wire Wire Line
	5370 2440 5560 2440
Wire Wire Line
	6790 2440 6680 2440
Wire Wire Line
	5410 2800 5560 2800
Wire Wire Line
	5840 2030 6210 2030
Wire Wire Line
	6210 2030 6210 1820
Wire Wire Line
	5850 2270 6210 2270
Wire Wire Line
	6210 2270 6210 2030
Connection ~ 6210 2030
Wire Wire Line
	6210 2440 6210 2270
Wire Wire Line
	5860 2440 6210 2440
Connection ~ 6210 2270
Wire Wire Line
	6300 2440 6210 2440
Connection ~ 6210 2440
$Comp
L power:GND #PWR?
U 1 1 625FD2DC
P 6680 2790
F 0 "#PWR?" H 6680 2540 50  0001 C CNN
F 1 "GND" H 6685 2617 50  0000 C CNN
F 2 "" H 6680 2790 50  0001 C CNN
F 3 "" H 6680 2790 50  0001 C CNN
	1    6680 2790
	1    0    0    -1  
$EndComp
Wire Wire Line
	6680 2790 6680 2730
Connection ~ 6680 2440
Wire Wire Line
	6680 2440 6600 2440
$Comp
L Device:CP_Small C?
U 1 1 625FD2E5
P 6680 2630
F 0 "C?" H 6780 2620 50  0000 L CNN
F 1 "470pf" H 6720 2550 50  0000 L CNN
F 2 "" H 6680 2630 50  0001 C CNN
F 3 "~" H 6680 2630 50  0001 C CNN
	1    6680 2630
	1    0    0    -1  
$EndComp
Wire Wire Line
	6680 2530 6680 2440
$Comp
L power:GND #PWR?
U 1 1 625FD2EC
P 6010 2850
F 0 "#PWR?" H 6010 2600 50  0001 C CNN
F 1 "GND" H 6015 2677 50  0000 C CNN
F 2 "" H 6010 2850 50  0001 C CNN
F 3 "" H 6010 2850 50  0001 C CNN
	1    6010 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5860 2800 6010 2800
Wire Wire Line
	6010 2800 6010 2850
Wire Notes Line
	7090 1390 7090 3290
Wire Notes Line
	5010 3290 7090 3290
$Comp
L power:+3.3V #PWR?
U 1 1 625FD2F6
P 3530 1520
F 0 "#PWR?" H 3530 1370 50  0001 C CNN
F 1 "+3.3V" V 3545 1648 50  0000 L CNN
F 2 "" H 3530 1520 50  0001 C CNN
F 3 "" H 3530 1520 50  0001 C CNN
	1    3530 1520
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 625FD2FC
P 6210 1820
F 0 "#PWR?" H 6210 1670 50  0001 C CNN
F 1 "+3.3V" H 6225 1993 50  0000 C CNN
F 2 "" H 6210 1820 50  0001 C CNN
F 3 "" H 6210 1820 50  0001 C CNN
	1    6210 1820
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 625FD302
P 7270 1720
F 0 "#PWR?" H 7270 1570 50  0001 C CNN
F 1 "+3.3V" H 7285 1893 50  0000 C CNN
F 2 "" H 7270 1720 50  0001 C CNN
F 3 "" H 7270 1720 50  0001 C CNN
	1    7270 1720
	1    0    0    -1  
$EndComp
Wire Notes Line
	5010 1390 7090 1390
Wire Notes Line
	7110 1390 7110 3290
$Comp
L Device:R R?
U 1 1 625FD30A
P 7270 1940
F 0 "R?" H 7340 1986 50  0000 L CNN
F 1 "470R" H 7340 1895 50  0000 L CNN
F 2 "" V 7200 1940 50  0001 C CNN
F 3 "~" H 7270 1940 50  0001 C CNN
	1    7270 1940
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 625FD310
P 7270 2310
F 0 "D?" V 7309 2192 50  0000 R CNN
F 1 "BLUE LED" V 7218 2192 50  0000 R CNN
F 2 "" H 7270 2310 50  0001 C CNN
F 3 "~" H 7270 2310 50  0001 C CNN
	1    7270 2310
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD316
P 7270 2720
F 0 "R?" H 7320 2640 50  0000 L CNN
F 1 "470R" H 7270 2560 50  0000 L CNN
F 2 "" V 7200 2720 50  0001 C CNN
F 3 "~" H 7270 2720 50  0001 C CNN
	1    7270 2720
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 625FD31C
P 7580 3100
F 0 "SW?" H 7610 3260 50  0000 C CNN
F 1 "SW_Push" H 7590 3040 50  0000 C CNN
F 2 "" H 7580 3300 50  0001 C CNN
F 3 "~" H 7580 3300 50  0001 C CNN
	1    7580 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 625FD322
P 7910 3100
F 0 "#PWR?" H 7910 2850 50  0001 C CNN
F 1 "GND" V 7915 2972 50  0000 R CNN
F 2 "" H 7910 3100 50  0001 C CNN
F 3 "" H 7910 3100 50  0001 C CNN
	1    7910 3100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7270 1720 7270 1760
Wire Wire Line
	7270 2160 7270 2090
Wire Wire Line
	7270 2570 7270 2510
Wire Wire Line
	7270 2870 7270 2950
Wire Wire Line
	7270 2950 7380 2950
Text GLabel 7450 2510 2    50   Input ~ 0
GPIO16
Wire Wire Line
	7450 2510 7270 2510
Connection ~ 7270 2510
Wire Wire Line
	7270 2510 7270 2460
Text GLabel 7580 2610 2    50   Input ~ 0
RST
Wire Wire Line
	7510 2610 7580 2610
Wire Wire Line
	7780 3100 7910 3100
Wire Wire Line
	7380 3100 7380 2950
Connection ~ 7380 2950
Wire Wire Line
	7380 2950 7510 2950
Text GLabel 7940 1810 1    50   Input ~ 0
GPIO0
$Comp
L Switch:SW_Push SW?
U 1 1 625FD338
P 7940 2500
F 0 "SW?" H 7690 2410 50  0000 C CNN
F 1 "SW_Push" H 7920 2410 50  0000 C CNN
F 2 "" H 7940 2700 50  0001 C CNN
F 3 "~" H 7940 2700 50  0001 C CNN
	1    7940 2500
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD33E
P 7940 2050
F 0 "R?" H 8010 2096 50  0000 L CNN
F 1 "470R" H 8010 2005 50  0000 L CNN
F 2 "" V 7870 2050 50  0001 C CNN
F 3 "~" H 7940 2050 50  0001 C CNN
	1    7940 2050
	1    0    0    -1  
$EndComp
Wire Notes Line
	7110 1390 8260 1390
Wire Wire Line
	7940 1810 7940 1900
Wire Wire Line
	7940 2200 7940 2300
$Comp
L power:GND #PWR?
U 1 1 625FD347
P 7940 2840
F 0 "#PWR?" H 7940 2590 50  0001 C CNN
F 1 "GND" H 7945 2667 50  0000 C CNN
F 2 "" H 7940 2840 50  0001 C CNN
F 3 "" H 7940 2840 50  0001 C CNN
	1    7940 2840
	1    0    0    -1  
$EndComp
Wire Wire Line
	7940 2700 7940 2840
Wire Notes Line
	8260 1390 8260 3290
Wire Notes Line
	7110 3290 8260 3290
Text Notes 7110 3290 0    50   ~ 0
KEY SWITCH\n
Wire Wire Line
	7510 2610 7510 2950
$Comp
L esp8266-rescue:USB_B_Micro-Connector J?
U 1 1 625FD352
P 2820 4850
F 0 "J?" H 2877 5317 50  0000 C CNN
F 1 "USB_B_Micro" H 2877 5226 50  0000 C CNN
F 2 "" H 2970 4800 50  0001 C CNN
F 3 "~" H 2970 4800 50  0001 C CNN
	1    2820 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:D D?
U 1 1 625FD358
P 3380 4650
F 0 "D?" H 3380 4433 50  0000 C CNN
F 1 "D" H 3380 4524 50  0000 C CNN
F 2 "" H 3380 4650 50  0001 C CNN
F 3 "~" H 3380 4650 50  0001 C CNN
	1    3380 4650
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 625FD35E
P 3690 4500
F 0 "#PWR?" H 3690 4350 50  0001 C CNN
F 1 "+5V" H 3705 4673 50  0000 C CNN
F 2 "" H 3690 4500 50  0001 C CNN
F 3 "" H 3690 4500 50  0001 C CNN
	1    3690 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3120 4650 3230 4650
Wire Wire Line
	3530 4650 3690 4650
Wire Wire Line
	3690 4650 3690 4500
Text GLabel 3290 4850 2    50   Input ~ 0
D+
Text GLabel 3290 4950 2    50   Input ~ 0
D-
Wire Wire Line
	3120 4850 3290 4850
Wire Wire Line
	3120 4950 3290 4950
NoConn ~ 3120 5050
$Comp
L power:GND #PWR?
U 1 1 625FD36C
P 2760 5390
F 0 "#PWR?" H 2760 5140 50  0001 C CNN
F 1 "GND" H 2765 5217 50  0000 C CNN
F 2 "" H 2760 5390 50  0001 C CNN
F 3 "" H 2760 5390 50  0001 C CNN
	1    2760 5390
	1    0    0    -1  
$EndComp
Wire Wire Line
	2720 5250 2720 5390
Wire Wire Line
	2720 5390 2760 5390
Wire Wire Line
	2820 5250 2820 5390
Wire Wire Line
	2820 5390 2760 5390
Connection ~ 2760 5390
$Comp
L Interface_USB:CH340G U?
U 1 1 625FD377
P 4860 4930
F 0 "U?" H 4980 4370 50  0000 C CNN
F 1 "CH340G" H 4630 4370 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 4910 4380 50  0001 L CNN
F 3 "http://www.datasheet5.com/pdf-local-2195953" H 4510 5730 50  0001 C CNN
	1    4860 4930
	1    0    0    -1  
$EndComp
Text GLabel 4350 4830 0    50   Input ~ 0
D+
Text GLabel 4350 4930 0    50   Input ~ 0
D-
Wire Wire Line
	4350 4830 4460 4830
Wire Wire Line
	4350 4930 4460 4930
NoConn ~ 5260 4830
NoConn ~ 5260 4930
NoConn ~ 5260 5030
NoConn ~ 5260 5130
NoConn ~ 4460 4630
$Comp
L Device:C C?
U 1 1 625FD386
P 4760 4100
F 0 "C?" H 4875 4146 50  0000 L CNN
F 1 "100nf" H 4875 4055 50  0000 L CNN
F 2 "" H 4798 3950 50  0001 C CNN
F 3 "~" H 4760 4100 50  0001 C CNN
	1    4760 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 625FD38C
P 4940 3880
F 0 "#PWR?" H 4940 3630 50  0001 C CNN
F 1 "GND" V 4945 3752 50  0000 R CNN
F 2 "" H 4940 3880 50  0001 C CNN
F 3 "" H 4940 3880 50  0001 C CNN
	1    4940 3880
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4760 4330 4760 4250
Wire Wire Line
	4760 3950 4760 3880
Wire Wire Line
	4760 3880 4940 3880
$Comp
L Device:R R?
U 1 1 625FD395
P 5530 4530
F 0 "R?" V 5620 4420 50  0000 L CNN
F 1 "470R" V 5610 4510 50  0000 L CNN
F 2 "" V 5460 4530 50  0001 C CNN
F 3 "~" H 5530 4530 50  0001 C CNN
	1    5530 4530
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD39B
P 5530 4630
F 0 "R?" V 5620 4520 50  0000 L CNN
F 1 "470R" V 5610 4610 50  0000 L CNN
F 2 "" V 5460 4630 50  0001 C CNN
F 3 "~" H 5530 4630 50  0001 C CNN
	1    5530 4630
	0    1    1    0   
$EndComp
Wire Wire Line
	5260 4530 5380 4530
Wire Wire Line
	5260 4630 5380 4630
Text GLabel 5770 4530 2    50   Input ~ 0
RXD
Text GLabel 5770 4630 2    50   Input ~ 0
TXD
Wire Wire Line
	5680 4530 5770 4530
Wire Wire Line
	5680 4630 5770 4630
$Comp
L power:GND #PWR?
U 1 1 625FD3A7
P 4860 5620
F 0 "#PWR?" H 4860 5370 50  0001 C CNN
F 1 "GND" H 4865 5447 50  0000 C CNN
F 2 "" H 4860 5620 50  0001 C CNN
F 3 "" H 4860 5620 50  0001 C CNN
	1    4860 5620
	1    0    0    -1  
$EndComp
Wire Wire Line
	4860 5620 4860 5530
$Comp
L Device:Crystal Y?
U 1 1 625FD3AE
P 4190 5260
F 0 "Y?" V 4180 5290 50  0000 R CNN
F 1 "Crystal" H 4300 5410 50  0000 R CNN
F 2 "" H 4190 5260 50  0001 C CNN
F 3 "~" H 4190 5260 50  0001 C CNN
	1    4190 5260
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4190 5410 4460 5410
Wire Wire Line
	4460 5410 4460 5330
Wire Wire Line
	4190 5110 4190 5080
Wire Wire Line
	4190 5080 4460 5080
Wire Wire Line
	4460 5080 4460 5130
$Comp
L Device:C C?
U 1 1 625FD3B9
P 3800 5080
F 0 "C?" V 3548 5080 50  0000 C CNN
F 1 "22pf" V 3639 5080 50  0000 C CNN
F 2 "" H 3838 4930 50  0001 C CNN
F 3 "~" H 3800 5080 50  0001 C CNN
	1    3800 5080
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 625FD3BF
P 3790 5410
F 0 "C?" V 3660 5410 50  0000 C CNN
F 1 "22pf" V 3910 5420 50  0000 C CNN
F 2 "" H 3828 5260 50  0001 C CNN
F 3 "~" H 3790 5410 50  0001 C CNN
	1    3790 5410
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 5080 4190 5080
Connection ~ 4190 5080
Wire Wire Line
	3940 5410 4190 5410
Connection ~ 4190 5410
Wire Wire Line
	3640 5410 3640 5250
Wire Wire Line
	3640 5080 3650 5080
$Comp
L power:GND #PWR?
U 1 1 625FD3CB
P 3470 5250
F 0 "#PWR?" H 3470 5000 50  0001 C CNN
F 1 "GND" V 3475 5122 50  0000 R CNN
F 2 "" H 3470 5250 50  0001 C CNN
F 3 "" H 3470 5250 50  0001 C CNN
	1    3470 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	3470 5250 3640 5250
Connection ~ 3640 5250
Wire Wire Line
	3640 5250 3640 5080
$Comp
L Device:R R?
U 1 1 625FD3D4
P 5890 5060
F 0 "R?" V 5800 5050 50  0000 C CNN
F 1 "12K" V 5890 5040 50  0000 C CNN
F 2 "" V 5820 5060 50  0001 C CNN
F 3 "~" H 5890 5060 50  0001 C CNN
	1    5890 5060
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 625FD3DA
P 5890 5500
F 0 "R?" V 5980 5490 50  0000 C CNN
F 1 "12K" V 5890 5490 50  0000 C CNN
F 2 "" V 5820 5500 50  0001 C CNN
F 3 "~" H 5890 5500 50  0001 C CNN
	1    5890 5500
	0    1    1    0   
$EndComp
Wire Wire Line
	5260 5230 5580 5230
Wire Wire Line
	5580 5230 5580 5060
Wire Wire Line
	5580 5060 5690 5060
Wire Wire Line
	5260 5330 5580 5330
Wire Wire Line
	5580 5330 5580 5500
Wire Wire Line
	5580 5500 5690 5500
$Comp
L Transistor_BJT:S8050 Q?
U 1 1 625FD3E6
P 6440 4900
F 0 "Q?" H 6630 4946 50  0000 L CNN
F 1 "S8050" H 6630 4855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 6640 4825 50  0001 L CIN
F 3 "http://www.unisonic.com.tw/datasheet/S8050.pdf" H 6440 4900 50  0001 L CNN
	1    6440 4900
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:S8050 Q?
U 1 1 625FD3EC
P 6430 5610
F 0 "Q?" H 6620 5656 50  0000 L CNN
F 1 "S8050" H 6620 5565 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 6630 5535 50  0001 L CIN
F 3 "http://www.unisonic.com.tw/datasheet/S8050.pdf" H 6430 5610 50  0001 L CNN
	1    6430 5610
	1    0    0    -1  
$EndComp
Wire Wire Line
	6240 5060 6240 4900
Wire Wire Line
	6040 5060 6240 5060
Wire Wire Line
	6040 5500 6230 5500
Wire Wire Line
	6230 5500 6230 5610
Wire Wire Line
	6540 5100 6540 5290
Wire Wire Line
	6540 5290 5690 5290
Wire Wire Line
	5690 5290 5690 5500
Connection ~ 5690 5500
Wire Wire Line
	5690 5500 5740 5500
Wire Wire Line
	6530 5810 6150 5810
Wire Wire Line
	6150 5810 6150 5220
Wire Wire Line
	6150 5220 5690 5220
Wire Wire Line
	5690 5220 5690 5060
Connection ~ 5690 5060
Wire Wire Line
	5690 5060 5740 5060
Text GLabel 6630 4660 2    50   Input ~ 0
RST
Wire Wire Line
	6540 4700 6540 4660
Wire Wire Line
	6540 4660 6630 4660
Text GLabel 6660 5400 2    50   Input ~ 0
GPIO0
$Comp
L Device:C C?
U 1 1 625FD405
P 5780 4050
F 0 "C?" H 5895 4096 50  0000 L CNN
F 1 "100nf" H 5895 4005 50  0000 L CNN
F 2 "" H 5818 3900 50  0001 C CNN
F 3 "~" H 5780 4050 50  0001 C CNN
	1    5780 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 625FD40B
P 5530 4050
F 0 "C?" H 5280 4080 50  0000 L CNN
F 1 "10uf" H 5300 3960 50  0000 L CNN
F 2 "" H 5568 3900 50  0001 C CNN
F 3 "~" H 5530 4050 50  0001 C CNN
	1    5530 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5530 4200 5650 4200
Wire Wire Line
	5530 3900 5640 3900
$Comp
L power:+5V #PWR?
U 1 1 625FD413
P 5650 4200
F 0 "#PWR?" H 5650 4050 50  0001 C CNN
F 1 "+5V" H 5665 4373 50  0000 C CNN
F 2 "" H 5650 4200 50  0001 C CNN
F 3 "" H 5650 4200 50  0001 C CNN
	1    5650 4200
	-1   0    0    1   
$EndComp
Connection ~ 5650 4200
Wire Wire Line
	5650 4200 5780 4200
Wire Wire Line
	4860 4330 4860 4200
Wire Wire Line
	4860 4200 5170 4200
Connection ~ 5530 4200
$Comp
L power:GND #PWR?
U 1 1 625FD41E
P 5640 3900
F 0 "#PWR?" H 5640 3650 50  0001 C CNN
F 1 "GND" H 5645 3727 50  0000 C CNN
F 2 "" H 5640 3900 50  0001 C CNN
F 3 "" H 5640 3900 50  0001 C CNN
	1    5640 3900
	-1   0    0    1   
$EndComp
Connection ~ 5640 3900
Wire Wire Line
	5640 3900 5780 3900
Wire Notes Line
	2480 5890 7140 5890
Wire Notes Line
	2480 3330 2480 5890
Wire Notes Line
	2480 3330 7140 3330
Wire Notes Line
	7140 3330 7140 5890
Text Notes 2490 5880 0    50   ~ 0
USB TO UART\n
$Comp
L Device:R R?
U 1 1 625FD42B
P 8810 1920
F 0 "R?" H 8880 1966 50  0000 L CNN
F 1 "220K" H 8880 1875 50  0000 L CNN
F 2 "" V 8740 1920 50  0001 C CNN
F 3 "~" H 8810 1920 50  0001 C CNN
	1    8810 1920
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 625FD431
P 8810 2540
F 0 "R?" H 8880 2586 50  0000 L CNN
F 1 "100K" H 8880 2495 50  0000 L CNN
F 2 "" V 8740 2540 50  0001 C CNN
F 3 "~" H 8810 2540 50  0001 C CNN
	1    8810 2540
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 625FD437
P 8810 2910
F 0 "#PWR?" H 8810 2660 50  0001 C CNN
F 1 "GND" H 8815 2737 50  0000 C CNN
F 2 "" H 8810 2910 50  0001 C CNN
F 3 "" H 8810 2910 50  0001 C CNN
	1    8810 2910
	1    0    0    -1  
$EndComp
Wire Wire Line
	8810 2910 8810 2690
Wire Wire Line
	8810 2390 8810 2250
Text GLabel 8760 1700 0    50   Input ~ 0
ADC-EX
Wire Wire Line
	8760 1700 8810 1700
Wire Wire Line
	8810 1700 8810 1770
Text GLabel 8650 2250 0    50   Input ~ 0
ADC
Wire Wire Line
	8650 2250 8810 2250
Connection ~ 8810 2250
Wire Wire Line
	8810 2250 8810 2070
Wire Notes Line
	8280 1390 8280 3290
Wire Notes Line
	8280 1390 9100 1390
Wire Notes Line
	9100 1390 9100 3290
Wire Notes Line
	8280 3290 9100 3290
Text Notes 8300 3290 0    50   ~ 0
ADC PIN\n
$Comp
L Connector:Conn_01x08_Male J?
U 1 1 625FD44B
P 7520 4060
F 0 "J?" H 7628 4541 50  0000 C CNN
F 1 "Conn_01x08_Male" H 7628 4450 50  0000 C CNN
F 2 "" H 7520 4060 50  0001 C CNN
F 3 "~" H 7520 4060 50  0001 C CNN
	1    7520 4060
	1    0    0    -1  
$EndComp
Text GLabel 7880 3760 2    50   Input ~ 0
RST
Text GLabel 7880 3960 2    50   Input ~ 0
EN
Text GLabel 7880 4060 2    50   Input ~ 0
GPIO16
Text GLabel 7880 4160 2    50   Input ~ 0
GPIO14
Text GLabel 7880 4260 2    50   Input ~ 0
GPIO12
Text GLabel 7880 4360 2    50   Input ~ 0
GPIO13
Wire Wire Line
	7720 3760 7880 3760
Wire Wire Line
	7720 3860 7880 3860
Wire Wire Line
	7720 3960 7880 3960
Wire Wire Line
	7720 4060 7880 4060
Wire Wire Line
	7720 4160 7880 4160
Wire Wire Line
	7720 4260 7880 4260
Wire Wire Line
	7720 4360 7880 4360
$Comp
L power:+3.3V #PWR?
U 1 1 625FD45E
P 7900 4460
F 0 "#PWR?" H 7900 4310 50  0001 C CNN
F 1 "+3.3V" V 7915 4588 50  0000 L CNN
F 2 "" H 7900 4460 50  0001 C CNN
F 3 "" H 7900 4460 50  0001 C CNN
	1    7900 4460
	0    1    1    0   
$EndComp
Wire Wire Line
	7720 4460 7900 4460
$Comp
L Connector:Conn_01x06_Male J?
U 1 1 625FD465
P 7510 4930
F 0 "J?" H 7618 5311 50  0000 C CNN
F 1 "Conn_01x06_Male" H 7618 5220 50  0000 C CNN
F 2 "" H 7510 4930 50  0001 C CNN
F 3 "~" H 7510 4930 50  0001 C CNN
	1    7510 4930
	1    0    0    -1  
$EndComp
Text GLabel 7840 4730 2    50   Input ~ 0
CSO
Text GLabel 7840 4830 2    50   Input ~ 0
MISO
Text GLabel 7840 4930 2    50   Input ~ 0
GPIO9
Text GLabel 7840 5030 2    50   Input ~ 0
GPIO10
Text GLabel 7840 5130 2    50   Input ~ 0
MOSI
Text GLabel 7840 5230 2    50   Input ~ 0
SCLK
Wire Wire Line
	7710 4730 7840 4730
Wire Wire Line
	7710 4830 7840 4830
Wire Wire Line
	7710 4930 7840 4930
Wire Wire Line
	7710 5030 7840 5030
Wire Wire Line
	7710 5130 7840 5130
Wire Wire Line
	7710 5230 7840 5230
$Comp
L Connector:Conn_01x08_Male J?
U 1 1 625FD477
P 8460 4000
F 0 "J?" H 8568 4481 50  0000 C CNN
F 1 "Conn_01x08_Male" H 8568 4390 50  0000 C CNN
F 2 "" H 8460 4000 50  0001 C CNN
F 3 "~" H 8460 4000 50  0001 C CNN
	1    8460 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 625FD47D
P 8770 3700
F 0 "#PWR?" H 8770 3450 50  0001 C CNN
F 1 "GND" V 8775 3572 50  0000 R CNN
F 2 "" H 8770 3700 50  0001 C CNN
F 3 "" H 8770 3700 50  0001 C CNN
	1    8770 3700
	0    -1   -1   0   
$EndComp
Text GLabel 8770 3800 2    50   Input ~ 0
GPIO15
Text GLabel 8770 3900 2    50   Input ~ 0
GPIO2
Text GLabel 8770 4000 2    50   Input ~ 0
GPIO0
Text GLabel 8770 4100 2    50   Input ~ 0
GPIO4
Text GLabel 8770 4200 2    50   Input ~ 0
GPIO5
Text GLabel 8770 4300 2    50   Input ~ 0
RXD
Text GLabel 8770 4400 2    50   Input ~ 0
TXD
Wire Wire Line
	8660 3700 8770 3700
Wire Wire Line
	8660 3800 8770 3800
Wire Wire Line
	8660 3900 8770 3900
Wire Wire Line
	8660 4000 8770 4000
Wire Wire Line
	8660 4100 8770 4100
Wire Wire Line
	8660 4200 8770 4200
Wire Wire Line
	8660 4300 8770 4300
Wire Wire Line
	8660 4400 8770 4400
Wire Notes Line
	7170 3330 9100 3330
Wire Notes Line
	7170 3330 7170 5890
Wire Notes Line
	7170 5890 9100 5890
Wire Notes Line
	9100 3330 9100 5890
Text Notes 7190 5880 0    50   ~ 0
GPIO PINS\n
Wire Wire Line
	6530 5410 6530 5400
Wire Wire Line
	6530 5400 6660 5400
$Comp
L power:PWR_FLAG #FLG?
U 1 1 625FD499
P 5170 4200
F 0 "#FLG?" H 5170 4275 50  0001 C CNN
F 1 "PWR_FLAG" H 5170 4373 50  0000 C CNN
F 2 "" H 5170 4200 50  0001 C CNN
F 3 "~" H 5170 4200 50  0001 C CNN
	1    5170 4200
	1    0    0    -1  
$EndComp
Connection ~ 5170 4200
Wire Wire Line
	5170 4200 5530 4200
$Comp
L power:PWR_FLAG #FLG?
U 1 1 625FD4A1
P 7270 1760
F 0 "#FLG?" H 7270 1835 50  0001 C CNN
F 1 "PWR_FLAG" V 7270 1888 50  0000 L CNN
F 2 "" H 7270 1760 50  0001 C CNN
F 3 "~" H 7270 1760 50  0001 C CNN
	1    7270 1760
	0    1    1    0   
$EndComp
Connection ~ 7270 1760
Wire Wire Line
	7270 1760 7270 1790
Text GLabel 7880 3860 2    50   Input ~ 0
ADC-EX
$EndSCHEMATC
