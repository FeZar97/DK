#ifndef DEMODULATOR_H
#define DEMODULATOR_H

#include <definitions.h>

class DEMODULATOR : public QObject
{
    Q_OBJECT

public:
    SDR_params *sdr_params;
    DSP_params *dsp_params;

    DEMODULATOR(SDR_params *new_sdr_params = nullptr, DSP_params *new_dsp_params = nullptr);
    ~DEMODULATOR();

public slots:
    void get_demod_step(Ipp32fc *cell = nullptr);

signals:
    void get_sound_step(Ipp32f *cell);
};

#endif // DEMODULATOR_H
