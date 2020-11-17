<h1>Domácí úkol č. 2: Interpret Robota Karla</h1>
<h2 id="robot-karel">Robot Karel</h2>
<p>Cílem tohoto úkolu je navrhnout a napsat interpret pro zjednodušený programovací jazyk <a href="http://mormegil.wz.cz/prog/karel/prog_doc.htm">Robot Karel</a>.</p>
<hr />
<h2 id="zadání">Zadání</h2>
<p>Napište knihovnu, která implementuje interpret pro zjednodušenou verzi programovacího jazyka Robot Karel. Vstupem pro interpret jsou dva soubory. První popisuje svět, ve kterém se robot pohybuje, druhý soubor obsahuje program pro robota. Svět se skládá z prázdných políček a stěn. Na některých políčkách se mohou nacházet značky, které robot sbírá nebo pokládá.</p>
<p>Přesná syntax souborů a sémantika programu je popsána níže.</p>
<p>Strukturu řešení si můžete navrhnout téměř libovolně, včetně názvů souborů a tříd, kromě těchto omezení:</p>
<ul>
<li><strong>nemodifikujte</strong> dodané soubory <code>defs.hpp</code>, <code>errors.hpp</code> ani <code>errors.cpp</code></li>
<li>odevzdávejte pouze soubory s příponami <code>.hpp</code>, <code>.h</code> nebo <code>.cpp</code></li>
<li>řešení <strong>nepište</strong> do souboru <code>main.cpp</code> ani souborů, které začínají <code>test-</code>
<ul>
<li>Kontr tyto soubory v repozitáři ignoruje, používejte je pouze pro vlastní testování.</li>
</ul></li>
<li>řešení musí obsahovat (alespoň) třídy <code>Robot</code>, <code>World</code> a <code>Interpret</code>, jejichž rozhraní je zadáno v souboru <code>interface.hpp</code>.
<ul>
<li>Tyto třídy můžete přesunout do jiných souborů, pokud chcete.</li>
</ul></li>
</ul>
<p>Můžete si do svého řešení přidat i další veřejné metody, pokud dávají smysl. Rozhodně doporučujeme napsat si další třídy, které řeší dílčí části úkolu.</p>
<h2 id="popis-dodaných-souborů">Popis dodaných souborů</h2>
<p>V souboru <code>defs.hpp</code> naleznete struktury, kterými bude rozhraní Vašeho interpretu komunikovat. Tento soubor nemodifikujte.</p>
<h3 id="struct-tile"><code>struct Tile</code></h3>
<p>Popisuje jedno políčko světa.</p>
<ul>
<li><code>bool wall</code> je pravdivé právě tehdy, pokud políčko reprezentuje stěnu.</li>
<li><code>unsigned marks</code> je počet značek na políčku. Pro stěnu musí být vždycky <code>0</code>.</li>
</ul>
<h3 id="struct-position"><code>struct Position</code></h3>
<p>Pozice robota nebo políčka ve světě.</p>
<ul>
<li><code>int x</code></li>
<li><code>int y</code></li>
</ul>
<p>Typ souřadnic je znaménkový proto, aby bylo možné pohodlněji pracovat i se zápornými koordináty.</p>
<h3 id="enum-class-direction"><code>enum class Direction</code></h3>
<p>Definuje čtyři směry, kterými může být robot natočen.</p>
<ul>
<li><code>Direction::North</code></li>
<li><code>Direction::East</code></li>
<li><code>Direction::South</code></li>
<li><code>Direction::West</code></li>
</ul>
<h3 id="complain"><code>Complain</code></h3>
<p>V souboru <code>errors.hpp</code> je definována hrozivě vypadající třída <code>Complain</code>, která slouží k oznamování chyb vašeho interpretu. C++ již má k tomuto účelu výjimky, které ještě ale nebyly na přednášce řádně vysvětleny, proto se jim vyhneme oklikou. Pochopení vnitřního fungování třídy vyžaduje alespoň středně pokročilé znalosti magie, proto se zaměříme pouze na veřejné rozhraní, které budete používat ve svém kódu.</p>
<ul>
<li>Třída je <em>statická</em>, tudíž nelze vytvořit žádnou její instanci; můžete pouze volat metody, před které <strong>vždy</strong> pište <code>Complain::</code>.</li>
<li>Předpokládejte, že volání libovolné metody <strong>ukončí</strong> volání vašeho kódu, chybou se dál nemusíte zabývat.</li>
<li>Hodnoty pojmenovaných parametrů se <strong>testují</strong>; některé testy očekávají, že zavoláte správnou metodu <code>Complain</code> se správným parametrem.</li>
<li><strong>Nepředpokládejte</strong>, že volání vyhazují konkrétní typ výjimky, testy mohou používat jinou implementaci třídy.</li>
</ul>
<p>Konkrétní metody jsou pak</p>
<ul>
<li><code>Complain::invalidWorld(line, ...)</code> značí chybu v popisu světa na řádku <code>line</code>.</li>
<li><code>Complain::invalidSource(line, ...)</code> značí chybu v textu programu na řádku <code>line</code>.</li>
<li><code>Complain::undefinedReference(where, to, ...)</code> znamená, že v proceduře <code>where</code> některá instrukce odkazuje na neexistující proceduru <code>to</code>.</li>
<li><code>Complain::runtimeError(procedure, index, ...)</code> zavolá interpret pro situace, které mohou nastat za běhu programu, např. pokud robot narazí do stěny.</li>
<li><code>Complain::error(...)</code> můžete používat na vlastní testování a ošetření chyb, které zadání nespecifikuje. V testech k takovýmto situacím nedochází.</li>
</ul>
<p>Místo <code>...</code> můžete napsat libovolně dlouhý seznam parametrů, které lze vypsat operátorem <code>&lt;&lt;</code> na <code>std::cerr</code> (tedy <code>std::string</code>, číselné typy atd.). Tyto parametry se netestují, používejte je pro bližší popis chyb při vlastním testování. Pokud neuvedete žádné další parametry, použije se <code>unspecified error</code>.</p>
<p>Příklad použití ve smyšleném kódu:</p>
<div class="sourceCode" id="cb1"><pre class="sourceCode c"><code class="sourceCode c"><a class="sourceLine" id="cb1-1" data-line-number="1"><span class="cf">if</span> (width != <span class="dv">666</span> || height != <span class="dv">666</span>)</a>
<a class="sourceLine" id="cb1-2" data-line-number="2">    Complain::invalidWorld(lineno, <span class="st">&quot;a world of size &quot;</span>, width, <span class="st">&quot;x&quot;</span>, height, <span class="st">&quot; is not suitable for The Beast&quot;</span>);</a>
<a class="sourceLine" id="cb1-3" data-line-number="3"></a>
<a class="sourceLine" id="cb1-4" data-line-number="4">summonBeast(world);</a></code></pre></div>
<p>Příklad výstupu, pokud svět nemá požadované rozměry:</p>
<pre><code>Invalid world
on line 2: a world of size 15x18 is not suitable for The Beast</code></pre>
<p>Program dále nepokračuje.</p>
<hr />
<h2 id="konstruktor">Konstruktor</h2>
<p>Úkolem konstruktoru je vytvořit svět podle popisu a sestavit program pro robota. Doporučujeme si pro tyto účely vytvořit pomocné třídy.</p>
<pre><code>Interpret::Interpret(std::istream&amp; world, std::istream&amp; source)</code></pre>
<ul>
<li><code>world</code> je vstupní proud pro definici světa</li>
<li><code>source</code> je vstupní proud pro zdrojový kód programu.</li>
</ul>
<p>Tyto vstupy se chovají jako <code>std::cin</code> a můžete z nich číst třeba operátorem <code>&gt;&gt;</code> nebo funkcí <code>std::getline</code>. Nepředpokládejte však, že na nich lze volat metodu <code>seekg</code>!</p>
<p>V následujícím textu <em>bílým znakem</em> rozumíme libovolný typ mezery v ASCII tabulce kromě znaku pro nový řádek (<code>\n</code>).</p>
<h2 id="syntax-světa">Syntax světa</h2>
<p>První dva řádky souboru jsou ve formátu</p>
<pre><code>WIDTH HEIGHT
POSX POSY DIR</code></pre>
<ul>
<li><code>WIDTH</code> a <code>HEIGHT</code> jsou <em>celá kladná</em> čísla udávající rozměry obdélníkového světa.</li>
<li><code>POSX</code> a <code>POSY</code> jsou <em>celá nezáporná</em> čísla a udávají počáteční polohu robota. Robot musí začínat na mapě a nesmí být zazděný.</li>
<li><code>DIR</code> je jeden ze znaků <code>n</code>, <code>e</code>, <code>s</code> a <code>w</code> a značí počáteční směr, kterým se robot dívá.</li>
</ul>
<p>Nadbytečné bílé znaky na těchto dvou řádcích ignorujte.</p>
<p>Pak následuje mapa světa, která musí mít <em>přesně</em> <code>HEIGHT</code> řádků, každý řádek pak musí mít <em>přesně</em> <code>WIDTH+1</code> znaků. Prvních <code>WIDTH</code> znaků značí typ políčka na dané pozici:</p>
<ul>
<li><code>' '</code> (mezera) nebo <code>'0'</code> (nula) je prázdné políčko,</li>
<li><code>'1'</code> až <code>'9'</code> je políčko, na kterém se nachází daný počet značek,</li>
<li><code>'#'</code> (mřížka) je stěna,</li>
<li>žádné jiné znaky nejsou povoleny.</li>
</ul>
<p>Posledním znakem řádku musí být <code>'\n'</code>.</p>
<p>Levý horní roh (první znak mapy) má pozici <code>[0,0]</code>, pravý dolní roh (poslední znak mapy před <code>'\n'</code>) má pozici <code>[WIDTH-1, HEIGHT-1]</code>. <em>Okrajem světa</em> nazveme pozice ve vzdálenosti 1 od mapy, tedy <em>neplatné</em> pozice se souřadnicemi</p>
<ul>
<li><em>x ∈ { -1, <code>WIDTH</code> }</em> a <em>0 ≤ y &lt; <code>HEIGHT</code></em> nebo</li>
<li><em>y ∈ { -1, <code>HEIGHT</code> }</em> a <em>0 ≤ x &lt; <code>WIDTH</code></em>.</li>
</ul>
<p>Nalezené chyby signalizujte voláním <code>Complain::invalidWorld(lineno, ...)</code>:</p>
<ul>
<li><code>lineno</code> je číslo řádku s první nalezenou chybou.</li>
<li>Řádky souboru se číslují od <code>1</code>.</li>
<li>Pokud některý řádek chybí nebo je navíc, ohlaste chybu právě na tomto řádku.</li>
<li>Pokud se robot nachází ve stěně, ohlaste chybu na řádku s polohou robota.</li>
</ul>
<h4 id="příklad">Příklad</h4>
<pre><code>4   3
1   1   e
####
# 3#
####
####</code></pre>
<p>má navíc poslední řádek, proto interpret zavolá <code>Complain::invalidWorld(6, ...)</code>. Pokud bychom tento řádek smazali, dostaneme korektní soubor, který popisuje svět široký 4 a vysoký 3 políčka. Okraje světa tvoří stěny. Na levém volném políčku se nachází robot, který se dívá na volné políčko směrem na východ. Zde jsou položeny tři značky.</p>
<h2 id="syntax-programu">Syntax programu</h2>
<p>Druhým krokem konstruktoru je analýza programu pro robota. Zde byste měli navíc program převést do podoby, kterou můžete jednoduše interpretovat.</p>
<p>Při programování je často užitečné psát komentáře nebo odsadit blok kódu.</p>
<ul>
<li>Na začátku a konci <strong>každého</strong> řádku ignorujte všechny bílé znaky.</li>
<li>Pokud je první nebílý znak <code>#</code> (mřížka), pak jde o komentář a ignorujte celý řádek.</li>
<li>Ignorujte všechny řádky, které obsahují pouze bílé znaky.</li>
</ul>
<p>Zbyde vám program, který se skládá z libovolného počtu <em>procedur</em>. Procedury mají následující formát:</p>
<pre><code>DEFINE NAME
INSTRUCTION
INSTRUCTION
...
END</code></pre>
<ul>
<li><code>DEFINE</code> a <code>END</code> jsou klíčová slova, žádná instrukce ani procedura se nesmí jmenovat stejně.</li>
<li><code>NAME</code> je název procedury.
<ul>
<li>Musí být v programu <strong>jedinečný</strong>.</li>
<li><em>Od klíčového slova <code>DEFINE</code> je oddělen alespoň jedním bílým znakem.</em></li>
<li>Smí obsahovat pouze malá a velká písmena anglické abecedy (<code>a</code>-<code>z</code>, <code>A</code>-<code>Z</code>) nebo arabské číslice (<code>0</code>-<code>9</code>).</li>
<li>Nesmí být stejný jako instrukce vyjmenované v seznamu níže.</li>
</ul></li>
<li>Mezi <code>DEFINE</code> a následujícím <code>END</code> se nachází tělo procedury.
<ul>
<li>Tělo tvoří 0 nebo více instrukcí.</li>
<li>Každá instrukce je na samostatném řádku.</li>
<li>Pokud instrukce očekává parametr, pak je tento parametr na stejném řádku za instrukcí oddělen alespoň jedným bílým znakem.</li>
</ul></li>
</ul>
<p>Instrukce se dělí do tří kategorií:</p>
<ul>
<li><strong>Volání procedury</strong> je název již definované procedury nebo té, která se právě definuje. Volání nemají žádné parametry.</li>
<li><p><strong>Vestavěné instrukce</strong>, které nemají žádné parametry:</p>
<p><code>SKIP</code>, <code>BREAK</code>, <code>HALT</code>, <code>MOVE</code>, <code>LEFT</code>, <code>RIGHT</code>, <code>PICKUP</code>, <code>PUTDOWN</code></p></li>
<li><p><strong>Instrukce skoku</strong> očekávají <em>přesně</em> jeden parametr oddělený alespoň jedním bílým znakem:</p>
<p><code>IFWALL</code>, <code>IFMARK</code>, <code>ELSE</code></p>
<p>Parametrem pro tyto instrukce je buď <em>vestavěná instrukce</em>, nebo <em>volání procedury</em>.</p></li>
</ul>
<p>Chyby syntaxe signalizujte voláním <code>Complain::invalidSource(lineno, ...)</code>.</p>
<ul>
<li><code>lineno</code> je číslo řádku.</li>
<li>Řádky zdrojového kódu se číslují od <code>1</code>.</li>
<li>Zdrojový kód je <em>case sensitive</em>, tedy <code>ELSE</code> není totéž co <code>else</code>.</li>
<li>Chybou je i to, pokud se na řádku nachází více slov, než se očekává.</li>
<li>Pokud soubor neobsahuje koncové <code>END</code> pro aktuální proceduru, ohlaste chybu na chybějícím řádku.</li>
<li>Instrukce <code>ELSE</code> není povinná, ale pokud se v kódu vyskytuje, pak musí následovat po <code>IFMARK</code> nebo <code>IFWALL</code>.</li>
<li>Žádná instrukce ani volání nesmí obsahovat jiné znaky, než <code>0</code>-<code>9</code>, <code>a</code>-<code>z</code> a <code>A</code>-<code>Z</code>.</li>
</ul>
<h3 id="kontrola-referencí">Kontrola referencí</h3>
<p>Navíc proveďte i sémantickou analýzu programu. Ta bude velice jednoduchá, pouze ověřte, že každé <em>volání procedury</em> skutečně odkazuje na existující proceduru. Pokud tomu tak není, zavolejte <code>Complain::undefinedReference(from, to, ...)</code></p>
<ul>
<li><code>from</code> je název procedury s chybnou instrukcí.</li>
<li><code>to</code> je název procedury, kterou instrukce volá.</li>
</ul>
<p>Jako vstupní bod programu se použije procedura <code>MAIN</code>. Pokud program tuto proceduru neobsahuje, zavolejte <code>Complain::undefinedReference(&quot;@ENTRY&quot;, &quot;MAIN&quot;, ...)</code>. Řetězec <code>@ENTRY</code> samozřejmě <em>není</em> platný název pro proceduru, proto jej zde použijte na indikaci tohoto speciálního případu.</p>
<p>Tuto kontrolu lze provádět zároveň se syntaktickou analýzou.</p>
<h4 id="příklad-1">Příklad</h4>
<p>Pro lepší ilustraci přidáme k zdrojovému kódu čísla řádků.</p>
<pre><code>01| DEFINE else
02|    # This is a comment
03|    # Special characters like @#$%^ are allowed here
04|    # And so are empty lines
05|
06|    SKIP
07|    IFWALL BREAK
08|    SKIP
09|    ELSE else
10| END
11|
12| DEFINE MAIN
13| # Indentation is not required
14| else
15|             # Insane indentation is allowed
16|                                         END</code></pre>
<p>Instrukce <code>ELSE</code> na řádku <code>09</code> následuje po <code>SKIP</code>, což není povoleno. Program tedy zavolá <code>Complain::invalidSource(9, ...)</code>.</p>
<p>Řádek <code>09</code> nahraďme instrukcí <code>DO_A_BARREL_ROLL</code>. Jelikož instrukce obsahuje nepovolený znak <code>_</code>, jde tedy znovu o syntaktickou chybu. Pokud bychom ale napsali <code>DoABarrelRoll</code>, pak jde o instrukci volání, která nemá v kódu definovaný cíl. V takovém případě se zavolá <code>Complain::undefinedReference(&quot;else&quot;, &quot;DoABarrelRoll&quot;, ...)</code>.</p>
<p>Pokud instrukci na řádku <code>09</code> smažeme nebo zakomentujeme řádek <code>08</code>, nebude kód obsahovat žádné další chyby.</p>
<h2 id="chování-interpretu">Chování interpretu</h2>
<ul>
<li>Interpret je ve stavu <em>běžící</em>, pokud nedošlo k žádné chybě hlášené třídou <code>Complain</code> a vykonávání programu ještě neskončilo.</li>
<li>Po dokončení vykonávání programu je interpret ve stavu <em>zastaven</em>.</li>
<li><em>Aktuální procedurou</em> je ta, jejíž tělo interpret právě vykonává.</li>
<li>Pokud procedura <code>A</code> zavolá proceduru <code>B</code>, pak <code>A</code> označíme <em>volající procedurou</em> a <code>B</code> <em>volanou procedurou</em>.</li>
<li><em>Index instrukce</em> v proceduře je její vzdálenost od první instrukce těla. První instrukce procedury má tedy index 0, další 1 atd.</li>
</ul>
<h3 id="inicializace">Inicializace</h3>
<p>Pokud je syntaktická analýza vstupních souborů úspěšná, konstruktor připraví interpret tak, aby byl od začátku ve stavu <em>běžící</em> a aktuální procedurou byla <code>MAIN</code>.</p>
<h3 id="krok-výpočtu">Krok výpočtu</h3>
<ol type="a">
<li>Pokud v <em>aktuální proceduře</em> existuje alespoň jedna nezpracovaná instrukce, pak se zpracuje první z nich.</li>
</ol>
<ul>
<li>Instrukce volání pozastaví vykonávání <em>aktuální procedury</em> a připraví vykonávání <em>volané procedury</em>.
<ul>
<li>Instrukce <em>volané procedury</em> se začnou vykonávat až v dalším kroku.</li>
<li>Interpret si zapamatuje stav <em>volající procedury</em> tak, aby mohl v jejím vykonávání pokračovat, když <em>volaná procedura</em> skončí.</li>
<li>Počet zapamatovaných procedur není explicitně omezen.</li>
</ul></li>
<li><code>SKIP</code> neudělá nic.</li>
<li><code>BREAK</code> zruší vykonávání <em>aktuální procedury</em>.
<ul>
<li>Interpret bude pokračovat ve vykonávání <em>volající procedury</em> za místem, kde došlo k volání právě zrušené procedury.</li>
<li>Pokud žádná <em>volající procedura</em> není, interpret přejde do stavu <em>zastaven</em>.</li>
</ul></li>
<li><code>HALT</code> zruší vykonávání programu a interpret přejde do stavu <em>zastaven</em>.</li>
<li><code>MOVE</code> posune robota o jedno políčko ve směru, kterým se dívá.
<ul>
<li>Pokud robot vrazí do stěny nebo se ocitne na <em>okraji světa</em>, interpret zavolá <code>Complain::runtimeError(procedure, index, ...)</code> pro aktuální proceduru a index této instrukce.</li>
</ul></li>
<li><code>LEFT</code> otočí robota doleva o 90 stupňů.</li>
<li><code>RIGHT</code> otočí robota doprava o 90 stupňů.</li>
<li><code>PICKUP</code> sníží počet značek na políčku, kde robot právě stojí.
<ul>
<li>Pokud se instrukce zavolá na políčku bez značek, interpret zavolá <code>Complain::runtimeError(procedure, index, ...)</code> pro aktuální proceduru a index této instrukce.</li>
</ul></li>
<li><code>PUTDOWN</code> zvýší počet značek na políčku, kde robot právě stojí.</li>
<li><code>IFWALL</code> otestuje, zda se před robotem ve směru, kterým se dívá, nachází stěna nebo <em>okraj světa</em>. Pokud ano, pak vykoná instrukci ve svém parametru, jinak neudělá nic.</li>
<li><code>IFMARK</code> otestuje, zda robot stojí na políčku s alespoň jednou značkou. Pokud ano, pak vykoná instrukci ve svém parametru, jinak neudělá nic.</li>
<li><code>ELSE</code> vykoná instrukci ve svém parametru, pokud předchozí test <code>IFMARK</code> nebol <code>IFWALL</code> v té samé proceduře byl negativní. Jinak neudělá nic.</li>
</ul>
<ol start="2" type="a">
<li>Jinak interpret ukončí volání <em>aktuální procedury</em>. Tuto operaci si lze představit tak, že před každým <code>END</code> v definici procedury se nachází skrytá instrukce <code>BREAK</code>.</li>
</ol>
<p>Při implementaci si dejte pozor na rekurzi. Každá procedura může volat sebe sama klidně několikrát. Interpret musí být schopný vrátit se po skončení volání na správné místo <em>volající procedury</em>.</p>
<h3 id="rozhraní">Rozhraní</h3>
<p>Pro komunikování s instancí interpretu implementujte zmíněné metody z požadovaného rozhraní (<code>interface.hpp</code>):</p>
<ul>
<li><code>const Robot&amp; robot() const</code> vrátí konstantní referenci na instanci robota.
<ul>
<li><code>Robot::position()</code> vrací aktuální pozici robota.</li>
<li><code>Robot::direction()</code> vrací aktuální směr robota.</li>
</ul></li>
<li><code>const World&amp; world() const</code> vrátí konstantní referenci na svět.
<ul>
<li><code>World::width()</code> a <code>World::height()</code> vrací rozměry světa.</li>
<li><code>World::tile(position)</code> vrací informace o políčku na pozici <code>position</code>. Pokud <code>position</code> není platná pozice ve světe, není návratová hodnota metody definována.</li>
</ul></li>
<li><code>bool hasProcedure(name) const</code> vrátí <code>true</code> právě tehdy, když v programu existuje procedura s názvem <code>name</code>.</li>
<li><code>bool running() const</code> vrátí <code>true</code>, pokud je interpret ve stavu <em>běžící</em>.</li>
<li><code>bool step()</code> vykoná jeden krok výpočtu, pokud je interpret ve stavu <em>běžící</em>. Jinak neudělá nic. Vráti <code>true</code>, právě když je na konci volání interpret ve stavu <em>běžící</em>.</li>
<li><code>unsigned run()</code> spustí vykonávání programu do úplného zastavení. Vrátí počet kroků, které interpret vykonal. Pokud se zavolá na <em>zastaveném</em> interpretu, vrátí <code>0u</code> a neudělá nic.</li>
</ul>
<p>Pokud dojde během vykonávání programu k volání některé metody třídy <code>Complain</code>, není stav interpretu definován (nemusíte nijak speciálně ošetřovat).</p>
<h2 id="poznámky-a-upřesnění">Poznámky a upřesnění</h2>
<ul>
<li><strong>Neoptimalizujte</strong> programy pro robota. Zadání to sice nezakazuje, ale pokud optimalizaci uděláte špatně, nemusí řešení projít testy.</li>
<li>Můžete předpokládat, že pokud z konstruktoru třídy <code>Interpret</code> zavoláte některou metodu <code>Complain</code>, instance se nevytvoří.</li>
<li>Můžete předpokládat, že pokud ze <code>step()</code> nebo <code>run()</code> zavoláte některou metodu <code>Complain</code>, nebude se volat žádná další metoda instance interpretu.</li>
<li>Pokud vstupní soubory obsahují více než jednu chybu, ohlaste <strong>první</strong> nalezenou. Pokud není ze zadání pořadí chyb jasné, pak je jedno, kterou z nich vyberete. Testy ověřují pouze takové chyby, kde je pořadí dané jednoznačně.</li>
</ul>
<h2 id="bonus-nepřímá-rekurze-2b">Bonus: Nepřímá rekurze (2b)</h2>
<p>V části <strong>Syntax programu</strong> stojí:</p>
<ul>
<li><strong>Volání procedury</strong> je název již definované procedury nebo té, která se právě definuje, bez parametrů.</li>
</ul>
<p>Tato definice umožňuje zapsat programy s přímou rekurzí a taky dovoluje jednodušší sémantickou analýzu, kterou je možné vykonávat zároveň se čtením kódu.</p>
<p><em>Nepřímá</em> (nebo <em>vzájemná</em>) <em>rekurze</em> je posloupnost různých procedur <em><code>P0</code></em>, …, <em><code>Pn</code></em> pro <em>n ≥ 1</em> taková, že</p>
<ul>
<li><em><code>Pi</code></em> volá <em><code>P(i+1)</code></em> pro každé <em>0 ≤ i &lt; n</em>,</li>
<li><em><code>Pn</code></em> volá <em><code>P0</code></em>.</li>
</ul>
<p>Základní zadání však neumožňuje takovou rekurzi zapsat, protože v některé části programu by muselo existovat volání procedury definované <em>později</em> v kódu. Jako bonusový úkol proto implementujte interpret s upravenou definicí volání:</p>
<ul>
<li><strong>Volání procedury</strong> je název libovolné procedury definované ve zdrojovém kódu, bez parametrů.</li>
</ul>
<p>Zkuste se nad bonusem zamyslet již před programováním úkolu. Pokud své řešení navrhnete pouze pro základní zadání, nemusí být pak lehké bonus implementovat dodatečně.</p>
<h2 id="podrobnější-ukázka-fungování-interpretu">Podrobnější ukázka fungování interpretu</h2>
<p>Mějme následující svět:</p>
<pre><code>3 1
0 0 n
000</code></pre>
<p>a k němu program:</p>
<pre><code>DEFINE ADVANCE
    IFWALL BREAK
    ELSE   MOVE
    ADVANCE
