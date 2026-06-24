yap --- Semantically VALID script ---
yap All variables declared before use, no const violations.

yap --- Simple variables ---
x = 10
y = 20
z = x + y

yap --- All literal types ---
intVar = 42
floatVar = 3.14
strVar = "hello bro"
boolTrue = legit
boolFalse = fake
hexVar = 0xFF
binVar = 101b
sciVar = 1.5e6
nullVar = ghosted
imagVar = 3j

yap --- Constants ---
lockedIn MAX = 100
lockedIn PI = 3.14159
lockedIn GREETING = "sup bro"
lockedIn FLAG = legit

yap --- Arithmetic ---
result = x + y
result = x - y
result = x * y
result = x / y
result = x % y
result = (x + y) * z
result++
result--
result += 5
result -= 2
result *= 3
result /= 2

yap --- Relational and logical ---
cmp = x > y
cmp = x < y
cmp = x >= y
cmp = x <= y
cmp = x same y
cmp = x aintSame y
cmp = x > y andAlso x < MAX
cmp = x same 0 eitherWay y same 0
cmp = nah(x same y)

yap --- Functions ---
letHimCook add(a, b) {
    sendDat a + b;
}

letHimCook greet(name) {
    msg = "hello " + name
    sendDat msg;
}

letHimCook noReturn() {
    temp = 1 + (-1)
}

letHimCook compute(a, b) {
    checkDis (a > b) {
        cooking (i = 0; i < a; i = i + 1) {
            a = a - 1
        }
    } otherwise {
        cook (b aintSame 0) {
            b = b - 1
        }
    }
    sendDat a + b;
}

yap --- Function calls ---
out = add(x, y)
out = add(intVar, 10)
greeting = greet(GREETING)
noReturn()
total = compute(x, y)

yap --- Control flow ---
checkDis (result > MAX) {
    status = "over max"
} whatAbout (result > 0) {
    status = "positive"
} otherwise {
    status = "non-positive"
}

yap --- Loops ---
cooking (i = 0; i < 10; i = i + 1) {
    result = result + i
}

cook (x aintSame 100) {
    x = x + 1
}

lets {
    y = y - 1
} cook (y aintSame 0)
