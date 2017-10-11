#Testbed configuration file for blr06end1
#Edited Battu kaushik Date 27june2014
#Last checkin 03SEP2013 12AM
####### Controller section:
# blr06end1: FC15
#
#
####### SOFTAP section:

# AP1:4360a
# AP2:4360b
####### STA section:
#
# blr06tst1: 4349 eth0 (10.131.80.32)
# blr06ref0: 4360a eth0 (10.132.30.30)
# blr06ref1: 4360b eth0 (10.132.30.31)

######################################################### #
# Load Packages
package require UTF::Aeroflex
package require UTF::Linux
package require UTF::Sniffer
package require UTF::Power
package require UTF::Airport
package require UTF::Vaunix


UTF::Aeroflex af -lan_ip 172.1.1.22 \
        -relay "blr06end1" \
        -group {
                G1 {1 2 3}
                G2 {4 5 6}
                G3 {7 8 9}
                ALL {1 2 3 4 5 6 7 8 9}
                }
G1 configure -default 10
G2 configure -default 10 
#G3 configure default 0
# Default TestBed Configuration Options

set ::UTF::SetupTestBed {
    #
    # Make Sure Attenuators are set to 0 value
    #
#making my own build#

		UTF::Try "build" {
        ::build
        return
}
    G1 attn 10 
    catch {G3 attn 10;}
    catch {G2 attn 10;}
    catch {G1 attn 10;}
}
# SummaryDir sets the location for test results
set ::UTF::SummaryDir "/projects/hnd_sig_ext18/$::env(LOGNAME)/blr06"

set UTF::Use11h 1

set UTF::SetupTestBed {
    G1 attn default
    foreach S {4360a 4360b 4349a0} {
	catch {$S wl down}
	$S deinit
    }
    return
}
package require UTF::Power
UTF::Power::Synaccess npc31 -lan_ip 172.1.1.31 -relay blr06end1 -rev 1

#pointing Apps to trunk
set ::UTF::TrunkApps 1 \

set ::UTF::FBDefault 1 \

set ::UTF::ChannelPerf 1 \

#set ::UTF::DisassocWARforRSDB 1 \

 
UTF::Linux blr06end1 \
     -lan_ip 10.131.80.29 \
     -sta {lan em1} \

UTF::Linux blr06ref0 \
     -lan_ip 10.131.80.30 \
     -sta {4360a eth0} \
    -slowassoc 5 -reloadoncrash 1 \
    -tag BISON_BRANCH_7_10 \
    -wlinitcmds {wl msglevel +assoc;wl down;wl country US/0;wl dtim 3;wl bw_cap 2g -1;wl vht_features 3} \
	-pre_perf_hook  {{%S wl ampdu_clear_dump} {%S wl amsdu_clear_counters}} \
    -post_perf_hook {{%S wl rssi} {%S wl rate} {%S wl nrate} {%S wl dump phycal} {%S wl dump rssi} {%S wl counters} {%S wl ampdu_clear_dump}} \

4360a configure -ipaddr 192.168.1.90 -attngrp G1  -hasdhcpd 1\

UTF::Linux blr06ref1 \
     -lan_ip 10.131.80.31 \
     -sta {4360b eth0} \
    -slowassoc 5 -reloadoncrash 1 \
    -tag BISON_BRANCH_7_10 \
	-pre_perf_hook  {{%S wl ampdu_clear_dump} {%S wl amsdu_clear_counters}} \
    -post_perf_hook {{%S wl rssi} {%S wl rate} {%S wl nrate} {%S wl dump phycal} {%S wl dump rssi} {%S wl counters} {%S wl ampdu_clear_dump}} \
    -brand "linux-internal-wl" \
    -wlinitcmds {
       wl msglevel +assoc;wl down;wl country US/0;wl dtim 3;wl bw_cap 2g -1;wl vht_features 3
    }


4360b configure -ipaddr 192.168.2.100  -ap 1 -attngrp G1 -hasdhcpd 1 \

###
###----- 4355B0 Pure MIMO----
###------4349B0/4359B0 RSDB/Automode----

UTF::DHD blr06tst1 \
     -lan_ip 10.131.80.32 \
     -sta {4349a0 eth0} \
	 -power "npc31 1" \
	 -dhd_brand linux-internal-dongle-pcie \
     -driver dhd-msgbuf-pciefd-debug \
     -tag CARIBOU_BRANCH_8_10 \
     -nocal 1 -slowassoc 5 \
     -nvram "bcm94359fcpagbss.txt" \
     -type 4349a0-roml/threadx-pcie-ag-p2p-mchan-wl11u-pktctx-tdls-mfp-splitrx-idsup-idauth-sr-ltecx-proptxstatus-ampduhostreorder-sstput-die2/rtecdc.bin \
    -udp 800m  \
    -tcpwindow 2m \
    -wlinitcmds {wl vht_features 3} \
    -yart {-attn5g 30-95 -attn2g 30-95} \
	-pre_perf_hook  {{%S wl ampdu_clear_dump} {%S wl amsdu_clear_counters}} \
    -post_perf_hook {{%S wl rssi} {%S wl rate} {%S wl nrate} {%S wl dump phycal} {%S wl dump rssi} {%S wl counters} {%S wl ampdu_clear_dump}} \
    -yart {-attn5g 30-95 -attn2g 30-95} \
	

