print("--- BRO LANGUAGE INTERPRETER TEST ---")

print("1. Variables and Constants")
lockedIn MAX = 100
a = 10
b = 20
print("a is", a, "and b is", b)

print("2. Arithmetic and Compound Assignment")
a += 5
b = b * 2
print("a is now", a, "and b is now", b)

print("3. Conditional (If/Else)")
checkDis (a > 10) {
    print("a is greater than 10, legit")
} otherwise {
    print("a is not greater than 10, fake")
}

print("4. Loops (While)")
counter = 0
cook (counter < 3) {
    print("Loop counter:", counter)
    counter++
}

print("5. Loops (For)")
cooking (i = 0; i < 3; i++) {
    print("For loop i:", i)
}

print("6. Functions and Scope")
letHimCook addAndMultiply(x, y) {
    z = x + y
    sendDat z * 2;
}

result = addAndMultiply(5, 5)
print("Function result (5+5)*2 =", result)

print("--- TEST COMPLETE ---")
