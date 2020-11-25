#!/usr/bin/env python3
# encoding: utf-8

#export PYTHONIOENCODING=utf8

'''
Created on MAR 14, 2020
@author: mindaugas greibus
'''

import time
import subprocess

#Send to core
import json
import random

import asyncio

import traceback
import sys
import datetime



class LiepaAsr(object):
    CHUNK = 4096
    CHANNELS = 1
    # FORMAT = pyaudio.paInt16
    RATE = 16000
    def __init__(self):
        '''
        Constructor
        '''
        print ("[__init__]+++")
        self.isProcessingRequested = False
        print ("[__init__]---")
        self.server_ip = "0.0.0.0"
        #self.server_ip = "192.168.1.203"
        #Indicate listening for next utterance
        print ("READY....")


    async def recognized_callback(self, text, is_final):
        print(u"recognized_callback:", text.encode("utf-8"))
        return is_final

    async def reader_read(self, reader, recognized_callback_):
        hyp_str = ""
        while(self.isProcessingRequested):
            in_char_bytes = await reader.read(1024)
            in_char_str = in_char_bytes.decode("utf-8")
            # print("in_char_str",in_char_str)
            for in_char in in_char_str:
                if(in_char == '\n'):
                    #skip false positive: it says recognized, but no text recorded yet
                    if(hyp_str == ""):
                        continue
                    self.isProcessingRequested=False
                    if(recognized_callback_):
                        # print("final hyp:", hyp_str)
                        await recognized_callback_(hyp_str, True)
                        hyp_str = ""
                elif(in_char == '\r'):
                    # print("initial hyp:", hyp_str)
                    await recognized_callback_(hyp_str, False)
                    hyp_str = ""
                else:
                    # hyp_char_arr.append(in_char)
                    hyp_str = hyp_str + in_char


    async def writer_write(self, writer):
        cmd = ['arecord', '-c', '1', '-f', 'S16_LE', '-r', '16000', '-t', 'raw', '-q', '-']
        #cmd = ['arecord', '-Dplughw:1,0', '-c', '1', '-f', 'S16_LE', '-r', '16000', '-t', 'raw', '-q', '-'] 
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE)

        while(self.isProcessingRequested):
            # sys.stdout.write(".")
            sys.stdout.flush()
            data = process.stdout.read(self.CHUNK)
            writer.write(data)
            await asyncio.sleep(0.01)
        process.stdout.close()
        writer.close()



    async def listen(self, id ,recognized_callback):
        print("Listening Started: ", id)
        loop = asyncio.get_event_loop()
        reader, writer = await asyncio.open_connection(self.server_ip, 5050,
                                                   loop=loop)

        self.isProcessingRequested = True

        await asyncio.gather(
            self.reader_read(reader, recognized_callback),
            self.writer_write(writer)
        )
        print("Listen Done", id)


if __name__ == "__main__":
    asr = LiepaAsr()
    # asyncio.run(asr.listen(1, asr.recognized_callback))
    loop = asyncio.get_event_loop()
    result = loop.run_until_complete(asr.listen(1, asr.recognized_callback))
