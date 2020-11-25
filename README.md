# cmusphinx_liepa_server
Automated recognition server based on cmu sphinx and liepa models. This demonstrate how can be exernalized recogntion from single board computers(or other underpower programable devices) to local network or internet dedicate resouces.

#How it works

This servier implemntation uses plain sockets to accept stream of raw audio data. The steam is processed by CMU pocketsphinx with Liepa project acoustic models. Oversimplified version of grammar is defined [tikrinimo.gram](https://github.com/liepa-project/cmusphinx_liepa_server/blob/main/model/LiepaASRResources/tikrinimo.gram) and dictionary [tikrinimo.dict](https://github.com/liepa-project/cmusphinx_liepa_server/blob/main/model/LiepaASRResources/tikrinimo.dict)

In order to create your own dictionary go to [liepa_automatinis_fonemizavimas](https://github.com/liepa-project/liepa_automatinis_fonemizavimas). How to work with gramar read in documentation [JSpeech Grammar Format](https://www.w3.org/TR/2000/NOTE-jsgf-20000605/) or here [cmusphinx language model](https://cmusphinx.github.io/wiki/tutoriallm/)

# Build

Clone it with cmu sphinx code:
```
git clone --recurse-submodules git@github.com:liepa-project/cmusphinx_liepa_server.git
```

Create make files:
```
(mkdir build; cd build ; cmake ..)
```

Configure sphinx for instalation
```
(cd ./cmusphinx/sphinxbase; ./autogen.sh; make)
(cd ./cmusphinx/pocketsphinx; ./autogen.sh; make)

```


Compile server:
```
./cmusphinx/pocketsphinx/autogen.sh
```

#Use it


Run server:
```
./build/cmusphinx_liepa_server
```

Testing server with bash onliner:
```
arecord  -c 1 -f S16_LE -r 16000 -t raw -q | nc -N localhost 5050
```


