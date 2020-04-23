r
h

// Load the application 
exec device R7S721031_DUALSPI
exec SetCompareMode = 0
exec SetVerifyDownload = 0

// Ensure WP# (P3.12 SPBIO21_0) to high
w2 0xfcfe300c,0x1c00
w2 0xfcfe340c,0x0000
w2 0xfcfe330c,0xc3ff
w2 0xfcfe720c,0x0000


loadbin ..\..\..\..\Debug\Exe\RZA1LU_Sample.bin,0x18080000
verifybin ..\..\..\..\Debug\Exe\RZA1LU_Sample.bin,0x18080000

r
g
q
