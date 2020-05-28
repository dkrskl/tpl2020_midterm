!! This example will fail at SEMANTIC ANALYZING stage
degiskenler
karakter : degisken : "Merhaba Dunya!";
karakter : degiskenAlternatifMetin : "Alternatif Metin ~;.!";
sayi : degiskenSayi : 25;

basla

eger esitse(degiskenSayi, 25)
    yap
    esitle(degisken, degiskenAlternatifMetin); !! This is fine.
    esitle(degisken, degiskenSayi); !! However you can't assign an integer inside a string
    cik

bitir
