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

# python-weather-api
# pywapi - https://code.google.com/p/python-weather-api/

# replace 'USTX0905' with your own location_id unless your interested in
# the sunrise and sunset in Missouri City, TX.
# One spot to find your location_id is to find your location at Intellicast
# http://www.intellicast.com/.  The location_id will be part of the url.
# This doesn't seem to work at Yahoo! weather for reasons unknown.

import pywapi

def piece(s, sep):
    stack = [s]
    for char in sep:
        pieces = []
        for substr in stack:
            pieces.extend(substr.split(char))
        stack = pieces
    return stack   
    
def minutes(h, m, ampm):
    t = int(h) * 60
    t = t + int(m)
    if ampm > 0:
       t = t + 720
    return str(t)

result = pywapi.get_weather_from_yahoo('USTX0905','imperial')
AM =  piece(result['astronomy']['sunrise'],(':',' am'))
sunrise = minutes(AM[0],AM[1],0)
PM = piece(result['astronomy']['sunset'],(':',' pm'))
sunset = minutes(PM[0],PM[1],1)
print(sunrise + '^' + sunset)
