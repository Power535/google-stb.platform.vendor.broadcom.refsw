# -*-tcl-*-
# Load Packages
#package require UTF::Aeroflex
package require UTF::Linux

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
UTF::Power::WebRelay Power -lan_ip 10.19.90.67 -relay Controller

# Controller
UTF::Linux Controller\
    -lan_ip 10.19.90.23\
    -sta {lan eth1}
lan configure -ipaddr 10.19.90.23


# AP Section
# BCM Reference Router to compare Linksys E4200 Behavior
UTF::Router Ap_2_5\
    -lan_ip 10.19.90.66\
    -sta {Ap_2 eth1 Ap_5 eth2}\
    -relay lan\
    -lanpeer lan\
    -model bcm94718nrl_p150\
    -power {Power 8}\
    -console "10.19.90.23:40007"\
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
		wl1_ssid=4331
		wl1_radio=1
		wl1_channel=36
		wl1_obss_coex=0
		wl_msglevel2=0x8
	}

### STA Sections ###
### BCM7445-43602a1 PCIe-Dongle STA ###
package require UTF::STB

UTF::STB BCM7445_43602a1_p0r0s6_sta \
-lan_ip 10.19.90.39 \
-console "10.19.90.23:40005" \
-sta {7445_43602 eth1} \
-power {Power 6}\
-power_sta {Power 6} \
-dhd_brand linux-internal-media \
-brand linux-external-media \
-tag  BISON05T_BRANCH_7_35\
-app_tag trunk \
-type dhd-msgbuf-pciefd-mfp-armv7l-debug \
-dongleimage "43602a1-roml/pcie-ag-pktctx-splitrx-amsdutx-txbf-p2p-mchan-idauth-idsup-tdls-mfp-proptxstatus-pktfilter-wowlpf-ampduhostreorder-keepalive.bin"

