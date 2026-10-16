; Parameter block read by the start-up code (crt0): where the autoload list and the BSS are, followed by a version
; word, a marker pair and two library identification strings.

	.text
	.global BuildInfo
	.extern data_020c0100
	.extern data_020c0118
	.extern data_020baf00

	.type BuildInfo, @object
BuildInfo:
	.word data_020c0100     ; autoload list: start
	.word data_020c0118     ; autoload list: end
	.word data_020baf00     ; first autoload destination
	.word data_020baf00     ; BSS start
	.word 0x02219440        ; BSS end
	.word 0                 ; end of the compressed part (0: not compressed)
	.word 0x04027539        ; version word
	.word 0xdec00621        ; marker
	.word 0x2106c0de        ; marker, byte-reversed
	.byte "[SDK+NINTENDO:BACKUP]", 0, 0, 0
	.byte "[SDK+Actimagine:Mobiclip SDK V1.1.1]", 0, 0, 0, 0
	.size BuildInfo, 100
