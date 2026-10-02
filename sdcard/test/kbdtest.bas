10 REM KBDTEST.BAS - keyboard test for pico-class workstation acceptance
20 REM Prints the code of every byte received from the keyboard.
30 REM Printable characters are echoed after the code. Exit: Ctrl+C
40 PRINT "KEYBOARD TEST. Press keys one by one. Exit: Ctrl+C"
50 PRINT "Letters, digits, Enter, Esc, NumPad (NumLock on)"
60 N=0
70 GET A
80 N=N+1
90 PRINT N;": ";A;
100 IF A>32 AND A<127 THEN PUT 32, A
110 PRINT
120 GOTO 70
