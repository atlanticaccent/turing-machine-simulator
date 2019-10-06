states 18
start
leftWordCycle1
midLeftWordCycle0
midRightWordCycle1
rightWordCycle1
rewind
restart
midRightWordCycle0
rightWordCycle0
leftWordCycle0
midLeftWordCycle1
midRightWordCycleCarry
rightWordCycleCarry
0andCarry
carryAndCarry
lastCheck
accept +
reject -
alphabet 10 0 1 # A B ~ C X Y *
start 0 leftWordCycle0 A R
leftWordCycle0 0 leftWordCycle0 0 R
leftWordCycle0 1 leftWordCycle0 1 R
leftWordCycle0 # midLeftWordCycle0 # R
midLeftWordCycle0 # rightWordCycle0 # R
midLeftWordCycle0 * midLeftWordCycle0 * R
midLeftWordCycle0 0 midRightWordCycle0 * R
midRightWordCycle0 0 midRightWordCycle0 0 R
midRightWordCycle0 1 midRightWordCycle0 1 R
midRightWordCycle0 # rightWordCycle0 # R
rightWordCycle0 * rightWordCycle0 * R
rightWordCycle0 0 rewind * L
rightWordCycle0 1 reject 1 L
rightWordCycle0 _ lastCheck _ L
rightWordCycle0 C 0andCarry * R
0andCarry 0 reject 0 L
0andCarry 1 rewind * L
midLeftWordCycle0 1 midRightWordCycle1 * R
midRightWordCycle1 0 midRightWordCycle1 0 R
midRightWordCycle1 1 midRightWordCycle1 1 R
midRightWordCycle1 # rightWordCycle1 # R
rightWordCycle1 * rightWordCycle1 * R
rightWordCycle1 1 rewind * L
rightWordCycle1 0 reject 0 L
rightWordCycle1 C rightWordCycleCarry * R
start 1 leftWordCycle1 B R
leftWordCycle1 0 leftWordCycle1 0 R
leftWordCycle1 1 leftWordCycle1 1 R
leftWordCycle1 # midLeftWordCycle1 # R
midLeftWordCycle1 * midLeftWordCycle1 * R
midLeftWordCycle1 0 midRightWordCycle1 * R
midLeftWordCycle1 1 midRightWordCycleCarry * R
midRightWordCycleCarry 0 midRightWordCycleCarry 0 R
midRightWordCycleCarry 1 midRightWordCycleCarry 1 R
midRightWordCycleCarry # rightWordCycleCarry # R
rightWordCycleCarry * rightWordCycleCarry * R
rightWordCycleCarry 0 rewind C L
rightWordCycleCarry 1 reject 1 L
rightWordCycleCarry C carryAndCarry * R
carryAndCarry 0 reject 0 L
carryAndCarry 1 rewind C L
rewind * rewind * L
rewind # rewind # L
rewind 0 rewind 0 L
rewind 1 rewind 1 L
rewind A restart A R
rewind B restart B R
rewind ~ restart ~ L
rewind X restart X R
rewind Y restart Y R
restart 0 leftWordCycle0 X R
restart 1 leftWordCycle1 Y R
restart # midLeftWordCycle0 # R
restart ~ midLeftWordCycle0 ~ R
start # midLeftWordCycle0 ~ R
midLeftWordCycle1 # rightWordCycle1 # R
lastCheck 0 lastCheck 0 L
lastCheck * lastCheck * L
lastCheck # lastCheck # L
lastCheck X lastCheck X L
lastCheck Y lastCheck Y L
lastCheck 1 reject 1 L
lastCheck A accept A L
lastCheck B accept B L
lastCheck ~ accept ~ L