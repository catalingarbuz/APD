Garbuz Catalin 336CC Tema 2 APD
Am realizat tema, urmarind indicatiile din enuntul acesteia.

In main, dupa ce salvez parametrii primiti la rularea programului, salvez
intr-un array de string-uri fisierele pe care urmeaza sa aplic paradigma 
map-reduce. Dupa aceasta, iterez prin lista cu fisierele, si pentru fiecare
fisier in parte realizez operatiile de map si operatiile de reduce, in urma
celei din urma, salvez rezultatele obtinute in array-ul static results. 

Array-ul results il folosesc pentru a sorta fisierele descendent dupa rank si
pentru a face printarea in fisierul out.

In for-ul in care iterez printre fisiere, imi creez un dictionar de tipul
ConcurrentHashMap si un array in care salvez cuvintele de lungime maxima de la 
fiecare fragment procesat. Imi creez un ExecutorService pentru operatia map,
primul task trimis, fiind cu variabila de tip RandomAccessFile, offset-ul(care
initial este 0), dimensiunea fragmentului si acel executor service creat 
anterior. Astept finisarea operatiei de map apeland functia awaitTermination,
cu o valoare de 200 ms, (valoare care poate fi modificata pentru fisiere mai 
mari decat cele prezente in teste). Dupa finisarea operatiei de map, creez 
task-ul de reduce, cu fisierul curent, dictionarul si array-ul de cuvinte 
maximale, calculate anterior la operatia de map. Dupa care la fel cu 
awaitTermination astept finalizarea executiei operatiei de reduce.

Map
Am implementat operatia de map in clasa MyMapTask.java. Aici la fiecare task,
pun pointerul de unde urmeaza sa citesc fragmentul de date la offset-ul curent
cu file.seek(). Dupa care citesc datele, actualizez offset-ul, si ajustez 
fragmentul si offset-ul daca fragmentul de date se termina in mijlocul unui
cuvant. Dupa ce fac acestea, iterez prin string-ul de cuvinte pe care il 
obtin apeland functia split() cu regex-ul"\\W+" si salvez cuvintele in 
dictionar si in array-ul de cuvinte cu lungime maxima dupa conditiile temei.
Si intr-un final, daca nu am ajuns la finalul fisierului mai creez un task
nou care urmeaza sa fie executat in cadrul aceluiasi executorService.

Reduce
Am implementat operatia de reduce in clasa MyReduceTask.java. Aici mai intai
calculez lungimea maxima si numarul de cuvinte cu acea lungime, iterand prin
array-ul de cuvinte cu lungime maximala. Dupa care, conform formulei din enunt
calculez rank-ul fisierului si la sfarsit adaug campurile care urmeaza sa fie 
printate in output, in array-ul static results.