#4349a0 configure -ipaddr 192.168.1.91  \


4349a0 clone 4349a0sr \
        -type 4349a0-roml/pcie-ag-p2p-pno-aoe-pktfilter-keepalive-mchan-wl11u-pktctx-okc-tdls-ccx-mfp-splitrx-wnm-ve-idsup-idauth-sr-ltecx-proptxstatus-ampduhostreorder/rtecdc.bin \
        -wlinitcmds {wl vht_features 3;wl rsdb_mode 0} \

4349a0 clone 4349a0.1 -sta {4349a0.1 eth0 _4349a0.2 wl1.2} \
    -type 4349a0-roml/pcie-ag-p2p-pno-aoe-pktfilter-keepalive-mchan-wl11u-pktctx-okc-tdls-ccx-mfp-splitrx-wnm-ve-idsup-idauth-sr-ampduhostreorder/rtecdc.bin \
    -wlinitcmds {wl mpc 0;wl vht_features 3;wl rsdb_mode 1;wl -w 1 bss -C 2 sta } \
    -perfchans {36/80 36l} \

_4349a0.2 clone 4349a0.2 -sta {_4349a0.1 eth0 4349a0.2 wl1.2} \
    -perfonly 1 
4349a0.2 configure -dualband {4360b 4349a0.1 -c1 36/80 -c2 3l -b1 800m -b2 800m}

4349a0 clone 4349a2 \
	-tag DINGO_BRANCH_9_10 \
	-type 4359a2-roml/threadx-pcie-ag-p2p-mchan-wl11u-pktctx-splitrx-idsup-idauth-sr-ltecx-proptxstatus-ampduhostreorder-sstput-die2-rsdbsw/rtecdc.bin \
	-nvram "bcm94359fcpagbss.txt" \
	-wlinitcmds {wl rsdb_mode 0;wl vht_features 3} \

4349a2 clone 4349a2.1 -sta {4349a2.1 eth0 _4349a2.2 wl1.2} \
    -wlinitcmds {wl mpc 0;wl vht_features 3;wl rsdb_mode 1;wl -w 1 bss -C 2 sta;wl -i wl1.2 chanspec 1} \
    -perfchans {36/80 36l} -nocustom 1 \

_4349a2.2 clone 4349a2.2 -sta {_4349a2.1 eth0 4349a2.2 wl1.2} \
    -perfonly 1 -nocustom 1 \

4349a2.2 configure -dualband {4360b 4349a2.1 -c1 36/80 -c2 3l -b1 800m -b2 800m -sec1 aespsk2 -sec2 wep} \


###################################### 4359B0-die3 ##################################################

4349a0 clone 4359d3 \
	-tag DINGO_BRANCH_9_10 \
	-type 4359b0-roml/threadx-pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-sstput-proptxstatus-idsup-assert-err-die3-rsdbsw/rtecdc.bin \
	-nvram "bcm943593fcpagbss_eval.txt" \
	-wlinitcmds {wl rsdb_mode 0;wl vht_features 3} \


##################### 4359B1 ############### 

4359d3 clone 4359 -sta {4359 eth0 _4359.2 wl0.2} \
    -wlinitcmds {wl down; wl vht_features 3; wl bss -C 2 sta} \
    -perfchans {36/80}

4359d3 clone 4359B1 -sta {4359B1 eth0 _4359B1.2 wl0.2} \
   -type 4359b1-roml/pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-sstput-proptxstatus-idsup-assert-err-die3-rsdbsw/rtecdc.bin \
   -nvram "bcm943593fcpagbss.txt" \
   -wlinitcmds { wl down; wl vht_features 3 ; wl bss -C 2 sta} \
   -channelsweep {-usecsa} \

4359B1 clone 4359B1.1 -sta {4359B1.1 eth0 _4359B1.2 wl0.2} \
    -wlinitcmds {wl vht_features 3;wl rsdb_mode 1;wl -w 1 bss -C 2 sta } \
	-nocustom 1 \

_4359B1.2 clone 4359B1.2 -sta {_4359B1.1 eth0 4359B1.2 wl0.2} \
        -perfchans 3l -perfonly 1 -nocustom 1 \

