r
h

exec device = R7S721001
// Ensure WP# (P9.6) to high
w2 0xfcfe3024,0x0040
w2 0xfcfe3424,0x0000
w2 0xfcfe3324,0xffbf
w2 0xfcfe7224,0x0000

// Ensure WP# (P2.14) to high
w2 0xfcfe3008,0x4000
w2 0xfcfe3408,0x0000
w2 0xfcfe3308,0xbfff
w2 0xfcfe7208,0x0000
loadbin ..\..\..\..\util\dos_scripts\tools\DisplayIt_QSPI_Loader_SDR_DUAL_4B.bin,0x18000000
verifybin ..\..\..\..\util\dos_scripts\tools\DisplayIt_QSPI_Loader_SDR_DUAL_4B.bin,0x18000000

r
g
q
