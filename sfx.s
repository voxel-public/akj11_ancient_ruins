
;this file for FamiTone2 libary generated by FamiStudio
FT_DPCM_PTR=$c000
	.export _akj_sfx
_akj_sfx:
	.word @ntsc
	.word @ntsc
@ntsc:
	.word @sfx_ntsc_barrierswitch
	.word @sfx_ntsc_dash
	.word @sfx_ntsc_die
	.word @sfx_ntsc_pickup
	.word @sfx_ntsc_reform

@sfx_ntsc_barrierswitch:
	.byte $87,$a6,$88,$02,$86,$8f,$8a,$06,$89,$3d,$01,$89,$3e,$01,$87,$80
	.byte $01,$89,$3f,$01,$87,$5c,$02,$87,$3a,$02,$87,$1a,$02,$87,$fb,$88
	.byte $01,$02,$87,$df,$02,$87,$c4,$01,$89,$3e,$01,$87,$ab,$02,$87,$93
	.byte $02,$87,$7c,$01,$89,$3d,$01,$87,$67,$02,$87,$2d,$02,$87,$ef,$88
	.byte $00,$89,$3c,$02,$87,$c9,$01,$87,$a9,$01,$8a,$07,$89,$3d,$01,$89
	.byte $3e,$01,$8a,$06,$89,$3f,$01,$8a,$04,$01,$89,$3e,$01,$8a,$03,$89
	.byte $3f,$01,$8a,$02,$01,$8a,$04,$01,$89,$3e,$01,$86,$80,$8a,$06,$89
	.byte $3f,$02,$89,$3e,$01,$89,$3b,$01,$89,$33,$01,$89,$30,$02,$8a,$0f
	.byte $89,$3d,$01,$89,$3e,$02,$89,$3f,$0c,$89,$3e,$06,$89,$3d,$05,$89
	.byte $3c,$04,$89,$3b,$05,$89,$3a,$05,$89,$39,$03,$89,$38,$03,$89,$37
	.byte $03,$89,$36,$03,$89,$35,$02,$89,$34,$02,$89,$33,$02,$89,$32,$02
	.byte $89,$31,$01,$00
@sfx_ntsc_dash:
	.byte $8a,$0c,$89,$3f,$01,$8a,$0b,$01,$8a,$0a,$01,$8a,$09,$01,$8a,$08
	.byte $01,$8a,$0c,$01,$8a,$0b,$01,$8a,$0a,$01,$8a,$09,$01,$8a,$08,$01
	.byte $8a,$0c,$01,$8a,$0b,$01,$8a,$0a,$01,$8a,$09,$01,$8a,$08,$01,$8a
	.byte $00,$01,$89,$3e,$01,$89,$3b,$01,$89,$33,$01,$00
@sfx_ntsc_die:
	.byte $82,$00,$81,$bd,$80,$bf,$8a,$03,$89,$3f,$01,$81,$fd,$02,$82,$01
	.byte $81,$2d,$01,$81,$67,$02,$81,$df,$03,$82,$02,$81,$80,$01,$8a,$04
	.byte $02,$81,$ce,$02,$82,$03,$81,$56,$02,$81,$89,$02,$81,$f8,$02,$8a
	.byte $05,$02,$82,$04,$81,$b8,$01,$80,$b0,$07,$8a,$06,$0a,$8a,$07,$0a
	.byte $8a,$09,$0a,$8a,$0b,$0a,$8a,$0d,$1b,$89,$3e,$03,$89,$3d,$02,$89
	.byte $3c,$01,$89,$3b,$02,$89,$3a,$01,$89,$39,$01,$89,$38,$01,$89,$37
	.byte $01,$89,$36,$01,$89,$35,$01,$89,$33,$01,$89,$31,$7f,$7f,$7f,$7f
	.byte $7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$7f,$22
	.byte $00
@sfx_ntsc_pickup:
	.byte $82,$00,$81,$d5,$80,$bd,$8a,$0a,$89,$3f,$01,$80,$be,$02,$80,$bf
	.byte $0c,$80,$be,$06,$80,$bd,$05,$80,$bc,$01,$89,$3e,$03,$80,$bb,$89
	.byte $3d,$02,$89,$3c,$01,$89,$3b,$02,$80,$ba,$89,$3a,$01,$89,$39,$01
	.byte $89,$38,$01,$89,$37,$01,$89,$36,$01,$81,$86,$80,$bd,$8a,$08,$89
	.byte $3f,$01,$80,$be,$02,$80,$bf,$0c,$80,$be,$05,$81,$8e,$80,$bd,$8a
	.byte $09,$01,$80,$be,$02,$80,$bf,$0c,$80,$be,$06,$80,$bd,$05,$80,$bc
	.byte $01,$89,$3e,$03,$80,$bb,$89,$3d,$02,$89,$3c,$01,$89,$3b,$02,$80
	.byte $ba,$89,$3a,$01,$89,$39,$01,$89,$38,$01,$89,$37,$01,$89,$36,$01
	.byte $80,$b9,$89,$35,$01,$89,$33,$01,$89,$31,$01,$80,$b8,$03,$80,$b7
	.byte $03,$80,$b6,$03,$80,$b5,$02,$80,$b4,$02,$80,$b3,$02,$80,$b2,$01
	.byte $80,$b0,$00
@sfx_ntsc_reform:
	.byte $82,$04,$81,$74,$80,$bf,$87,$ab,$88,$01,$86,$8f,$8a,$09,$89,$3f
	.byte $01,$87,$67,$01,$87,$3f,$01,$81,$34,$87,$0c,$01,$87,$fd,$88,$00
	.byte $89,$f0,$01,$87,$d5,$01,$82,$03,$81,$56,$01,$87,$fd,$02,$82,$02
	.byte $81,$f9,$01,$89,$3f,$01,$87,$c9,$01,$87,$a9,$01,$81,$80,$87,$9f
	.byte $01,$89,$f0,$01,$87,$c9,$01,$82,$01,$81,$df,$01,$87,$b3,$01,$87
	.byte $96,$01,$81,$7c,$87,$7e,$01,$87,$77,$89,$3f,$03,$87,$c9,$01,$81
	.byte $52,$87,$e1,$89,$f0,$02,$81,$0c,$87,$a9,$01,$87,$86,$01,$87,$77
	.byte $02,$87,$6a,$89,$3f,$01,$87,$77,$02,$87,$6a,$01,$81,$67,$01,$89
	.byte $f0,$04,$86,$80,$01,$80,$b0,$89,$3f,$04,$8a,$07,$05,$8a,$05,$03
	.byte $8a,$04,$02,$8a,$03,$01,$8a,$02,$01,$8a,$01,$03,$8a,$00,$04,$00