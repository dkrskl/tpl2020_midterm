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
