# -*-tcl-*-
# Load Packages
#package require UTF::Aeroflex
package require UTF::Linux
#
#set ::UTF::SummaryDir "/projects/hnd_software/work/mbeyer/logs/"

# Default TestBed Configuration Options
set ::UTF::SetupTestBed {
    #
    # Make Sure Attenuators are set to 0 value
    #
    #ALL attn 0;

    # Make sure radios are off on APs before testing
    # to ensure that Embedded Nightly only sees one AP
    #
    #AP1 restart wl0_radio=0
    #AP1 restart wl1_radio=0

    # delete myself to make sure it doesn't rerun by some script issue
    unset ::UTF::SetupTestBed

    return
}

# Define power controllers on cart
UTF::Power::WebRelay Power -lan_ip 10.19.90.195 -relay Controller

# Controller
UTF::Linux Controller\
    -lan_ip 10.19.90.25\
    -sta {lan eth1}
lan configure -ipaddr 10.19.90.25


# AP Section
# BCM Reference Router to compare Linksys E4200 Behavior
UTF::Router Ap_2_5\
    -lan_ip 10.19.90.201\
    -sta {Ap_2 eth1 Ap_5 eth2}\
    -relay lan\
    -lanpeer lan\
    -model bcm94718nrl_p150\
    -power {Power 8}\
    -console "10.19.90.25:41007"\
    -brand linux26-internal-router\
    -tag "AKASHI_REL_5_110_35"\
    -txt_override {wandevs=et0}\
    -nvram {
		lan_ipaddr=192.168.1.1
		lan_gateway=192.168.1.1
		dhcp_start=192.168.1.100
		dhcp_end=192.168.1.150
		lan1_ipaddr=192.168.2.1
		lan1_gateway=192.169.2.1
		dhcp1_start=192.168.2.100
   		dhcp1_end=192.168.2.150
		watchdog=3000
		console_loglevel=7
		wl0_ssid=smoke
		wl0_radio=1
		wl0_channel=11
		wl0_obss_coex=0
		fw_disable=1
		# No DHCP server when router disable 1 is set
		router_disable=0
		wl1_ssid=smoke5
		wl1_radio=1
		wl1_channel=11
		wl1_obss_coex=0
		wl_msglevel2=0x8
	}

### STA Sections ###

UTF::DHD 10.19.90.171 \
-sta {fc19-4350c5-pcie-sta1 eth0} \
-hostconsole "10.19.90.25:40100"\
-power {Power 1}\
    -dhd_brand linux-external-dongle-pcie \
    -driver dhd-msgbuf-pciefd-debug \
    -tag BIS120RC4_BRANCH_7_15\
	-brand linux-external-dongle-pcie\
    -type 4350c5-roml/pcie-ag-msgbuf-splitrx-splitbuf-p2p-pno-nocis-keepalive-aoe-idsup-wapi-sr-cca-pwrstats-noclminc-logtrace-err-assert-txpwrcap-swdiv-ecounters-bssinfo/rtecdc.bin \
    -nvram "bcm94350RieslingBMurataMT.txt" \
    -nvram_add {macaddr=00:90:4c:12:c0:01} \
    -clm_blob 4350_riesling_b.clm_blob \
    -postinstall {dhd -i eth0 msglevel -event}

UTF::DHD 10.19.90.172 \
-sta {fc19-4350c5-pcie-sta2 eth0} \
-hostconsole "10.19.90.25:40101"\
-power {Power 2}\
    -dhd_brand linux-external-dongle-pcie \
    -driver dhd-msgbuf-pciefd-debug \
    -tag BIS120RC4_BRANCH_7_15\
	-brand linux-external-dongle-pcie\
    -type 4350c5-roml/pcie-ag-msgbuf-splitrx-splitbuf-p2p-pno-nocis-keepalive-aoe-idsup-wapi-sr-cca-pwrstats-noclminc-logtrace-err-assert-txpwrcap-swdiv-ecounters-bssinfo/rtecdc.bin \
    -nvram "bcm94350RieslingBMurataMT.txt" \
    -nvram_add {macaddr=00:90:4c:12:c0:02} \
    -clm_blob 4350_riesling_b.clm_blob \
    -postinstall {dhd -i eth0 msglevel -event}

