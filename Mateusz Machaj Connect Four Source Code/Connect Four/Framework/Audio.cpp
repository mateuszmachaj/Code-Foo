#include "Audio.h"

bool Audio::init(HWND hwnd)
{
    HRESULT result;

    //create DirectSound manager object
    dsound = new CSoundManager();

    //initialize DirectSound
    result = dsound->Initialize(hwnd, DSSCL_PRIORITY);
    if (result != DS_OK)
        return false;

    //set the primary buffer format
    result = dsound->SetPrimaryBufferFormat(2, 22050, 16);
    if (result != DS_OK)
        return false;

    //return success
    return true;
}

CSound *Audio::LoadSound(char *filename)
{
    HRESULT result;

    //create local reference to wave data
    CSound *wave;

    //attempt to load the wave file
    result = dsound->Create(&wave, filename);
    if (result != DS_OK)
        return NULL;

    //return the wave
    return wave;
}

void Audio::PlaySound(CSound *sound)
{
    sound->Play();
}

void Audio::LoopSound(CSound *sound)
{
    sound->Play(0, DSBPLAY_LOOPING);
}

void Audio::StopSound(CSound *sound)
{
    sound->Stop();
}
