# tpl2020_midterm
Midterm for Programming Languages lecture.

Basically, it is a programming language with Turkish keywords. There are 3 main components: Lexer, parser and semantic analyzer.
It resembles Pascal as I was inspired by it. Parser works in top-down manner. Semantic analyzer will also "execute" the code to demonstrate the project.

Sample Code:
```
!! This example will compile with no errors
!! However repetition structure will only repeat ONCE. As it seemed enough for the assignment we didn't implement any code generation.
degiskenler

sayi : kucukSayi : 25;
sayi : buyukSayi : 2525;

ikili : kontrol : dogru;

basla

eger esitse(kontrol, dogru tersi)
    yap
        tekrarla kucukse(kucukSayi, buyukSayi)
        yap
        esitle(kucukSayi, topla(buyukSayi, 1));
        cik
    cik


bitir
```

Post Grading:
95/100 grade,
It made me realize that I suck at C,
Learned a lot about pointers (compared to where I was before)

It helped me to get AA in grade letters.