UTF::DHD 10.19.90.173 \
-sta {fc19-4350c5-pcie-sta3 eth0} \
-hostconsole "10.19.90.25:40102"\
-power {Power 3}\
    -dhd_brand linux-external-dongle-pcie \
    -driver dhd-msgbuf-pciefd-debug \
    -tag BIS120RC4_BRANCH_7_15\
	-brand linux-external-dongle-pcie\
    -type 4350c5-roml/pcie-ag-msgbuf-splitrx-splitbuf-p2p-pno-nocis-keepalive-aoe-idsup-wapi-sr-cca-pwrstats-noclminc-logtrace-err-assert-txpwrcap-swdiv-ecounters-bssinfo/rtecdc.bin \
    -nvram "bcm94350RieslingBMurataMT.txt" \
    -nvram_add {macaddr=00:90:4c:12:c0:03} \
    -clm_blob 4350_riesling_b.clm_blob \
    -postinstall {dhd -i eth0 msglevel -event}

UTF::DHD 10.19.90.174 \
-sta {fc19-4350c5-pcie-sta4 eth0} \
-hostconsole "10.19.90.25:40103"\
-power {Power 4}\
    -dhd_brand linux-external-dongle-pcie \
    -driver dhd-msgbuf-pciefd-debug \
    -tag BIS120RC4_BRANCH_7_15\
	-brand linux-external-dongle-pcie\
    -type 4350c5-roml/pcie-ag-msgbuf-splitrx-splitbuf-p2p-pno-nocis-keepalive-aoe-idsup-wapi-sr-cca-pwrstats-noclminc-logtrace-err-assert-txpwrcap-swdiv-ecounters-bssinfo/rtecdc.bin \
    -nvram "bcm94350RieslingBMurataMT.txt" \
    -nvram_add {macaddr=00:90:4c:12:c0:04} \
    -clm_blob 4350_riesling_b.clm_blob \
    -postinstall {dhd -i eth0 msglevel -event}

UTF::DHD 10.19.90.175 \
-sta {fc19-4350c5-pcie-sta5 eth0} \
-hostconsole "10.19.90.25:40104"\
-power {Power 5}\
    -dhd_brand linux-external-dongle-pcie \
    -driver dhd-msgbuf-pciefd-debug \
    -tag BIS120RC4_BRANCH_7_15\
	-brand linux-external-dongle-pcie\
    -type 4350c5-roml/pcie-ag-msgbuf-splitrx-splitbuf-p2p-pno-nocis-keepalive-aoe-idsup-wapi-sr-cca-pwrstats-noclminc-logtrace-err-assert-txpwrcap-swdiv-ecounters-bssinfo/rtecdc.bin \
    -nvram "bcm94350RieslingBMurataMT.txt" \
    -nvram_add {macaddr=00:90:4c:12:c0:05} \
    -clm_blob 4350_riesling_b.clm_blob \
    -postinstall {dhd -i eth0 msglevel -event}

UTF::DHD 10.19.90.176 \
-sta {fc19-4350c5-pcie-sta6 eth0} \
-hostconsole "10.19.90.25:40105"\
-power {Power 6}\
    -dhd_brand linux-external-dongle-pcie \
    -driver dhd-msgbuf-pciefd-debug \
    -tag BIS120RC4_BRANCH_7_15\
	-brand linux-external-dongle-pcie\
    -type 4350c5-roml/pcie-ag-msgbuf-splitrx-splitbuf-p2p-pno-nocis-keepalive-aoe-idsup-wapi-sr-cca-pwrstats-noclminc-logtrace-err-assert-txpwrcap-swdiv-ecounters-bssinfo/rtecdc.bin \
    -nvram "bcm94350RieslingBMurataMT.txt" \
    -nvram_add {macaddr=00:90:4c:12:c0:06} \
    -clm_blob 4350_riesling_b.clm_blob \
    -postinstall {dhd -i eth0 msglevel -event}
