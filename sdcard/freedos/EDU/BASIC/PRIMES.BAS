10 REM Ž‘’›… —ˆ‘‹€: Ž„€ Žƒ€ŒŒ€ „‹Ÿ GW-BASIC ˆ BASCOMP
20 PRINT "‘ŠŽ‹œŠŽ Ž‘’›• —ˆ‘…‹ Ž’ 2 „Ž M?"
30 INPUT "M (… Ž‹œ˜… 32000)", M
40 GOSUB 200
50 T = S
60 C = 0
70 FOR N = 2 TO M
80 D = 2
90 IF D * D > N THEN GOTO 130
100 IF N MOD D = 0 THEN GOTO 140
110 D = D + 1
120 GOTO 90
130 C = C + 1
140 NEXT N
150 GOSUB 200
160 IF S < T THEN S = S + 3600
170 PRINT "€‰„…Ž Ž‘’›•:"; C
180 PRINT "‘…Š“„:"; S - T
190 END
200 REM —€‘›: ‘…Š“„› ‘ €—€‹€ —€‘€
210 S = VAL(MID$(TIME$, 4, 2)) * 60 + VAL(MID$(TIME$, 7, 2))
220 RETURN