END

DEFINE MAIN
    RIGHT
    ADVANCE
    PICKUP
END</code></pre>
<p>Instanci struktury <code>Position</code> budeme pro zjednodušení označovat <code>[x,y]</code>. Po vytvoření instance třídy <code>Interpret</code>, kterou nazveme <code>interpret</code>, platí:</p>
<ul>
<li><code>interpret.running()</code> vrátí <code>true</code>.</li>
<li><code>interpret.robot().position()</code> vrátí <code>[0,0]</code>.</li>
<li><code>interpret.robot().direction()</code> vrátí <code>Direction::North</code>.</li>
<li><code>interpret.world().width()</code> vrátí <code>3u</code>.</li>
<li><code>interpret.world().height()</code> vrátí <code>1u</code>.</li>
<li><code>interpret.world().tile(p)</code>, pro každé <code>p</code> z <code>[0,0]</code>, <code>[1,0]</code> a <code>[2,0]</code>, vrátí <code>Tile t</code> takové, že
<ul>
<li><code>t.wall</code> je <code>false</code></li>
<li><code>t.marks</code> je <code>0u</code></li>
</ul></li>
</ul>
<p>Aktuální procedurou je <code>MAIN</code>, žádná instrukce se zatím nezpracovala. Budeme postupně volat <code>interpret.step()</code>. Zápisem <code>X(N)</code> značíme, že aktuální procedurou je <code>X</code> se zanořením <code>N</code>, které budeme číslovat od 0 (zanoření se netestuje, slouží pouze pro ilustraci).</p>
<ol type="1">
<li><code>MAIN(0)</code>: interpret zpracuje instrukci <code>RIGHT</code>. Robot se natočí na <code>Direction::East</code>.</li>
<li><code>MAIN(0)</code>: interpret zpracuje volání <code>ADVANCE</code>. Zapamatuje si pozici v <code>MAIN</code> a aktuální procedurou se stává <code>ADVANCE</code>.</li>
<li><code>ADVANCE(1)</code>: interpret zpracuje instrukci <code>IFWALL</code>. Test je negativní, před robotem na pozici <code>[1,0]</code> se stěna nenachází.</li>
<li><code>ADVANCE(1)</code>: interpret zpracuje instrukci <code>ELSE</code>. Předchozí test <code>IFWALL</code> byl negativní, proto interpret zpracuje i instrukci <code>MOVE</code>. Robot se posune na pozici <code>[1,0]</code>.</li>
<li><code>ADVANCE(1)</code>: interpret zpracuje (rekurzivní) volání <code>ADVANCE</code>. Zapamatuje si pozici v <code>ADVANCE(1)</code> a aktuální procedurou se stává nová instance <code>ADVANCE(2)</code>.</li>
<li><code>ADVANCE(2)</code>: <code>IFWALL</code>, stejné jako krok 3.</li>
<li><code>ADVANCE(2)</code>: <code>ELSE</code>, stejné jako krok 4, robot se posune na pozici <code>[2,0]</code>.</li>
<li><code>ADVANCE(2)</code>: <code>ADVANCE</code>, stejné jako krok 5, rekurzivně se volá další instance <code>ADVANCE(3)</code>.</li>
<li><code>ADVANCE(3)</code>: interpret zpracuje instrukci <code>IFWALL</code>. Protože se robot dívá na pozici <code>[3,0]</code>, která tvoří <em>okraj světa</em>, je test pozitivní a vykoná se instrukce <code>BREAK</code>. Procedura <code>ADVANCE(3)</code> se zruší a program bude v dalším kroku pokračovat v její <em>volající proceduře</em> <code>ADVANCE(2)</code>.</li>
<li><code>ADVANCE(2)</code>: všechny instrukce byly zpracovány, volání procedury končí.</li>
<li><code>ADVANCE(1)</code>: všechny instrukce byly zpracovány, volání procedury končí.</li>
<li><code>MAIN(0)</code>: interpret zpracuje instrukci <code>PICKUP</code>. Na pozici robota <code>[2,0]</code> se nenachází žádná značka, interpret proto zavolá <code>Complain::runtimeError(&quot;MAIN&quot;, 2u, ...)</code>.</li>
</ol>
<p>V krocích 1 až 11 vrátí <code>step()</code> vždycky <code>true</code>. Po kroku <code>12</code> nejsou stav interpretu a tudíž ani návratová hodnota volání <code>step()</code> definovány.</p>
<p>Pokud bychom instrukci <code>PICKUP</code> nahradili <code>PUTDOWN</code>, pak kroky 1 až 11 budou vypadat stejně, změna nastane od kroku 12:</p>
<ol start="12" type="1">
<li><code>MAIN(0)</code>: interpret zpracuje instrukci <code>PUTDOWN</code>. Počet značek na pozici <code>[2,0]</code> se změní na <code>1u</code>.</li>
<li><code>MAIN(0)</code>: všechny instrukce byly zpracovány, volání procedury končí. Žádná zapamatovaná procedura již neexistuje, interpret proto přejde do stavu <em>zastaven</em>.</li>
</ol>
<p>V kroku 12 vrátí <code>step()</code> <code>true</code>, v kroku 13 vrátí <code>false</code>. Pokud bychom místo opakovaného volání <code>step()</code> zavolali přímo <code>run()</code>, vrátí <code>13u</code>.</p>
<p>Po skončení programu navíc platí:</p>
<ul>
<li><code>interpret.running()</code> vrátí <code>false</code></li>
<li><code>interpret.robot().position()</code> vrátí <code>[2,0]</code></li>
<li><code>interpret.robot().direction()</code> vrátí <code>Direction::East</code></li>
<li><code>interpret.world().tile(p).marks</code> pro pozici <code>[2,0]</code> vrátí <code>1u</code></li>
</ul>
<h2 id="co-se-testuje">Co se testuje</h2>
<p><strong>Všechno</strong>. Předpokládejte, že každá věta v zadání, která je testovatelná, má test. Pro přehled, co můžete očekávat, uvádíme přehled testů:</p>
<ol type="1">
<li>11 testů parsování světa. Zde se testuje, zda konstruktor sestaví svět podle zadání.</li>
<li>13 testů na chyby světa. Zde se ověří, že interpret odhalí všechny chyby ve zdrojovém kódu světa a ohlásí je na správném řádku.</li>
<li>12 testů parsování programu. Protože zadání nespecifikuje způsob uložení kódu, zde se pouze ověří, že konstruktor nezavolá žádnou metodu <code>Complain</code> a má správné procedury.</li>
<li>9 testů syntaktických chyb programu.</li>
<li>3 testy sémantických chyb.</li>
<li>20 testů na chování instrukcí. Interpretu se předhodí jednoduché (ne nutně <em>triviální</em>) světy a programy a testuje se, zda mají programy žádaný efekt.</li>
<li>6 testů na složitější programy, které mají mnoho procedur, zanoření nebo velký počet kroků.</li>
<li>2 testy na bonus.</li>
</ol>
<ul>
<li>Sady 1 až 5 volají pouze konstruktor třídy <code>Interpret</code> a dotazovací metody (<code>robot()</code>, <code>world()</code>, <code>hasProcedure()</code>, <code>running()</code>).</li>
<li><p>V sadách 3, 4 a 5 se používá <em>triviální svět</em>, který vypadá přesně takhle:</p>
<pre><code>1 1
0 0 n</code></pre>
(na třetím řádku je jedna mezera <code></code> a <code>\n</code>, kterou Markdown ignoruje).</li>
<li><p>V sadách 1 a 2 se používá <em>triviální program</em>, který vypadá přesně takhle:</p>
<pre><code>DEFINE MAIN
END</code></pre></li>
</ul>
<h2 id="kostra-úkolu">Kostra úkolu</h2>
<p>Kostra úkolu obsahuje <code>CMakeLists.txt</code> s následujícími cíli:</p>
<ul>
<li><code>robot</code> je knihovna, do které se sestaví všechny vaše soubory kromě těch, které jsou zakázany v zadání.</li>
<li><code>robot-cli</code> je spustitelný soubor, do kterého se přidá <code>main.cpp</code>.</li>
<li><code>robot-tests</code> je spustitelný soubor vytvořený z vašich testů.</li>
</ul>
<p>Kostra obsahuje iniciálně soubor <code>interface.cpp</code>, aby si CMake nestěžoval. Tento soubor nemusíte v řešení ponechat, dejte si však pozor, aby vždy existoval alespoň jeden <code>*.cpp</code> soubor, jinak CMake skončí s chybou</p>
<pre><code>There are no source files to build Robot Library!</code></pre>
<p>Pokud vytvoříte nový soubor, <em>nemusíte</em> upravovat <code>CMakeLists.txt</code>, pouze znovu zavolejte <code>cmake</code>.</p>
<p>Kostra úkolu obsahuje hlavičkový soubor <code>interface.hpp</code> s definicí požadovaného rozhraní tříd. <strong>Nemusíte</strong> tento soubor zachovat. Pokud chcete, můžete třídy definovat v jiných souborech a <code>interface.hpp</code> smazat.</p>
<p>Navíc v <code>test-example.cpp</code> naleznete ukázkové testy, ke kterým <strong>silně doporučujeme</strong> napsat si další vlastní testy. Pro větší pohodlí <code>tests-utils.hpp</code> definuje pomocná makra, která vám mohou testování usnadnit.</p>
<h2 id="vzorové-řešení">Vzorové řešení</h2>
<p>Na Aise v <code>/home/kontr/pb161-hw02/robot</code> naleznete vzorovou implementaci. Obsahuje konzolové grafické rozhraní, se kterým můžete testovat vlastní programy pro robota a jeho chování krok po kroku.</p>
<p>Se správnými přepínači vypisuje užitečné informace, ze kterých můžete hádat, jak si vzorové řešení ukládá kód. V <code>/home/kontr/pb161-hw02/</code> taky naleznete některé vzorové vstupní soubory.</p>
<p>Pro více informací spusťte program s přepínačem <code>--help</code>.</p>
<h2 id="odkazy">Odkazy</h2>
<ul>
<li><a href="https://en.wikipedia.org/wiki/Compiler">Překladač (Wikipedie)</a></li>
<li><a href="https://en.wikipedia.org/wiki/Interpreter_(computing)">Interpret (Wikipedie)</a></li>
<li><a href="https://en.wikipedia.org/wiki/Recursive_descent_parser">Analýza rekuzivním sestupem (Wikipedie)</a></li>
</ul>
<p>Zajímá vás téma překladačů? Pak vás mohou zajímat i tyto předměty na FI:</p>
<ul>
<li><a href="https://is.muni.cz/auth/predmet/fi/IB102"><code>IB102</code> Automaty a gramatiky</a></li>
<li><a href="https://is.muni.cz/auth/predmet/fi/IB005"><code>IB005</code> Formální jazyky a automaty</a></li>
<li><a href="https://is.muni.cz/auth/predmet/fi/IA006"><code>IA006</code> Vybrané kapitoly z teorie automatů</a></li>
<li><a href="https://is.muni.cz/auth/predmet/fi/PA008"><code>PA008</code> Překladače</a></li>
<li><a href="https://is.muni.cz/auth/predmet/fi/PA037"><code>PA037</code> Projekt z překladačů</a></li>
</ul>
<h2 id="nápověda">Nápověda</h2>
<p><strong>SPOILER ALERT!</strong></p>
<p>Pokud nemáte ani tušení, jak své řešení navrhnout, uvádíme přehled souborů a tříd, které má vzorové řešení pro inspiraci. Vzorové řešení včetně bonusu má celkem 635 řádků bez komentářů a prázdných řádků.</p>
<ul>
<li><code>interpret.hpp</code> (73 ř.), <code>interpret.cpp</code> (133 ř.)
<ul>
<li><code>Flags</code></li>
<li><code>CallFrame</code> - reprezentace volání procedur</li>
<li><code>Interpret</code> - požadováno zadáním</li>
</ul></li>
<li><code>parsers.hpp</code> (36 ř.), <code>parsers.cpp</code> (187 ř.)
<ul>
<li><code>WorldParser</code></li>
<li><code>ProgramParser</code></li>
</ul></li>
<li><code>program.hpp</code> (86 ř.), <code>program.cpp</code> (43 ř.)
<ul>
<li><code>OpCode</code></li>
<li><code>Instruction</code></li>
<li><code>InstructionDecoder</code></li>
<li><code>Procedure</code></li>
<li><code>Program</code></li>
</ul></li>
<li><code>world.hpp</code> (43 ř.), <code>world.cpp</code> (34 ř.)
<ul>
<li><code>Robot</code> - požadováno zadáním</li>
<li><code>World</code> - požadováno zadáním</li>
</ul></li>
</ul>
