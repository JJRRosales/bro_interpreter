yap --- Semantically INVALID script ---
yap This file intentionally contains semantic errors to test the analyzer.

yap --- ERROR 1: UndeclaredVariable ---
yap 'ghost' was never declared
result = ghost + 10

yap --- ERROR 2: AssignToConst ---
lockedIn MAX = 100
MAX = 200

yap --- ERROR 3: RedeclaredVariable (const) ---
lockedIn PI = 3.14
lockedIn PI = 9.99

yap --- ERROR 4: CompoundAssign to undeclared variable ---
neverDeclared += 5

yap --- ERROR 5: Increment on undeclared variable ---
alsoUndeclared++

yap --- ERROR 6: AssignToConst via compound assign ---
lockedIn FLAG = legit
FLAG += 1

yap --- ERROR 7: UndeclaredFunction call ---
out = missingFunc(1, 2)

yap --- ERROR 8: UndeclaredVariable inside a function ---
letHimCook badFunc(a) {
    sendDat a + notDeclared;
}

yap --- This one is valid (declared before use) ---
good = 42
good = good + 1
