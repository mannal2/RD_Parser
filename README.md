# RD Parser
EBNF로 문법이 정의되는 언어를 위한 해석기

### 문법
```
 <program> → {<declaration>} {<statement>}
 <declaration> → <type> <var> ;
 <statement> → <var> = <aexpr> ; | print <aexpr> ;  |
 while ( <bexpr> ) do ‘ { ’ {<statement>} ‘ } ’  ;  |
 if ( <bexpr> ) ‘ { ’ {<statement>} ‘ } ’ else ‘ { ’ {<statement>} ‘ } ’ ;
 <bexpr> → <var> <relop> <var>
 <relop> → == | != | < | >
 <aexpr> → <term> {( + | - ) <term>}
 <term> → <factor> { * <factor>}
 <factor> → [ - ] ( <number> | <var> | ‘(’<aexpr>‘)’ )
 <type> → integer
 <number> → <digit> {<digit>}
 <digit> → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 <var> → <alphabet>{<alphabet>}
 <alphabet> → a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | 
s | t | u | v | w | x | y | z
```

### 조건
1. 토큰사이에는 공백 문자가 1개 이상
2. 선언하지 않은 변수는 사용 불가
3. 선언된 변수는 0으로 자동 초기화
4. 산술연산은 왼쪽 결합성(Left Associativity)

### 출력
문법 오류시 Syntax Error! 출력, 그 외는 출력할 값 출력하고 다음 코드 입력받기

### 실행 예시
```
 >> integer k ; integer j ; k = 3 ; j = 20 ; print k + j ;
 23
 >> a = 10 ;
 Syntax Error!
 >> integer k ; integer j ; k = 30 ; j = 25 ; while ( k > j ) do { print ( k - j ) * 10 ; k = k - 1 ; } ; print k ;
 50 40 30 20 10 25
```
