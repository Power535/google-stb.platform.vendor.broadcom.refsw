#!/usr/bin/env python2.7

# Copyright (c) 2014 by Broadcom Corporation

"""
Pack generic data into an MSF-formatted file.

MSF is a generic data file format which is used to wrap multiple segments
of data into a single binary data file.

EXAMPLE:

    msf.py -f data.bin [type] -i 0x4350 [type] -s "any text" [type] -o msf.out
"""

from __future__ import print_function

import argparse
import binascii
import logging
import struct
import sys

logging.basicConfig(
    format='[%(module)s:%(levelname)s] %(message)s',
    level=logging.INFO)
if sys.version_info[0] != 2 or sys.version_info[1] != 7:
    sys.exit('Currently supports only Python 2.7')


def main():
    """Entry point for standalone use."""

    # Command line argument processing
    parser = argparse.ArgumentParser(
        epilog=__doc__.strip(),
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument(
        '-f', '--file',
        action='append',
        metavar=('IFILE_DAT', 'segment_type'),
        nargs='+',
        help="Input data file: IFILE_DAT with optional segment type")
    parser.add_argument(
        '-i', '--integer',
        action='append',
        metavar=('IDATA_INT', 'segment_type'),
        nargs='+',
        help="Input integer value: IDATA_INT with optional segment type")
    parser.add_argument(
        '-s', '--string',
        action='append',
        metavar=('IDATA_STR', 'segment_type'),
        nargs='+',
        help="Input string value: IDATA_STR with optional segment type")
    parser.add_argument(
        '-o', '--output',
        metavar='OFILE_MSF',
        help="Output MSF file: OFILE_MSF")
    opts = parser.parse_args()

    if not opts.output:
        return 'Missing output MSF file name'
    fout = opts.output

    # MSF segments identification
    seg_header_list = []
    if opts.file:
        for arg in opts.file:
            if not arg:
                return 'Missing data file name'
            fin = arg[0]
            try:
                with open(fin, 'rb') as fin_hdl:
                    seg_data = bytearray(fin_hdl.read())
            except IOError as err:
                return str(err)
            seg_data_type = 0
            if len(arg) > 1:
                seg_data_type = int(arg[1])
            seg_header_list.append((seg_data_type, seg_data))
            logging.info('segment: - src %s type %d len %d',
                         arg, seg_data_type, len(seg_data))
    if opts.integer:
        for arg in opts.integer:
            if not arg:
                return 'Missing integer value'
            seg_data = struct.pack('<I', int(arg[0], 0))
            seg_data_type = 0
            if len(arg) > 1:
                seg_data_type = int(arg[1])
            seg_header_list.append((seg_data_type, seg_data))
            logging.info('segment: - src %s type %d len %d',
                         arg, seg_data_type, len(seg_data))
    if opts.string:
        for arg in opts.string:
            if not arg:
                return 'Missing string value'
            seg_data = bytearray(arg[0])
            seg_data.append(0)
            seg_data_type = 0
            if len(arg) > 1:
                seg_data_type = int(arg[1])
            seg_header_list.append((seg_data_type, seg_data))
            logging.info('segment: - src %s type %d len %d',
                         arg, seg_data_type, len(seg_data))

    if not seg_header_list:
        return 'No MSF segment'
    num_segments = len(seg_header_list)
    logging.info('Total segments: %d\n', len(seg_header_list))

    # MSF construction
    seg_struct = '<I I I I I'
    header_prefix_struct = '<4s I I I I'
    header_prefix_magic = 'BLOB'

    seg = struct.pack(seg_struct, 0, 0, 0, 0, 0)
    header_prefix = struct.pack(header_prefix_struct, header_prefix_magic,
                                0, 0, 0, 0)
    header_prefix_len = len(header_prefix)
    header_len = header_prefix_len + len(seg) * num_segments
    header_prefix = struct.pack(header_prefix_struct, header_prefix_magic,
                                header_len, 0, 1, num_segments)

    seg_offset = header_len
    header_crc32 = binascii.crc32(header_prefix[12:])
    seg_list = []
    for seg_hdr in seg_header_list:
        seg_data_type = seg_hdr[0]
        seg_data = seg_hdr[1]
        seg_data_len = len(seg_data)
        seg_data_crc32 = binascii.crc32(seg_data) & 0xffffffff
        seg = struct.pack(seg_struct, seg_data_type, seg_offset,
                          seg_data_len, seg_data_crc32, 0)
        header_crc32 = binascii.crc32(seg, header_crc32)
        seg_offset += seg_data_len
        seg_list.append((seg, seg_data))
        logging.info('segment: - type %d len %d crc32 0x%x',
                     seg_data_type, seg_data_len, seg_data_crc32)

    header_crc32 = header_crc32 & 0xffffffff
    header_prefix = struct.pack(header_prefix_struct, header_prefix_magic,
                                header_len, header_crc32, 1, num_segments)
    logging.info('header: - num_segments %d len %d crc32 0x%x\n',
                 num_segments, header_len, header_crc32)

    # Write output
    try:
        with open(fout, 'wb') as fout_hdl:
            fout_hdl.write(header_prefix)
            for seg in seg_list:
                fout_hdl.write(seg[0])
            for seg in seg_list:
                fout_hdl.write(seg[1])
    except IOError as err:
        return str(err)

# Standard boilerplate to call the main() function
if __name__ == '__main__':
    err_msg = main()
    if err_msg:
        logging.error(err_msg)
        sys.exit(1)

# vim: ts=8:sw=4:tw=80:et:
