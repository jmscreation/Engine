#include "engine.h"
#include "test/includes.h"

Engine::App* app = nullptr;

#ifndef SAMPLE_RATE
	#define SAMPLE_RATE 48000
#endif

int main(int argc,char** argv) {
    #ifndef APP_CLASS

        #error APP_CLASS must be defined

    #else
    	#ifdef USE_AUDIO
		Engine::AudioContext audioContext(SAMPLE_RATE);
		#endif

        Engine::App::setAppParams(argc,argv);

        APP_CLASS application;
        static_cast<Engine::App*>(&application)->run();

    #endif // APP_CLASS

    return 0;
}
