# cmusphinx_liepa_server
Automated recognition server based on cmu sphinx and liepa models 



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


Run server:
```
./build/cmusphinx_liepa_server
```

Testing server with bash onliner:
```
arecord  -c 1 -f S16_LE -r 16000 -t raw -q | nc -N localhost 5050
```


