#ifndef __AX_AUDIO_WAVE_TABLE__
#define __AX_AUDIO_WAVE_TABLE__

#include "axAudioUtils.h"

#define axBUFFER_SIZE 2048

class axAudioBuffer;

namespace axAudio
{
    class WaveTable
    {
    public:
        WaveTable();
        WaveTable(axAudioBuffer* buffer);
        
        enum axWaveformType
        {
            axWAVE_TYPE_SINE,
            axWAVE_TYPE_TRIANGLE,
            axWAVE_TYPE_SQUARE,
            axWAVE_TYPE_SAW
        };
        
        void SetWaveformType(const axWaveformType& type);
        
        void ProcessSample(float* out);
        void ProcessSample(float* out, float* freq);
        void ProcessSample(double* out);
        void ProcessSample(double* out, double* freq);
        void ProcessBlock(float* out, unsigned long frameCount);
        
        void SetFreq(const double& freq);
        
        double GetFreq() const;
        
    private:
        
        double _freq = { 200.0 };
        //    float _data[axBUFFER_SIZE + 1];
        float* _data;
        double _cPhase;
        int _bufferSize;
        
        
        double WaveInterpole(const double& freq,
                             const unsigned int& len,
                             const double& phase);
    };
}

#endif // __AX_AUDIO_WAVE_TABLE__
