#ifndef IQCOMPENSATOR_H
#define IQCOMPENSATOR_H

#include <definitions.h>

class IQCompensator : public QObject
{
    Q_OBJECT

public:
    DSP_params *dsp_params;

    IQCompensator(DSP_params *new_dsp_params = nullptr);
    ~IQCompensator();

    Ipp32f tetta1;
    Ipp32f tetta2;
    Ipp32f tetta3;

    Ipp32f c1;
    Ipp32f c2;

    Ipp32f g;
    Ipp32f fi;

public slots:
    void get_compensate_step(Ipp32fc *cell);
};

#endif // IQCOMPENSATOR_H
