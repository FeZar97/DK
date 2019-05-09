#ifndef ASKR_H
#define ASKR_H

#include <definitions.h>
#include <rpu.h>
#include <sdr.h>
#include <dsp.h>

/*
    Автоматическая система контроля работоспособности (АСКР) обеспечивает диагностику технического состояния РПУ при старте программы и во время работы.
    Процедура тестирования проводится в 5 этапов:
        «Тест калибратора»,
        «Тест чувствительности»,
        «Тест аттенюатора преселектора»,
        «Тест аттенюатора ВЧ»,
        «Тест аттенюатора ПЧ».
    Измерения проводятся по каждому из каналов РПУ. */

class ASKR
{
    QString testReport;
    RPU     *rpu;
    SDR     *sdr;
    DSP     *dsp;

public:
    ASKR(RPU *_rpu, SDR *_sdr, DSP *_dsp);

    void kalibratorTest();
    void tractTest();
};

#endif // ASKR_H
