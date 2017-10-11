##################################################################
#
# UTF configuration for kpiblr01 ( KPI Phase 2 testbed ) SEPTEMBER 2015              
# Testbed location : 5th Floor , Bldg 190 , Mathilda Place  
#                                         
################Station Generic Information########################
#
# Controller  kpiblr01end1   10.131.80.187 
# AP1         4708/4360  		 192.168.1.1      NPC PORT 1  ATTN G1
# AP2         4708/4360  		 192.168.1.2      NPC PORT 2  ATTN G2
# AP HOST     kpiblr01aphost   	 10.131.80.197   NPC PORT 3  
# LINUX DHD   kpiblr01dhd   	 10.131.80.196   NPC PORT 4  ATTN G3
# MACBOOK PRO kpiblr01tst1   	 10.131.80.171   DUT
# MACBOOK PRO kpiblr01tst2   	 10.131.80.173   DUT
# MACBOOK AIR kpiblr01tst3   	 10.131.80.174   DUT
# MACBOOK AIR kpiblr01tst4       10.131.80.175   DUT
# MACBOOK PRO kpiblr01stressor   10.131.80.178   STRESSOR
# LINUX BOX   kpiblr01snf1   	 10.131.80.170   SNIFFER1 -> AP
# LINUX BOX2  kpiblr01snf2  	 10.131.80.171   SNIFFER2 -> STA   -> For Temporary purpose using blr16tst1 : 10.131.80.113 as sniffer as Brix was not working.
#
# 
#
# Attenuator       Aefoflex AF1         172.16.1.106
# Attenuator       Aeroflex AF2         172.16.1.107
#
# CISCO SG300-10  Distribution Switch   172.16.1.100
# CISCO SG300-10  Distribution Switch   172.16.1.101
# CISCO SG300-10  SPAN SWITCH AP1       172.16.1.103
# CISCO SG300-19  SPAN SWITCH AP2       172.16.1.104
# 
# NPC 8 PORT      POWER CONTROLLER      172.16.1.108
#
# PORT 1(AP1) PORT 2(AP2) PORT 3(APHOST)  PORT 4(LINUX DHD) PORT 5(SNIFFER 1)  PORT 6 (SNIFFER 2)  PORT 7(AF1)  PORT 8( AF2)
# 
# NPC 8 PORT      POWER CONTROLLER      172.16.1.109
#
# PORT 1(AP1) PORT 2(AP2) PORT 3(APHOST)  PORT 4(LINUX DHD) PORT 5(SNIFFER 1)  PORT 6 (SNIFFER 2)  PORT 7(AF1)  PORT 8( AF2)
#
# NPC 2 PORT      POWER CONTROLLER      172.16.1.110
#
# PORT 1(kpiblr01tst1) PORT 2(kpiblr01tst2)
#
# NPC 2 PORT      POWER CONTROLLER      172.16.1.111
#
# PORT 1(kpiblr01tst3) PORT 2(kpiblr01tst4)
#
#
# NPC 2 PORT      POWER CONTROLLER      172.16.1.123
#
#
#  PORT 2(kpiblr01tst5)
#
#
#AP1 (Netgear)L1     af1 1,5,9
#AP2 (Cisco)  L2     af1 2,6,10
#AP3 (Linksys)L3     af1 3,7,11
#BT           L4     af1 4,8,12
#MBP          L5     af2 1,5,9 
#MacBook      L6     af2 2,6,10
#sressor      L7     af2 3,7,11
#MBA          L8     af2 4,8,12
#
# Mirror Port : 
# 172.16.1.103 : port 3
# port 3 : AP1 Box (brcm ref ap)
# 
#
# 172.16.1.104 : port 3
#
# port 3 : SOFTAP Box (Netgear R7000)
#####################################################################

set ::UTF::SummaryDir "/projects/hnd_sig_ext21/anujgupt/kpiblr04"