4359B1.2 configure -dualband {4360b 4359B1.1 -c1 36/80 -c2 3l -b1 800m -b2 800m } \

4359B1 clone 4359B1tp1 \
   -type 4359b1-roml/pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-sstput-proptxstatus-idsup-assert-err-die3-rsdbsw/rtecdc.bin \
   -nvram "bcm943593fcpagbss.txt" \
   -wlinitcmds { wl rsdb_mode 0;wl down; wl vht_features 3} \

4359B1tp1 clone 4359B1tp2 \
	-type 4359b1-roml/pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-proptxstatus-sr-idsup-sstput-die3-pktctx-rsdbsw/rtecdc.bin \
	
4359B1tp2 clone 4359B1tp \
		-tag DINGO2_BRANCH_9_15 \
	-brand hndrte-dongle-wl \
	-dhd_brand linux-internal-dongle-pcie \
	-nvram "bcm94355fcpagb.txt" \
	-dhd_tag trunk \
	-app_tag trunk \
	-type 4355b2-roml/threadx-pcie-ag-msgbuf-splitrx-splitbuf-pktctx-proptxstatus-p2p-pno-nocis-keepalive-aoe-idsup-wapi-sr-ve-awdl-ndoe-pf2-cca-pwrstats-wnm-wl11u-anqpo-noclminc-mpf-mfp-ltecx-txbf-logtrace-srscan-txpwrcap-dfsradar-txcal-err-pktctx-die0/rtecdc.bin \
	
4359B1tp clone 4359B1tp.1 -sta {4359B1tp.1 eth0 _4359B1tp.2 wl1.2} \
    -wlinitcmds {wl vht_features 3;wl rsdb_mode 1;wl -w 1 bss -C 2 sta } \
	-nocustom 1 \

_4359B1tp.2 clone 4359B1tp.2 -sta {_4359B1tp.1 eth0 4359B1tp.2 wl1.2} \
        -perfchans 3l -perfonly 1 -nocustom 1 \

4359B1tp.2 configure -dualband {4360b 4359B1tp.1 -c1 36/80 -c2 3l -b1 800m -b2 800m } \

4359B1tp clone 4359B1a \
   -type 4359b1-roml/pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-proptxstatus-sr-idsup-assert-err-die3-pktctx-sstput/rtecdc.bin \
   -nvram "bcm943593fcpagbss.txt" \
   -wlinitcmds {wl down;wl vht_features 3;wl bss -C 2 sta; wl rsdb_mode} \

4359B1a clone 4359B1a -sta {4359B1a.1 eth0 _4359B1a.2 wl0.2} \
    -wlinitcmds {wl vht_features 3;wl rsdb_mode 1;wl bss -C 2 sta} \
	-nocustom 1 \

_4359B1a.2 clone 4359B1a.2 -sta {_4359B1a.1 eth0 4359B1a.2 wl0.2} \
        -perfchans 3l -perfonly 1 -nocustom 1 \

4359B1 clone 4359B1GN5 \
	-type 4359b1-roml/pcie-wl11u-ccx-ve-mfp-okc-idsup-sr-ltecx-die3-tdls-pwrofs-gscan-wepso-sarctrl-linkstat-wnm-pfn-hs20sta-ulb-pwrstats-lpc-olpc-mobfd-txbf-mimopscan-rcc-noccxaka-btcdyn-sstput/rtecdc.bin \
	-tag DINGO_BRANCH_9_10 \
	-wlinitcmds {wl vht_features 3;wl bss -C 2 sta } \
	-nocustom 1 \
		

#####################################DINGO07T_BRANCH_9_35######################################

4359B1 clone 4359B1_35 \
   -tag DINGO07T_BRANCH_9_35 \
   -type 4359b1-roml/pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-proptxstatus-sr-idsup-assert-err-die3-pktctx-slna/rtecdc.bin \
   -nvram "bcm943593fcpagbss_slna.txt" \
   -wlinitcmds {wl rsdb_mode 0; wl down; wl vht_features 3} \
   -channelsweep {-usecsa -band a}  \

4359B1_35 clone 4359B1_35x \
   -dhd_tag DHD_BRANCH_1_363 \
   -type 4359b1-roml/pcie-wl11u-ve-mfp-okc-idsup-sr-die3-tdls-pwrofs-gscan-wepso-sarctrl-linkstat-wnm-pfn-hs20sta-ulb-pwrstats-lpc-olpc-mobfd-txbf-mimopscan-apcs-pspretend-rcc-slna-ccx-ltecx-clm_ss_mimo/rtecdc.bin \
   -wlinitcmds { wl down; wl vht_features 3 ; wl bss -C 2 sta} \
   
   
