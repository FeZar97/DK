#ifndef SOUNDER_H
#define SOUNDER_H

#include <definitions.h>

class SOUNDER : public QObject
{
    Q_OBJECT
public:

    SDR_params *sdr_params;
    DSP_params *dsp_params;

    SOUNDER(SDR_params *new_sdr_params = nullptr, DSP_params *new_dsp_params = nullptr);
    ~SOUNDER();

public slots:
    void get_sound_step(Ipp32f *cell = nullptr);

signals:
    void write_to_file16s(Ipp16s *cell, int cell_size);
};

#endif // SOUNDER_H
