yap --- Test: Function return statement (sendDat) ---

print("--- TESTING FUNCTION RETURNS ---")

yap 1. Simple return
letHimCook getConstant() {
    sendDat 100;
}
res1 = getConstant()
print("getConstant returned:", res1)

yap 2. Return with conditional
letHimCook maxVal(a, b) {
    checkDis (a > b) {
        sendDat a;
    } otherwise {
        sendDat b;
    }
}
res2 = maxVal(45, 87)
print("maxVal(45, 87) returned:", res2)

yap 3. Early return
letHimCook checkThreshold(val) {
    checkDis (val < 0) {
        sendDat "negative";
    }
    
    checkDis (val > 100) {
        sendDat "too high";
    }
    
    sendDat "ok";
}
print("checkThreshold(-5) is:", checkThreshold(-5))
print("checkThreshold(150) is:", checkThreshold(150))
print("checkThreshold(50) is:", checkThreshold(50))

yap 4. Recursive return
letHimCook factorial(n) {
    checkDis (n <= 1) {
        sendDat 1;
    } otherwise {
        sendDat n * factorial(n - 1);
    }
}
res3 = factorial(5)
print("factorial(5) returned:", res3)

print("--- RETURNS TEST COMPLETE ---")
