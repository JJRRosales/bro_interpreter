yap --- Test: Functions with one or more parameters ---

print("--- TESTING FUNCTION PARAMETERS ---")

yap 1. Zero parameters
letHimCook testZero() {
    print("Zero parameters works, legit")
}
testZero()

yap 2. One parameter
letHimCook testOne(x) {
    print("One parameter x =", x)
}
testOne(42)

yap 3. Two parameters
letHimCook testTwo(a, b) {
    print("Two parameters a =", a, "and b =", b)
}
testTwo("hello", "world")

yap 4. Three parameters
letHimCook testThree(x, y, z) {
    sum = x + y + z
    print("Three parameters sum =", sum)
}
testThree(10, 20, 30)

print("--- PARAMETERS TEST COMPLETE ---")
