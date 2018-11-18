#ifndef SPEAKER_H
#define SPEAKER_H


class speaker : public QObject
{
    Q_OBJECT
public:
    explicit speaker(QObject *parent = 0);
    ~speaker();

signals:

public slots:
};

#endif // SPEAKER_H
