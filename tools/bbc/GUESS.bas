10 REM GUESS - guess the number
20 MODE 7
30 N%=RND(100)
40 T%=0
50 PRINT "I am thinking of a number 1 to 100."
60 REPEAT
70   INPUT "Your guess";G%
80   T%=T%+1
90   IF G%<N% THEN PRINT "Bigger!"
100  IF G%>N% THEN PRINT "Smaller!"
110 UNTIL G%=N%
120 PRINT "Right! ";STR$(T%);" tries."
130 END
