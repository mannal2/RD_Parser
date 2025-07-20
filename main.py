#변수저장 딕셔너리
varDict = {}
token=""
tokens = []
index=0
end=0

def inputSentence():
    global tokens, index, varDict, end
    end=0
    varDict={}
    index=0
    tokens=[]
    text = input(">> ")
    if text=="":
        return 1
    tokens = text.split()
    

def nextToken():
    global index, token
    if index>=len(tokens):
       token=""
       return
    token = tokens[index]
    index+=1

def error():
    global end
    end = 1
    

def check(exp):
    if token == exp:
        nextToken()
    else:
        error()

def number():
    if end==1:
        return 0
    if not token.isdigit():
        error()
        return 0
    num = int(token)
    nextToken()
    return num

def var():
    if end==1:
        return None
    if not token.isalpha():
        error()
        return None
    if not token.islower():
        error()
        return None
    if token == "print" or token == "integer":
        error()
        return None
    if token == "while" or token=="do" or token == "if" or token=="else":
        error()
        return None

    val = token
    nextToken()
    return val

def type():
    if end==1:
        return 0
    if token == "integer":
        return 1
    else:
        return 0

def factor():
    if end==1:
        return 0
    sign = 1
    if token == "-":
        sign = -1
        nextToken()
    if token and token[0].isdigit():
        return sign * number()
    if token and token[0].isalpha():
        value = varDict.get(var())
        if value == None:
            error()
            return 0
        else:
            return sign * value
    if token == "(":
        nextToken()
        value = aexpr()
        check(")")
        return sign*value
    error()
    return 0

def term():
    if end==1:
        return 0
    value = factor()
    while token == "*":
        nextToken()
        value *= factor()
    return value

def aexpr():
    if end==1:
        return 0
    value = term()
    while token=="+" or token=="-":
        op=token
        nextToken()
        if op=="+":
            value += term()
        else:
            value -= term()
    return value
    
def relop():
    if end==1:
        return 0
    text = token
    nextToken()
    v = 0
    if text == "==":
        v=1
    elif text == "!=":
        v=2
    elif text == "<":
        v=3
    elif text == ">":
        v=4
    return v

def bexpr():
    if end==1:
        return 0
    var1 = varDict.get(var())
    op = relop()
    var2 = varDict.get(var())
    if var1 == None or var2 == None:
        error()
        return 0
    if op==1:
        return var1==var2
    elif op==2:
        return var1!=var2
    elif op==3:
        return var1 < var2
    elif op==4:
        return var1 > var2
    else:
        error()
    return 0

def declaration():
    if end==1:
        return 0
    if type():
        nextToken()
        name = var()
        check(";")
        if name in varDict:
            error()
            return 0
        varDict[name]=0
        return 1
    return 0

def statement(printQueue, execute):
    global token, index
    if end==1:
        return
    
    if token=="print":
        nextToken()
        if end==1:
            return
        value = aexpr()
        check(";")
        if end==1:
            return
        if execute==1:
            printQueue.append(value)
    elif token=="while":
        save_index = index;
        nextToken()
        check("(")
        e = bexpr()
        if end==1:
            return
        if execute==0:
            e=0
        check(")")
        check("do")
        check("{")

        while token != "}":
            statement(printQueue, e)
            if end==1:
                return
        check("}")
        check(";")
        
        if e==1:
            token = tokens[save_index-1]
            index = save_index
            statement(printQueue, e)
    elif token == "if":
        nextToken()
        check("(")
        e = bexpr()
        if end==1:
            return
        ce = e
        if execute==0:
            e=0;
        check(")")
        check("{")
        while token!="}":
            statement(printQueue, e)
            if end==1:
                return
        check("}")
        check("else")
        check("{")
        if ce==1:
            e=0
        else:
            e=1
        if execute==0:
            e=0
        while token != "}":
            statement(printQueue, e)
            if end == 1:
                return
        check("}")
        check(";")
    elif token and token[0].isalpha():
        name = var()
        if end==1 or name is None:
            return
        check("=")
        value = aexpr()
        check(";")
        if end==1:
              return
        if name in varDict:
              if execute==1:
                varDict[name]=value
        else:
            error()
            return
    else:
        error()

def program():
    printQueue=[]
    r=1
    while token != "":
        if r==1:
            r=declaration()
        else:
            statement(printQueue, 1)
        if end==1:
            print("Syntax Error!")
            return
    for val in printQueue:
        print(val, end=" ")
    if printQueue:
        print()

while True:
    if inputSentence()==1:
        break
    nextToken()
    program()

    
    