4359B1_35x clone 4359B1_35X.1 -sta {4359B1_35x.1 eth0 _4359B1_35x.2 wl1.2} \
    -wlinitcmds {wl down; wl vht_features 3;wl rsdb_mode 1;wl -w 1 bss -C 2 sta} \
	-nocustom 1 \

_4359B1_35x.2 clone 4359B1_35x.2 -sta {_4359B1_35x.1 eth0 4359B1_35x.2 wl1.2} \
        -perfchans 3l -perfonly 1 -nocustom 1 \

4359B1_35x.2 configure -dualband {4360b 4359B1_35x.1 -c1 36/80 -c2 3l -b1 800m -b2 800m } \
 
4359B1 clone 4359B1_35a \
   -tag DINGO07T_BRANCH_9_35 \
   -type 4359b1-roml/pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-sstput-proptxstatus-idsup-assert-err-die3-rsdbsw/rtecdc.bin \
   -nvram "bcm943593fcpagbss.txt" \
   -wlinitcmds {wl rsdb_mode 0; wl down; wl vht_features 3} \
   -channelsweep {-usecsa} \

4359B1_35 clone 4359B1_35.1 -sta {4359B1_35.1 eth0 _4359B1_35.2 wl1.2} \
    -wlinitcmds {wl vht_features 3;wl rsdb_mode 1;wl -w 1 bss -C 2 sta } \
	-nocustom 1 \

_4359B1_35.2 clone 4359B1_35.2 -sta {_4359B1_35.1 eth0 4359B1_35.2 wl1.2} \
        -perfchans 3l -perfonly 1 -nocustom 1 \

4359B1_35.2 configure -dualband {4360b 4359B1_35.1 -c1 36/80 -c2 3l -b1 800m -b2 800m } \

4359B1_35 clone 4359B1at \
   -type 4359b1-roml/pcie-ag-msgbuf-p2p-mchan-splitrx-ampduhostreorder-sstput-proptxstatus-idsup-assert-err-die3-rsdbsw/rtecdc.bin \
   -nvram "bcm943593fcpagbss.txt" \
   -wlinitcmds { wl down; wl vht_features 3} \
   
4359B1at clone 4359B1at.1 -sta {4359B1at.1 eth0 _4359B1at.2 wl1.2} \
    -wlinitcmds {wl vht_features 3;wl rsdb_mode 1;wl -w 1 bss -C 2 sta } \
	-nocustom 1 \

_4359B1at.2 clone 4359B1at.2 -sta {_4359B1at.1 eth0 4359B1at.2 wl1.2} \
        -perfchans 3l -perfonly 1 -nocustom 1 \

#######################gold drop################	
		
4359B1_35 clone 4359B1_35GN5 \
	-type 4359b1-roml/pcie-wl11u-ve-mfp-okc-idsup-sr-die3-betdls-pwrofs-gscan-wepso-sarctrl-linkstat-wnm-pfn-hs20sta-pwrstats-lpc-olpc-mobfd-txbf-mimopscan-apcs-pspretend-rcc-slna-ccx-noccxaka-clm_ss_mimo-proxd-txpwr-fmc-shif-wbtext-roamol-rapsta/rtecdc.bin \
	-tag DINGO07T_REL_9_35_46 \
	-wlinitcmds { wl down; wl vht_features 3 ; wl bss -C 2 sta } \
	
4359B1_35GN5 clone 4359B1_35GN5.1 -sta {4359B1_35GN5.1 eth0 _4359B1_35GN5.2 wl0.2} \
    -wlinitcmds {wl vht_features 3;wl rsdb_mode 1;wl bss -C 2 sta} \
	-nocustom 1 \

_4359B1_35GN5.2 clone 4359B1_35GN5.2 -sta {_4359B1_35GN5.1 eth0 4359B1_35GN5.2 wl0.2} \
        -perfchans 3l -perfonly 1 -nocustom 1 \

 

UTF::Linux blr06tst3 \
     -lan_ip 10.131.80.32 \
     -sta {4359Nic eth0} \
	 -power "npc31 2" \
	 -nvram "bcm94359fcpagbss.txt" \
     -udp 800m  \
     -tcpwindow 2m \
     -wlinitcmds {wl vht_features 3} \
     -yart {-attn5g 05-95 -attn2g 30-95}


	 

set UTF::StaNightlyCustom {
    if {$(ap2) ne ""} {
	if {[regexp {(4359..)$} $STA - r]} {
	    UTF::Try "+RSDB Mode Switch" {
		package require UTF::Test::RSDBModeSwitch
		RSDBModeSwitch $Router $(ap2) $STA ${r}.2
	    }
	}
}
	package require UTF::Test::MultiSTANightly
	MultiSTANightly -ap1 $Router -ap2 $(ap2) -sta $STA \
	    -nosetuptestbed -nostaload -nostareload -nosetup \
	    -noapload -norestore -nounload

}	



UTF::Q blr06
