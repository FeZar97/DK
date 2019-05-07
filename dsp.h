#ifndef DSP_H
#define DSP_H

#include <definitions.h>

#include <filter.h>

#include <sdr.h>
#include <reader.h>
#include <iqcompensator.h>
#include <filter.h>
#include <fft_calcer.h>
#include <shifter.h>
#include <wav_recorder.h>
#include <demodulator.h>
#include <sounder.h>

class DSP : public QObject
{
    Q_OBJECT

public:
    SDR          *sdr;
    DSP_params   *dsp_params;
    READER       *reader;
    IQCompensator *iqcompensator;
    FILTER       *flt;
    FFT_CALCER   *fft;
    SHIFTER      *shifter;
    DEMODULATOR  *demod;
    SOUNDER      *sounder;

    WAV_RECORDER *reader_rec;
    WAV_RECORDER *flt_rec;
    WAV_RECORDER *shift_rec;
    WAV_RECORDER *sound_rec;

    QThread      reader_thread;
    QThread      iqcompensator_thread;
    QThread      filtration_thread;
    QThread      fft_thread;
    QThread      fft_shift_thread;
    QThread      demod_thread;
    QThread      sound_thread;
    QThread      wav_thread;

    DSP(SDR *new_sdr = nullptr);
    ~DSP();

public slots:
    void                  bind_objects();

    int                   prepair_to_record();              // КРУТАЯ ВЕЩЬ
    int                   recalc_dsp_params();              // пересчет параметров ЦОС
    void                  prepair_memory();

    void                  create_file_names();
    void                  close_files();

    void                  setDynRng(float _dnRng);

signals:
    void                  global_update_interface();
};

#endif // DSP_H
