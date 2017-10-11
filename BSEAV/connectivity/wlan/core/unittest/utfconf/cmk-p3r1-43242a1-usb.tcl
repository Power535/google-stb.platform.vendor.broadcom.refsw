# -*-tcl-*-
# Load Packages
#package require UTF::Aeroflex
package require UTF::Linux

#set ::UTF::SummaryDir "/projects/hnd_software/work/chongmok/logs/"

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
UTF::Power::WebRelay Power -lan_ip 10.19.80.74 -relay Controller
UTF::Power::WebRelay PowerUSB1 -lan_ip 10.19.80.74 -relay Controller
UTF::Power::WebRelay PowerUSB2 -lan_ip 10.19.80.74 -relay Controller

# Controller
UTF::Linux Controller\
    -lan_ip 10.19.80.23\
    -sta {lan eth1}
lan configure -ipaddr 10.19.80.23

# AP Section
# BCM Reference Router to compare Linksys E4200 Behavior
UTF::Router Ap_2_5\
    -lan_ip 10.19.80.73\
    -sta {Ap_2 eth1 Ap_5 eth2}\
    -relay Controller\
    -lanpeer Controller\
    -model bcm94718nrl_p150\
    -power {Power 8}\
    -console "10.19.80.23:41007"\
    -brand linux26-internal-router\
    -txt_override {wandevs=et0}\
    -nvram {
		lan_ipaddr=192.168.1.1
        watchdog=3000
        console_loglevel=7
        fw_disable=1
        router_disable=0
        wl0_ssid=smoke
        wl0_radio=1
        wl0_channel=1
        wl0_obss_coex=0
        wl1_ssid=smoke5
        wl1_radio=1
        wl1_channel=36
        wl1_obss_coex=0
    }

### STA Sections ###

# 43242a1 USB BMac FC19x64
UTF::DHD 10.19.80.43 -sta {43242_p3r1_1 eth0}\
    -tag TRUNK \
	-user root \
	-power {Power 1} \
    -power_sta {PowerUSB1 1} \
    -dhd_brand {} \
    -console "10.19.80.23:44000"\
    -hostconsole "10.19.80.23:41000"\
    -wlinitcmds {wl msglevel +assoc}\
    -nvram bcm943242usbref_p665.txt \
	-brand linux-internal-media \
    -driver "debug-apdef-stadef-high-p2p-mchan-tdls-wowl-media" \
    -type "43242a1-bmac/ag-assert-p2p-mchan-media-srvsdb-vusb.bin.trx"

UTF::DHD 10.19.80.44 -sta {43242_p3r1_2 eth0}\
    -tag TRUNK \
	-user root \
	-power {Power 2} \
    -power_sta {PowerUSB1 2} \
    -dhd_brand {} \
    -console "10.19.80.23:44001"\
    -hostconsole "10.19.80.23:41001"\
    -wlinitcmds {wl msglevel +assoc}\
    -nvram bcm943242usbref_p665.txt \
	-brand linux-internal-media \
    -driver "debug-apdef-stadef-high-p2p-mchan-tdls-wowl-media" \
    -type "43242a1-bmac/ag-assert-p2p-mchan-media-srvsdb-vusb.bin.trx"

UTF::DHD 10.19.80.45 -sta {43242_p3r1_3 eth0}\
    -tag TRUNK \
	-user root \
	-power {Power 3} \
    -power_sta {PowerUSB1 3} \
    -dhd_brand {} \
    -console "10.19.80.23:44002"\
    -hostconsole "10.19.80.23:41002"\
    -wlinitcmds {wl msglevel +assoc}\
    -nvram bcm943242usbref_p665.txt \
	-brand linux-internal-media \
    -driver "debug-apdef-stadef-high-p2p-mchan-tdls-wowl-media" \
    -type "43242a1-bmac/ag-assert-p2p-mchan-media-srvsdb-vusb.bin.trx"

UTF::DHD 10.19.80.46 -sta {43242_p3r1_4 eth0}\
    -tag TRUNK \
	-user root \
	-power {Power 4} \
    -power_sta {PowerUSB1 4} \
    -dhd_brand {} \
    -console "10.19.80.23:44003"\
    -hostconsole "10.19.80.23:41003"\
    -wlinitcmds {wl msglevel +assoc}\
    -nvram bcm943242usbref_p665.txt \
	-brand linux-internal-media \
    -driver "debug-apdef-stadef-high-p2p-mchan-tdls-wowl-media" \
    -type "43242a1-bmac/ag-assert-p2p-mchan-media-srvsdb-vusb.bin.trx"

UTF::DHD 10.19.80.47 -sta {43242_p3r1_5 eth0}\
    -tag TRUNK \
	-user root \
	-power {Power 5} \
    -power_sta {PowerUSB1 5} \
    -dhd_brand {} \
    -console "10.19.80.23:44004"\
    -hostconsole "10.19.80.23:41004"\
    -wlinitcmds {wl msglevel +assoc}\
    -nvram bcm943242usbref_p665.txt \
	-brand linux-internal-media \
    -driver "debug-apdef-stadef-high-p2p-mchan-tdls-wowl-media" \
    -type "43242a1-bmac/ag-assert-p2p-mchan-media-srvsdb-vusb.bin.trx"

UTF::DHD 10.19.80.48 -sta {43242_p3r1_6 eth0}\
    -tag TRUNK \
	-user root \
	-power {Power 6} \
    -power_sta {PowerUSB1 6} \
    -dhd_brand {} \
    -console "10.19.80.23:44005"\
    -hostconsole "10.19.80.23:41005"\
    -wlinitcmds {wl msglevel +assoc}\
    -nvram bcm943242usbref_p665.txt \
	-brand linux-internal-media \
    -driver "debug-apdef-stadef-high-p2p-mchan-tdls-wowl-media" \
    -type "43242a1-bmac/ag-assert-p2p-mchan-media-srvsdb-vusb.bin.trx"
