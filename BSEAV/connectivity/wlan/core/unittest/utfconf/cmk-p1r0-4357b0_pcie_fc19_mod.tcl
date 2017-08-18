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
UTF::Power::WebRelay Power -lan_ip 10.19.90.131 -relay Controller

# Controller
UTF::Linux Controller\
	-lan_ip 10.19.90.24\
    -sta {lan eth1}
lan configure -ipaddr 10.19.90.24


# AP Section
# BCM Reference Router to compare Linksys E4200 Behavior
UTF::Router Ap_2_5\
    -lan_ip 10.19.90.130\
    -sta {Ap_2 eth1 Ap_5 eth2}\
    -relay lan\
    -lanpeer lan\
    -model bcm94718nrl_p150\
    -power {Power 8}\
    -console "10.19.90.24:40007"\
    -brand linux-2.6.36-arm-internal-router\
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
		wl1_channel=11
		wl1_obss_coex=0
		wl_msglevel2=0x8
	}

### STA Sections ###
UTF::DHD 4357b0_p1r0_st1 \
    -lan_ip 10.19.90.98 \
    -sta {p1r0s1_4357MAIN eth0 p1r0s1_4357AUX eth0} \
	-power {Power 1} \
    -power_sta {Power 1} \
    -hostconsole "10.19.90.24:40000"\
	-tag IGUANA_BRANCH_13_10 \
	-dhd_tag DHD_BRANCH_1_579  \
	-dhd_brand linux-internal-dongle-pcie \
	-driver dhd-msgbuf-pciefd-debug \
	-brand linux-internal-dongle-pcie \
	-type 4357b0-roml/config_pcie_debug/rtecdc.bin \
    -nvram "src/shared/nvram/bcm94357GuinnessUsiKK.txt" \
    -clm_blob 4347b0.clm_blob \
  -wlinitcmds {
     wl down;
   }

UTF::DHD 4357b0_p1r0_st5 \
    -lan_ip 10.19.90.102 \
    -sta {p1r0s5_4357MAIN eth0 p1r0s5_4357AUX eth0} \
	-power {Power 5} \
    -power_sta {Power 5} \
    -hostconsole "10.19.90.24:40004"\
	-tag IGUANA_BRANCH_13_10 \
	-dhd_tag DHD_BRANCH_1_579  \
	-dhd_brand linux-internal-dongle-pcie \
	-driver dhd-msgbuf-pciefd-debug \
	-brand linux-internal-dongle-pcie \
	-type 4357b0-roml/config_pcie_debug/rtecdc.bin \
    -nvram "src/shared/nvram/bcm94357TuborgMurataMM.txt" \
    -clm_blob 4347b0.clm_blob \
  -wlinitcmds {
     wl down;
   }
