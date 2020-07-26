# PDV zkouska

Skoro vyreseny zkouskovy termin, ktery je verejne pristupny na strankach CW:

https://cw.fel.cvut.cz/wiki/_media/courses/b4b36pdv/zk_05.zip - balicek ke stazeni

## Zadani

*Upozornění:
Váš kód musí být korektní z hlediska vícevláknového přístupu ke sdíleným proměnným. Pokud nezabezpečíte synchronizaci, budeme Vaše řešení ručně penalizovat.*


### Úloha 1:
Vaším úkolem je v daném poli std::vector<std::string> vector projít slova a zjistit, jaká je Levenshteinho vzdálenost každých dvou slov v tomto poli obsažených. Zároveň musíte najít i nejnižší index dvojice slov takový, že vzdálenost mezi těmito slovy je největší.

Zparalelizujte metodu distances (pouze tuto metodu, zadnou jinou) v souboru levenshtein.cpp, která vrátí sumu vzdáleností slov ve vector každého s každým. Index s maximální vzdáleností uložte do proměnné maxIdx. Soubor levenshtein.cpp odevzdávejte do BRUTE do úlohy ZK_05_A.


### Úloha 2:
Vaším úkolem je paralelizovat aplikaci lokálního filtru ve čtvercové matici čísel na základě čtyř-okolí prvku. Filtr se aplikuje v iteracích, přičemž v každé iteraci se pro každý prvek table(i,j) aplikuje výpočet:

`table(i,j) = sqrt( (table(i,j)^2 + table(i-1,j)^2 + table(i+1,j)^2 + table(i,j-1)^2 + table(i,j+1)^2)/3 )`

sekvenčně od prvku 0,0 po řádcích (pokud prvky v okolí přesahují index matice, použije se hodnota 0).

Zparalelizujte metodu filtering v souboru filter.cpp, která tento výpočet provádí. Soubor filter.cpp odevzdávejte do BRUTE do úlohy


## Vysledky

Prumerny cas behu v ms


|              | Sekvencni  | Referencni  | Paralelizace                 |
|--------------|------------|-------------|------------------------------|
| levenshtein  |     162    |   63.3748   |  64.4912 (1.02x od referenci)|
|  filter      |  1192.551  |   486.6795  |  262.073 (0.54x od referenci)|
