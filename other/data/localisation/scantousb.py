# *******************************************************************************************
# *******************************************************************************************
#
#      Name :      scantousb.py
#      Purpose :   Convert a scan code to a USB code
#      Date :      3rd July 2025
#      Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

# *******************************************************************************************
#   
#                                   Scan to USB convertor class
#
# *******************************************************************************************

class ScanToUSB(object):
    def __init__(self):
        self.setup()

    def get(self,n):
        return self.scanToUSB[n] if n in self.scanToUSB else None 

    def setup(self):
        self.scanToUSB = {
                0x00FF:0x01,   # Overrun Error
                0x00FC:0x02,   # POST Fail
                0x001E:0x04,   # a A
                0x0030:0x05,   # b B
                0x002E:0x06,   # c C
                0x0020:0x07,   # d D
                0x0012:0x08,   # e E
                0x0021:0x09,   # f F
                0x0022:0x0A,   # g G
                0x0023:0x0B,   # h H
                0x0017:0x0C,   # i I
                0x0024:0x0D,   # j J
                0x0025:0x0E,   # k K
                0x0026:0x0F,   # l L
                0x0032:0x10,   # m M
                0x0031:0x11,   # n N
                0x0018:0x12,   # o O
                0x0019:0x13,   # p P
                0x0010:0x14,   # q Q
                0x0013:0x15,   # r R
                0x001F:0x16,   # s S
                0x0014:0x17,   # t T
                0x0016:0x18,   # u U
                0x002F:0x19,   # v V
                0x0011:0x1A,   # w W
                0x002D:0x1B,   # x X
                0x0015:0x1C,   # y Y
                0x002C:0x1D,   # z Z
                0x0002:0x1E,   # 1 !
                0x0003:0x1F,   # 2 @
                0x0004:0x20,   # 3 #
                0x0005:0x21,   # 4 $
                0x0006:0x22,   # 5 %
                0x0007:0x23,   # 6 ^
                0x0008:0x24,   # 7 &
                0x0009:0x25,   # 8 *
                0x000A:0x26,   # 9 (
                0x000B:0x27,   # 0 )
                0x001C:0x28,   # Return
                0x0001:0x29,   # Escape
                0x000E:0x2A,   # Backspace
                0x000F:0x2B,   # Tab
                0x0039:0x2C,   # Space
                0x000C:0x2D,   # - _
                0x000D:0x2E,   # = +
                0x001A:0x2F,   # [ 
                0x001B:0x30,   # ] 
                0x002B:0x31,   # \ |
                0x002B:0x32,   # Europe 1 (Note 2)
                0x0027:0x33,   # ; :
                0x0028:0x34,   # ' "
                0x0029:0x35,   # ` ~
                0x0033:0x36,   # :<
                0x0034:0x37,   # . >
                0x0035:0x38,   # / ?
                0x003A:0x39,   # Caps Lock
                0x003B:0x3A,   # F1
                0x003C:0x3B,   # F2
                0x003D:0x3C,   # F3
                0x003E:0x3D,   # F4
                0x003F:0x3E,   # F5
                0x0040:0x3F,   # F6
                0x0041:0x40,   # F7
                0x0042:0x41,   # F8
                0x0043:0x42,   # F9
                0x0044:0x43,   # F10
                0x0057:0x44,   # F11
                0x0058:0x45,   # F12
                0xE037:0x46,   # Print Screen (Note 1)
                0x0046:0x47,   # Scroll Lock
                0xE052:0x49,   # Insert (Note 1)
                0xE047:0x4A,   # Home (Note 1)
                0xE049:0x4B,   # Page Up (Note 1)
                0xE053:0x4C,   # Delete (Note 1)
                0xE04F:0x4D,   # End (Note 1)
                0xE051:0x4E,   # Page Down (Note 1)
                0xE04D:0x4F,   # Right Arrow (Note 1)
                0xE04B:0x50,   # Left Arrow (Note 1)
                0xE050:0x51,   # Down Arrow (Note 1)
                0xE048:0x52,   # Up Arrow (Note 1)
                0x0045:0x53,   # Num Lock
                0xE035:0x54,   # Keypad / (Note 1)
                0x0037:0x55,   # Keypad *
                0x004A:0x56,   # Keypad -
                0x004E:0x57,   # Keypad +
                0xE01C:0x58,   # Keypad Enter
                0x004F:0x59,   # Keypad 1 End
                0x0050:0x5A,   # Keypad 2 Down
                0x0051:0x5B,   # Keypad 3 PageDn
                0x004B:0x5C,   # Keypad 4 Left
                0x004C:0x5D,   # Keypad 5
                0x004D:0x5E,   # Keypad 6 Right
                0x0047:0x5F,   # Keypad 7 Home
                0x0048:0x60,   # Keypad 8 Up
                0x0049:0x61,   # Keypad 9 PageUp
                0x0052:0x62,   # Keypad 0 Insert
                0x0053:0x63,   # Keypad . Delete
                0x0056:0x64,   # Europe 2 (Note 2)
                0xE05D:0x65,   # App
                0x0059:0x67,   # Keypad =
                0x005D:0x68,   # F13
                0x005E:0x69,   # F14
                0x005F:0x6A,   # F15
                0x007E:0x85,   # Keypad :(Brazilian Keypad .)
                0x0073:0x87,   # Keyboard Int'l 1 ろ (Ro)
                0x0070:0x88,   # Keyboard Int'l 2 かたかな ひらがな ローマ字 (Katakana/Hiragana)
                0x007D:0x89,   # Keyboard Int'l 3 ￥ (Yen)
                0x0079:0x8A,   # Keyboard Int'l 4 前候補 変換 (次候補) 全候補 (Henkan)
                0x007B:0x8B,   # Keyboard Int'l 5 無変換 (Muhenkan)
                0x005C:0x8C,   # Keyboard Int'l 6 (PC9800 Keypad :)
                0x00F2:0x90,   # Keyboard Lang 1 한/영 (Hanguel/English)
                0x00F1:0x91,   # Keyboard Lang 2 한자 (Hanja)
                0x0078:0x92,   # Keyboard Lang 3 かたかな (Katakana)
                0x0077:0x93,   # Keyboard Lang 4 ひらがな (Hiragana)
                0x0076:0x94,   # Keyboard Lang 5 半角/全角 (Zenkaku/Hankaku)
                0x001D:0xE0,   # Left Control
                0x002A:0xE1,   # Left Shift
                0x0038:0xE2,   # Left Alt
                0xE05B:0xE3,   # Left GUI
                0xE01D:0xE4,   # Right Control
                0x0036:0xE5,   # Right Shift
                0xE038:0xE6,   # Right Alt
                0xE05C:0xE7    # Right GUI
}

if __name__ == "__main__":
    print(ScanToUSB().get(0x1E))