# package require UTFD
# set ::env(UTFDPORT) 9977
package require UTF::Power
package require UTF::Q
package require UTF::AeroflexDirect
package require UTF::Aeroflex
package require UTF::Airport
package require UTF::WebRelay
package require UTF::Streams
package require UTF::StaticAP
package require UTF::Sniffer
package require UTF::MacBT_dev
package require UTF::Test::ConnectAPSTA
package require UTF::Test::APChanspec
package require UTF::FTTR
package require UTF::Test::ConfigBridge
package require UTF::Test::AutoJoinSetup


# Packages commonly used in interactive mode
if {[info exists ::tcl_interactive] && $::tcl_interactive} {
    package require UTF::Test::ConnectAPSTA
    package require UTF::Test::APChanspec
    package require UTF::FTTR
    package require UTF::Test::ConfigBridge
}

set ::afmaxattn 95

set ::UTF::MSTimeStamps 1

set ::UTF::TrunkApps 1 \

set ::UTF::IBSSWithAP 1 \

set ::UTF::FBDefault 1 \

set ::UTF::ChannelPerf 1 \

set ::DRIVER "7_21_171_7"

set OS "16A247a"


######################## Power Controllers ################

UTF::Power::Synaccess npc102 -lan_ip  192.168.1.102 -relay kpiblr04aphost -rev 1
UTF::Power::Synaccess npc104 -lan_ip  172.16.1.104 -relay kpiblr02end1 -rev 1
UTF::Power::Synaccess npc104 -lan_ip  172.16.1.103 -relay kpiblr02end1 -rev 1
#UTF::Power::Synaccess npc110 -lan_ip 172.16.1.110 -relay kpi01end1 -rev 1
#UTF::Power::Synaccess npc111 -lan_ip  172.16.1.111 -relay kpi01end1 -rev 1
#UTF::Power::Synaccess npc123 -lan_ip  172.16.1.123 -relay kpi01end1 -rev 1

####################### Attenuator  ######################



UTF::AeroflexDirect af1  -lan_ip 192.168.1.106 -cmdresptimeout 500 -retries 0 -concurrent 0 -silent 0 \
       -relay kpiblr04aphost \
		-power {npc108 7} \
        -group {
                L1 {1  2  3}
                L2 {4  5  6}
                L3 {7  8  9}
				L4 {10  11  12}
                ALL1 {1 2 3 4 5 6 7 8 9 10 11 12}
                }


L1 configure -default 0
L2 configure -default 0
L3 configure -default 0
L4 configure -default 95



########################################### test bed setup #################

set UTF::SetupTestBed {
    L1 attn 95
    L2 attn 95
    L3 attn 95
    L4 attn 95
	UTF::Sleep 2
	# L3 attn 30
	#ALL1 attn default
	#ALL2 attn default
  # foreach STA $physicalmacstas {
	#set tag INFO
	# if {[catch {$STA power on}]} {
	    # set tag ERROR
	# }
	# $STA rexec networksetup -setairportpower [$(sta) cget -device] on
#	UTF::Message $tag "" "AC power on and Radio on for $STA"
    # }
 #   foreach S [concat $physicalmacstas $physicalrouterstas] {
	#catch {$S wl down}
	#$S deinit
	#MacX-A rexec "wl ampdu_clear_dump"
	
	#foreach STA {MacX-A MacX-B MacAirX-A MacAirX-B MacBook-B DOME_STRESS} {}
	foreach STA {MacX-B MacAirX-B} {
	set tag INFO
	if {[catch {0 && $STA power on}]} {
	    set tag ERROR
	}
	if {[$STA hostis MacOS]} {
	catch {$STA killall dns-sd}
	catch {$STA wl awdl 0}
	UTF::Sleep 1
	$STA rexec "/usr/local/bin/applebt --setPowerState 0"
	$STA rexec "/usr/local/bin/applebt --getPowerState"
	$STA rexec networksetup -listpreferredwirelessnetworks [$(sta) cget -device]
	$STA rexec networksetup -removeallpreferredwirelessnetworks [$(sta) cget -device]
	$STA rexec networksetup -setairportpower [$(sta) cget -device] off
	# UTF::Sleep 5
	# $STA rexec networksetup -setairportpower [$(sta) cget -device] on
	UTF::Sleep 1
	} else {
	catch {$STA wl down}
	}
}
#    }

    set sniffers [UTF::Sniffer info instances]
    foreach sniffer $sniffers {
	catch ($sniffer unload)
    }
    return
}

