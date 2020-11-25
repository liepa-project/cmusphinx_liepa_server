#include "TcpServer.h"
#include <pocketsphinx.h>	

int main(int argc, char* argv[]){
   int read_timeout = 3;
   int port_num = 5050;
   
   printf("server warming up...\n"); 

   ps_decoder_t *decoder;
   cmd_ln_t *config;
   char const *hyp;
   int32 score;
   config = cmd_ln_init(NULL, ps_args(), TRUE,
			     "-hmm",  "./model/LiepaASRResources/G20150114_EZ15_FZ1.3semi750-sph20141812",
			     "-jsgf",  "./model/LiepaASRResources/tikrinimo.gram",
			     "-dict",  "./model/LiepaASRResources/tikrinimo.dict",
			   //   "-logfn", "/dev/null",
			     NULL);
	if (config == NULL){
		return 1;
	}
	decoder = ps_init(config);
	if (decoder == NULL){
		return 1;
	}

   TcpServer server(read_timeout); 
   bool eos = false;
   double chunk_length_secs = 0.18;
   double samp_freq = 16000.0;
   size_t chunk_len = static_cast<size_t>(chunk_length_secs * samp_freq);
   server.Listen(port_num);
   while (true) {
      server.Accept();
      ps_start_utt(decoder);
      bool isCurrentInSpeech = false;
      bool isUttStarted = false;
      while (true) {
         eos = !server.ReadChunk(chunk_len);
         vector<int16> audioBuffer = server.GetChunk();
         int16_t* bufferArr = &audioBuffer[0];
         ps_process_raw(decoder, bufferArr, audioBuffer.size(), false, false);
         isCurrentInSpeech = ps_get_in_speech(decoder);
         //VAD detected speech, but utterance not started 
         if (isCurrentInSpeech && !isUttStarted) {
            isUttStarted = TRUE;
            printf("\nListening...\n");
         //VAD detected silence after speech and utterance already started
         }else if (!isCurrentInSpeech && isUttStarted) {
            ps_end_utt(decoder);
            hyp = ps_get_hyp(decoder, &score );
            if (hyp != NULL) {
                printf("Final: %s\n", hyp);
                server.Write(hyp);
                server.Write("\n");
                fflush(stdout);
            }else{
               printf("\nFinal: Nothing found\n");
               server.Write("***\n");
            }
            isUttStarted = false;
            eos=true;
         }

         if (eos) {
            printf("\nbreak???");
            break;
         }
      }

      
      printf("\nDONE");
      server.Write("\n");
      server.Write("\n");
   }
   ps_free(decoder);
   return 0;
}
