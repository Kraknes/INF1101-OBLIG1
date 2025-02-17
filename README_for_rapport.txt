Spørsmål:

1: "The average complexity for search should be O(log n). 
   You should prove that your implementation fulfils this in your report through experiments/benchmarks."
   - Hashmap er O(1) utenom flere instanser av objekter, da må en lenketliste som må itereres. Er dette OK?
   - Hvordan gjør man eksperimenter/benchmarks på dette?
   --> Lag en variabel i map strukturen for antall iterasjoner å søke etter word
   --> Lag en for loop for å gå gjennom antall ord (la oss si 1000 ord)
   --> Ta 1000 / på antall iterasjoner for å finne ord
   --> Dette vil være benchmark for O. Forhåpentligvis er det O(log n)

   --> Liste på 5000
   --> iterato
   --> Fra testmap er det 12345 ord 
   --> Hashmap har iterert 12359 ganger for å finne ord med array størrelse 100 000. 
   --> 25000 ord
   --> iterasjon på 25152
   --> test på 50000 ord
   --> iterator på 51698 ganger
   --> 51698 / 50000 = 1,03396 => O(1,03396) ~ O(1)
   --> Log n = log(2) 50000 = 15,6096 

2. "Argue for what trade-offs your application makes with regards to space and time complexity"
   - Hashmaps tar kort tid med search, men tar mye plass. Stemmer?
   --> Stemmer, må allokere mye plass til hashtable/array
   --> Har nå 100 000, må allokere minne til 100 000 noder.
   --> effektiviteten er veldig lav
   --> kan innføre reallokere minne til array for hver gang, men reallokering 

3. Måtte endre på main.c for å få hashmap til å funke. Det er OK, må bare diskuteres i rapporten. 

