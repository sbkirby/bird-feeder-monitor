# coding=utf-8

# Copyright (C) 2014  Stephen Kirby

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# This script sends data via PushingBox (http://www.pushingbox.com) to a 
# specific Google Docs spreadsheet

import sys, urllib, urllib2

key = "YOUR_PUSHINGBOX_DEVICE_ID"  # Replace with your PushingBox Device ID

# The number of variables sent to PushingBox may vary with your configuration  
def pushingbox(key, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12):
    url = 'http://api.pushingbox.com/pushingbox'
    values = {'devid':key, 'cnt1':val1[0], 'time1':val2[0], 'cnt2':val3[0], 'time2':val4[0], 'cnt3':val5[0], 'time3':val6[0], 'cnt4':val7[0], 'time4':val8[0], 'cnt5':val9[0], 'time5':val10[0], 'cnt6':val11[0], 'time6':val12[0]}
    #print "values=", values
    try:
      data = urllib.urlencode(values)  
      #print "data=", data
      req = urllib2.Request(url, data)
      sendrequest = urllib2.urlopen(req)
    except Exception, detail:
      print "Error ", detail

# The number of variables sent to PushingBox may vary with your configuration
pushingbox(key, [sys.argv[1]], [sys.argv[2]], [sys.argv[3]], [sys.argv[4]], [sys.argv[5]], [sys.argv[6]], [sys.argv[7]], [sys.argv[8]], [sys.argv[9]], [sys.argv[10]], [sys.argv[11]], [sys.argv[12]])