## $physicalmacstas - > $physicalmacstastas

############## Controller and hosts####################

UTF::Linux kpiblr04aphost -lan_ip 10.131.78.63 \
                    -sta {lan em2 mirror1 p7p2 mirror2 p7p3} \

lan configure -ipaddr 192.168.1.51 \


################### Sniffer ###########################

UTF::Sniffer SNIF_AP \
	-lan_ip 10.131.80.170 \
	-user root \
	-tag BISON_BRANCH_7_10 \
	-app_tag BISON_BRANCH_7_10 \
	-sta {4360SNIF_AP p3p1} \
	-power {npc108 5} \


UTF::Sniffer SNIF_STA \
   -user root \
   -tag EAGLE_BRANCH_10_10 \
   -app_tag EAGLE_BRANCH_10_10 \
   -sta {4360SNIF_STA  enp1s0} \
   -power {npc108 6}


################# Routers ##############################

set LANPEER lan
# set LANPEER ApHostB

UTF::Router NETGEAR \
    -sta {NETGEAR_2G eth1 NETGEAR_5G eth2} \
    -brand linux-2.6.36-arm-internal-router \
    -tag "BISON04T_REL_7_14_43_22" \
    -power {npc102 1} \
    -console 10.131.78.63:40000 \
    -relay "lan" \
    -lanpeer lan \
    -lan_ip 192.168.1.3 \
    -datarate {-b 1.2G -i 0.5 -frameburst 1} \
    -tcpwindow 4M -udp 1.2G \
    -slowassoc 10 \
    -portmirror mirror1 \
    -post_perf_hook {{%S wl rate} {%S wl nrate} {%S wl dump rssi}} \
    -nvram {
        lan_ipaddr=192.168.1.3
        wl0_ssid=NETGEAR_2G
		wl0_country_code=US
        wl1_ssid=NETGEAR_5G
		wl1_country_code=US
		wl0_frameburst=on
		wl1_frameburst=on
    } \
    -wlinitcmds { sleep 5; wl -i eth1 country US; wl -i eth2 country US; wl -i eth1 frameburst 1; wl -i eth2 frameburst 1}
	

NETGEAR_2G configure -attngrp L3
NETGEAR_5G configure -attngrp L3



UTF::Router LINKSYSEA3500 \
    -sta {LINKSYS eth1} \
    -brand linux-2.6.36-arm-internal-router \
    -tag "1.1.40.162464" \
    -power {npc108 3} \
    -relay "lan" \
    -lanpeer lan \
    -lan_ip 192.168.1.2 \
    -datarate {-b 1.2G -i 0.5 -frameburst 1} \
    -tcpwindow 4M -udp 1.2G \
    -slowassoc 10 \
    -post_perf_hook {{%S wl rate} {%S wl nrate} {%S wl dump rssi}} \
    -portmirror mirror2 \
    -nvram {
        lan_ipaddr=192.168.1.2
        wl0_ssid=LINKSYSKPI2G
		wl0_country_code=US
        wl1_ssid=LINKSYSKPI5G
		wl1_country_code=US
    } \
    -wlinitcmds { sleep 5; wl -i eth1 country US; wl -i eth2 country US}
	

LINKSYS configure -attngrp L2


###################### Cisco AP ############

