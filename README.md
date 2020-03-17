# TunerPlug
General Purpose Tuner Plugin based on the YIN algorithm using a modified example project from Oli Larkin's WDL-OL framework:
https://github.com/olilarkin/wdl-ol

which can be compiled as VST2, VST3 and AU Plugin or macOS/Win32 standalone

## Dependencies

The GIST audio analysis library by Adam Stark:
https://github.com/adamstark/Gist

Currently the project is configured to us KISSfft:
https://github.com/mborgerding/kissfft

but can be made more performant using FFTW3.

## Build

Configure wdl-ol according to:
http://www.martin-finke.de/blog/articles/audio-plugins-002-setting-up-wdl-ol/
