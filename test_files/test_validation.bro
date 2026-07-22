yap --- Test validation for variables, loops, and recursive functions ---

yap 1. Variable declaration
cont = -10
print("Initial cont:", cont)

yap 2. Loop (cooking)
cooking (i = -10; i < 5; i++) {
    cont += i
}
print("Cont after loop (expected 18):", cont)

yap 3. Recursive function
letHimCook sumatoria(n) {
    checkDis (n >= 1) {
        sendDat n + sumatoria(n - 1);
    } otherwise {
        sendDat 0;
    }
}

valor = 5
print("Sumatoria of", valor, "is (expected 15):", sumatoria(valor))