UTF::StaticAP Cisco \
    -sta {
	CISCO_2G
		{
		-ssid CISCO_TEST_2G
		-security open
		
		}
	CISCO_5G
	{
	   -ssid CISCO_TEST_5G
	}
	}	\
    -lanpeer "lan" \
    -lan_ip 192.168.1.1 \

CISCO_2G configure -attngrp L1
CISCO_5G configure -attngrp L1


################# DUT configuration #############################

UTF::MacOS kpiblr04tst1 -sta {MacX-A en0 AMacX-A awdl0} \
    -wlinitcmds { Sleep 5; wl msglevel 0x101; wl down; wl vht_features 3; wl up ; wl assert_type 1; apple80211 -debug=err,rsn,scan } \
        -brand  "macos-external-wl-gala" \
		-tag BIS715GALA_REL_7_21_94_25 \
		-app_tag BIS715GALA_REL_7_21_94_25 \
        -name "MacX-A" \
        -power {npc102 1} \
        -type Release_10_11 \
        -channelsweep {-count 15} \
        -lan_ip 10.131.78.64 \
        -kextload false \
        -slowassoc 5 \
        -datarate {-b 1.2G -i 0.5 -frameburst 1} \
        -nativetools 1 \
		-post_assoc_hook {%S innetworkwait $AP 20} \
        -console "/var/log/system.log" \
        -nobighammer 1 -tcpwindow 4M -custom 1

MacX-A configure -ipaddr 192.168.1.119
AMacX-A configure -ipaddr 192.168.5.119
	
MacX-A clone MacX-$OS-$::DRIVER-A \
	-tag BIS715GALA_REL_$::DRIVER \
	-app_tag BIS715GALA_REL_$::DRIVER \
	-type Release_10_12 \
    -brand macos-external-wl-10_12		
	
UTF::MacOS kpiblr04tst2 -sta {MacX-B en0 AMacX-B awdl0} \
    -wlinitcmds { Sleep 5; wl msglevel 0x101; wl down; wl vht_features 3; wl up ; wl assert_type 1; apple80211 -debug=err,rsn,scan } \
        -brand  "macos-external-wl-10_12" \
		-tag BIS715GALA_REL_7_21_120 \
		-app_tag BIS715GALA_REL_7_21_120 \
        -name "MacX-B" \
        -power {npc102 2} \
        -type Release_10_12 \
        -kextload false \
        -slowassoc 5 \
        -lan_ip 10.131.78.65 \
        -channelsweep {-count 15} \
        -datarate {-b 1.2G -i 0.5 -frameburst 1} \
		-yart {-attn5g 0-95 -attn2g 0-95 -pad 23} \
        -nativetools 1 \
		-pre_perf_hook {{%S wl rate} {%S wl nrate} {%S wl rssi} {%S wl phy_rssi_ant} {%S wl dump rssi} {%S wl rxchain} {%S wl txchain}} \
		-post_perf_hook {{%S wl rate} {%S wl nrate} {%S wl rssi} {%S wl phy_rssi_ant} {%S wl dump rssi} {%S wl rxchain} {%S wl txchain}} \
		-post_assoc_hook {%S innetworkwait $AP 20} \
        -nobighammer 1 -tcpwindow 4M -custom 1

MacX-B configure -ipaddr 192.168.1.147
AMacX-B configure -ipaddr 192.168.5.147

MacX-B clone MacX-16A99-ext-B

	
MacX-B clone MacX-$OS-$::DRIVER-B \
	-tag BIS715GALA_REL_$::DRIVER \
	-app_tag BIS715GALA_REL_$::DRIVER \
	-type Release_10_12 \
    -brand macos-external-wl-10_12	
	
