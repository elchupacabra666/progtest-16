# Dialogové okno I

- **Hodnocení**: 3.0 bodů (žádný možný bonus)
- **Termín odevzdání**: 01.05.2023 11:59:59

## Zadání

Úkolem je navrhnout a implementovat sadu tříd, které budou simulovat prvky v uživatelském rozhraní. Konkrétně jde o **okno (CWindow)**, **tlačítko (CButton)**, **textový vstup (CInput)**, **textový popisek (CLabel)** a **combobox (CComboBox)**.

Tento úkol je zaměřen na návrh tříd, kde bude využito dědičnosti, polymorfismu a abstraktních metod. Pokud jsou tyto OOP prostředky použité racionálně, implementace nebude příliš dlouhá. Špatně navržený kód se bude opakovat a soubor bude zbytečně dlouhý.

Navržené třídy budou **simulovat GUI**, nic vykreslovat nebudou, jen budou modelovat uživatelské prvky. Třídy tedy půjde použít k sestavení modelu ovládacích prvků v okně, půjde manipulovat s jejich stavem a zobrazit jejich stav ve výstupním proudu.

Jednotlivá okna a ovládací prvky bude možné identifikovat pomocí jejich číselného **id**. Pokud **id** není unikátní, bude prvek fungovat, ale nepůjde jej podle **id** vyhledat.

### Správa oken a prvků

Okno bude reagovat na změnu umístění/velikosti. Poloha a velikost jednotlivých ovládacích prvků je zadána relativně (0 až 1) vzhledem k šířce a výšce okna. Změní-li se velikost okna, přepočítají se pozice a velikosti prvků.

### Rozhraní tříd

- **CButton**
    - Reprezentuje tlačítko.
    - Požadované funkce: konstruktor a operátor výstupu `<<`.

- **CInput**
    - Reprezentuje textový vstup.
    - Požadované funkce: konstruktor, operátor výstupu `<<`, `setValue(x)`, `getValue()`.

- **CLabel**
    - Reprezentuje statický text.
    - Požadované funkce: konstruktor a operátor výstupu `<<`.

- **CComboBox**
    - Reprezentuje combobox.
    - Požadované funkce: konstruktor, operátor výstupu `<<`, `add(x)`, `setSelected(x)`, `getSelected()`.

- **CWindow**
    - Reprezentuje okno.
    - Požadované funkce:
        - Konstruktor: `CWindow(id, title, absPos)`.
        - `add(x)`: Přidá ovládací prvek.
        - `setPosition(absPos)`: Změní pozici a velikost okna.
        - `search(id)`: Vyhledá ovládací prvek podle id.
        - Operátor výstupu `<<` pro zobrazení obsahu okna.

- **CRect**
    - Pomocná třída reprezentující obdélník. Tato třída je součástí testovacího prostředí a není potřeba ji měnit.

### Poznámky

- Používejte operátory přetypování `dynamic_cast` s rozmyslem. RTTI, `dynamic_cast` a `typeid` vedou k větvenému kódu, což zhoršuje čitelnost i rozšiřitelnost.
- Řešení musí využívat OOP principy: dědičnost a polymorfismus. Řešení, které tyto principy nevyužívá, bude odmítnuto při kompilaci.
- Výstup bude ve formě stromu, přičemž strom bude mít maximálně dvě úrovně. ID okna nebo ovládacího prvku bude uvedeno v hranatých závorkách, absolutní souřadnice pak v kulatých závorkách.

### Výstupní formát

Výpis bude obsahovat přepočtené absolutní souřadnice ovládacích prvků. Absolutní souřadnice jsou známé až po vložení ovládacího prvku do nějakého okna. Testovací prostředí proto nezkouší vypisovat objekty před jejich vložením do okna.
