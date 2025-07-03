# *******************************************************************************************
# *******************************************************************************************
#
#      Name :      extract.py
#      Purpose :   Extract a scan code set from an XML file
#      Date :      3rd July 2025
#      Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,re
from scantousb import *

# *******************************************************************************************
#   
#                                       Extractor class
#
# *******************************************************************************************

class Extractor(object):
    def __init__(self,fileName):
        self.scanToUSB = ScanToUSB()
        self.usbToChars = {}
        self.inPK = False
        for l in [x.strip() for x in open(fileName).readlines()]:                   # Scan through file
            if l[:3].upper() == "<PK":                                              # Look for Physical key records
                self.processOpen(l)
                self.inPK = True
            if l[:5].upper() == "</PK>":
                self.inPK = False
            if self.inPK and l[:7].upper() == "<RESULT":                            # <RESULT entry
                if self.currentUSBCode is not None:                                 # process it if a key current
                    self.processResult(l)

    def processOpen(self,l):
        m = re.search("SC=\"([0-9A-Fa-f]+)\"",l.upper())                            # Find scan code
        assert m is not None,"Can't find scancode "+l
        self.currentUSBCode = self.scanToUSB.get(int(m.group(1),16))                # Convert to USB code
        if self.currentUSBCode is not None:                                         # Create if it exists
            if self.accept(self.currentUSBCode):                                    # If we want it
                assert self.currentUSBCode not in self.usbToChars
                self.usbToChars[self.currentUSBCode] = [ None, None ]
            else:                                                                   # We don't, some we do manually. This is ASCII standard
                self.currentUSBCode = None

    def accept(self,n):
        if n >= 0x28 and n <= 0x2B:                                                 # ENTER, ESC, BACKSPACE, TAB
            return False
        if n >= 0x39:                                                               # All the control characters, number keypad and odd things.
            return False
        return True

    def processResult(self,l):
        m = re.search('Text=\"(.*?)\"',l)                                           # Character to use
        if m is not None:
            keyChar = m.group(1)                                                    # Get character and remove XML detritus
            keyChar = keyChar.replace("&quot;",'"').replace("&amp;","&").replace("&lt;","<").replace("&gt;",">")
            m = re.search('With=\"(.*?)\"',l)                                       # Find modifier
            mode = "" if m is None else m.group(1).upper()
            if mode == "":                                                          # Put in shifted or unshifted slot.
                self.usbToChars[self.currentUSBCode][0] = keyChar
            if mode == "VK_SHIFT":
                self.usbToChars[self.currentUSBCode][1] = keyChar

    def dump(self):
        keys = [x for x in self.usbToChars.keys()]
        keys.sort()
        for k in keys:
            print("{0:2} : {1}".format(k,self.usbToChars[k]))

if __name__ == "__main__":
    for root,dirs,files in os.walk("layouts"):
        for f in files:
            e = Extractor(root+os.sep+f)
            print("\n"+f)
            e.dump()