UTF::MacOS kpiblr04tst3 -sta {MacAirX-A en0 AMacAirX-A awdl0} \
    -wlinitcmds { Sleep 5; wl msglevel 0x101; wl down; wl vht_features 3; wl up ; wl assert_type 1; apple80211 -debug=err,rsn,scan } \
        -brand  "macos-external-wl-gala" \
		-tag BIS715GALA_REL_7_21_94_25 \
		-app_tag BIS715GALA_REL_7_21_94_25 \
        -name "MacAirX-A" \
        -power {npc111 1} \
        -type Release_10_11 \
        -kextload false \
        -lan_ip 10.131.78.66 \
        -slowassoc 5 \
        -channelsweep {-count 15} \
        -datarate {-b 1.2G -i 0.5 -frameburst 1} \
        -nativetools 1 \
        -nobighammer 1 -tcpwindow 4M -custom 1

MacAirX-A configure -ipaddr 192.168.1.74 
AMacAirX-A configure -ipaddr 192.168.5.74 

MacAirX-A clone MacAirX-$OS-$::DRIVER-A \
	-tag BIS715GALA_REL_$::DRIVER \
	-app_tag BIS715GALA_REL_$::DRIVER \
	-type Release_10_12 \
    -brand macos-external-wl-10_12	

UTF::MacOS kpiblr04tst4 -sta {MacAirX-B en0 AMacAirX-B awdl0} \
    -wlinitcmds { Sleep 5; wl msglevel 0x101; wl down; wl vht_features 3; wl up ; wl scansuppress 1; wl assert_type 1; apple80211 -debug=err,rsn,scan } \
        -brand  "macos-external-wl-10_12" \
		-tag BIS715GALA_REL_7_21_94_25 \
		-app_tag BIS715GALA_REL_7_21_94_25 \
        -name "MacAirX-B" \
        -power {npc111 2} \
        -type Release_10_12 \
        -lan_ip 10.131.78.67 \
        -kextload false \
        -slowassoc 5 \
        -channelsweep {-count 15} \
        -datarate {-b 1.2G -i 0.5 -frameburst 1} \
        -nativetools 1 \
        -nobighammer 1 -tcpwindow 4M -custom 1 \
		-yart {-attn5g 0-95 -attn2g 0-95 -pad 23} \
		-pre_perf_hook {{%S wl rate} {%S wl nrate} {%S wl rssi} {%S wl phy_rssi_ant} {%S wl dump rssi}} \
		-post_perf_hook {{%S wl rate} {%S wl nrate} {%S wl rssi} {%S wl phy_rssi_ant} {%S wl dump rssi}}

MacAirX-B configure -ipaddr 192.168.1.75
AMacAirX-B configure -ipaddr 192.168.5.75

MacAirX-B clone MacAirX-$OS-$::DRIVER-B \
	-tag BIS715GALA_REL_$::DRIVER \
	-app_tag BIS715GALA_REL_$::DRIVER \
	-type Release_10_12 \
    -brand macos-external-wl-10_12

######################## MacBook #########################

UTF::MacOS kpiblr04tst6 -sta {MacBook-B en0 AMacBook-B awdl0} \
    -wlinitcmds {Sleep 5; wl msglevel 0x101; wl down; wl vht_features 3; wl up ; wl assert_type 1; apple80211 -debug=err,rsn,scan } \
        -brand  "macos-external-wl-10_12" \
		-tag BIS715GALA_REL_7_21_137 \
		-app_tag BIS715GALA_REL_7_137 \
        -name "MacBook-B" \
        -power {npc104 1} \
        -type Release_10_12 \
        -lan_ip 10.131.78.69 \
        -kextload false \
        -slowassoc 5 \
        -channelsweep {-count 15} \
        -datarate {-b 1.2G -i 0.5 -frameburst 1} \
        -nativetools 1 \
        -nobighammer 1 -tcpwindow 4M -custom 1
		
MacBook-B configure -ipaddr 192.168.1.68 
AMacBook-B configure -ipaddr 192.168.5.68

MacBook-B clone MacBook-$OS-$::DRIVER-B \
	-tag BIS715GALA_REL_$::DRIVER \
	-app_tag BIS715GALA_REL_$::DRIVER \
	-type Release_10_12 \
    -brand macos-external-wl-10_12
		
######################## Stressor ###########################

