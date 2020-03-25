#!/usr/bin/env python

import argparse
import sys
from junit_xml import TestSuite, TestCase
import subprocess

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("test_abspath")
    parser.add_argument("xmlfile_abspath")
    args = parser.parse_args()


    def grep_for(symbol, location):
        out = ""
        try:
            cmd = 'objdump -t {} | grep "{}" | grep "{}"'.format(args.test_abspath, symbol, location)
            out = subprocess.check_output(cmd, shell=True).split()
        except subprocess.CalledProcessError:
            # non-zero exit code means nothing found => good
            pass
        return out

    test_cases = []
    fail = False

    test_case = TestCase('test_linkage_int', args.test_abspath, 1)
    out = grep_for("_int", "^8")
    if out:
        fail = True
        test_case.add_failure_info(message='symbols:\n{}'.format(out))
    test_cases.append(test_case)

    test_case = TestCase('test_linkage_ext', args.test_abspath, 1)
    out = grep_for("_ext", "^0")
    if out:
        fail = True
        test_case.add_failure_info(message='symbols:\n{}'.format(out))
    test_cases.append(test_case)

    ts = [TestSuite("libnux", test_cases)]

    with open(args.xmlfile_abspath, 'w') as fd:
        TestSuite.to_file(fd, ts, prettyprint=True)

    sys.exit(fail)