UTF::MacOS kpiblr02stressor -sta {DOME_STRESS en0 PDOME_STRESSOR awdl0} \
    -wlinitcmds { Sleep 5; wl msglevel 0x101; wl down; wl vht_features 3; wl up ; wl assert_type 1; apple80211 -debug=err,rsn,scan;} \
        -brand  "macos-internal-wl-syr" \
        -name "DOME_STRESS" \
		-power {npc104 2} \
        -type Debug_10_10 \
        -lan_ip 10.131.80.188 \
        -kextload false \
        -slowassoc 5 \
        -channelsweep {-count 15} \
        -datarate {-b 1.2G -i 0.5 -frameburst 1} \
        -nativetools 0 \
        -console "/var/log/system.log" \
        -nobighammer 1 -tcpwindow 4M -custom 1


DOME_STRESS configure -tag "BIS120RC4_REL_7_15_166_24" -custom 1
DOME_STRESS configure -ipaddr 192.168.1.76
DOME_STRESS configure -attngrp L7




##################### Mac Custom tools configurations #######################################

proc ::update_mac_tools {{devices ""}} {
    # set MACIPERF "/projects/hnd_sig_ext16/rmcmahon/Code/KPI/mac/iperf/iperf2-code/src/iperf"
   # set MACIPERF "/projects/hnd_sig_ext16/rmcmahon/Code/iperf/sourceforge/counters/iperf2-code/src/iperf"
    if {$devices eq {}} {
	set devices {MacX-A DOME_STRESS}
    }
    foreach DUT $devices {
#	catch {eval [concat $DUT copyto $MACIPERF /usr/bin/iperf]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf_awdl /usr/bin/iperf_awdl]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf /usr/bin/iperf]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/10.10/wl /usr/bin/wl]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/ptp/ptpd2 /usr/bin/ptpd2]}
	catch {$DUT rexec mkdir /usr/local/bin}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/applebt/applebt /usr/local/bin/applebt]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/wget/wget /usr/bin/wget]}
	$DUT deinit
    }
}

## BT objects ##

##################### PTP Configurations #######################################

#array set ::ptpinterfaces [list lan enp3s0f0 MacX-A en5 MacX-B en3 MacAirX-A en4 MacAirX-B en3 DOME_STRESS en4 ApHostB eth0 MacBook-B en2]
#array set ::ptpinterfaces [list MacX-A en3 MacX-B en3 MacAirX-B en2 MacAirX-A en2 DOME_STRESS en3 lan em1 MacBook-B en1]
array set ::ptpinterfaces [list lan em1 MacX-B en3 MacAirX-B en2]
proc ::enable_ptp {args} {
    if {![llength $args]} {
	set devices [array names ::ptpinterfaces]
    }  else {
	set devices $args
    }
    foreach dut $devices {
	if {[catch {$dut uname -r} err]} {
	    UTF::Message WARN $dut $err
	    continue
	}
	set interface $::ptpinterfaces([namespace tail $dut])
	if {[$dut hostis MacOS] && ![catch {$dut uname -r}]} {
	     catch {$dut rexec pkill ptpd2}
		catch {$dut rexec /usr/bin/ptpd2 -s -i $interface}
		UTF::Sleep 2
	    catch {$dut rexec rm /var/log/ptp*.log}
	    catch {$dut rexec /usr/bin/ptpd2 -L -s -f /var/log/ptp.log -S /var/log/ptpstats.log -i $interface}
	    # catch {$dut rexec route add -host 224.0.0.107 -interface $interface}
	    # catch {$dut rexec route add -host 224.0.1.129 -interface $interface}
	} else {
	     catch {$dut rexec systemctl stop ntpd.service}
	    catch {$dut rexec pkill ptpd2}
	    catch {$dut rm /var/log/ptp*.log}
	   #catch {$dut rexec /projects/hnd_sig/anujgupt/KPI/ptp_230_fc19/ptpd2 -s -f /var/log/ptp.log -S /var/log/ptpstats.log -i $interface}
	 # catch {$dut ip route replace multicast 224.0.0.107/32 dev $interface}
	 # catch {$dut ip route replace multicast 224.0.1.129/32 dev $interface}
	    # catch {$dut rexec route del -net 224.0.0.107 netmask 255.255.255.255}
	    # if {[catch {$dut rexec route add -net 224.0.0.107 netmask 255.255.255.255 dev $interface} err]} {
		# UTF::Message WARN $dut $err
	    # }
	    # catch {$dut rexec route del -net 224.0.1.129 netmask 255.255.255.255}
	    # if {[catch {$dut rexec route add -net 224.0.1.129 netmask 255.255.255.255 dev $interface} err]} {
		# UTF::Message WARN $dut $err
	    # }
	#catch {$dut rexec pkill ptpd2}
	catch {$dut rexec /usr/bin/ptpd2 -M -i $interface}
    UTF::Sleep 2
	}
    }
    UTF::Sleep 10
    foreach dut $devices {
	catch {$dut rexec tail -10 /var/log/ptpstats.log}
	$dut deinit
    }
}



############################## iperf 2.0.8 copy ################
 
proc ::iperf208_copy {{devices ""}} {
 if {$devices eq {}} {
	set devices {MacX-A MacX-B MacAirX-A MacAirX-B MacBook-A MacBook-B kpiblr04aphost}
    }
	foreach DUT $devices {
	if {$DUT eq "kpiblr04aphost"} {
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf2.0.8/linux/iperf /usr/local/bin/iperf]}
	} else {
	#catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf_awdl /usr/bin/iperf_awdl]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf /usr/bin/iperf]}
	}
	}
}


############################## iperf 2.0.9 copy ################
 
proc ::iperf209_copy {{devices ""}} {
 if {$devices eq {}} {
	set devices {MacX-A MacX-B MacAirX-A MacAirX-B MacBook-A MacBook-B kpiblr04aphost}
    }
	foreach DUT $devices {
	if {$DUT eq "kpiblr04aphost"} {
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf2.0.9/linux/iperf209 /usr/local/bin/iperf]}
	} else {
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf2.0.9/mac/iperf /usr/bin/iperf_awdl]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf2.0.9/mac/iperf /usr/bin/iperf]}
	}
	}
}

##################### Check for init and crash report configurations #######################################

proc ::check_crash_report {{devices ""}} {
    if {$devices eq {}} {
	set devices {MacX-A MacX-B MacAirX-A MacAirX-B MacBook-A MacBook-B DOME_STRESS}
    }
    foreach DUT $devices {	
	catch {$DUT rexec ls /Library/Logs/CrashReporter/CoreCapture | grep reinit}
	catch {$DUT rexec ls /Library/Logs/DiagnosticReports | grep panic}
	
    }
}

##################### load the driver #######################################

proc ::load {{devices ""}} {
    if {$devices eq {}} {
	set devices {MacX-A MacX-B MacAirX-A MacBook-A MacBook-B DOME_STRESS}
    }
    foreach DUT $devices {
	catch {$DUT rexec cp /Users/user/Desktop/$::DRIVER/10_12/Tools/wl /usr/bin}
	catch {$DUT rexec /Users/user/Desktop/$::DRIVER/10_12/Release_10_12/install.sh}
	UTF::Sleep 30
    }
}




##################### Driver copy #######################################


proc ::driver_copy {{devices ""}} {
 if {$devices eq {}} {
	set devices {MacX-A MacX-B MacAirX-A MacAirX-B MacBook-B}
    }
	foreach DUT $devices {
	#catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/iperf/iperf2.0.9/mac/iperf /usr/bin/iperf]}
	catch {eval [concat $DUT copyto /projects/hnd_sig/anujgupt/KPI/Driver/$::DRIVER /Users/user/Desktop]}
	}
}



UTF::Q kpiblr04

