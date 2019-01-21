/********************************************************************************
** Form generated from reading UI file 'config_manager.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_MANAGER_H
#define UI_CONFIG_MANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_config_manager
{
public:
    QWidget *InterfaceWidget;
    QTabWidget *ConfigTab;
    QWidget *RPUTab;
    QComboBox *LPTPortCB;
    QLabel *LPTPortL;
    QLabel *ConfigLabel;
    QComboBox *TractConfigCB;
    QTabWidget *RPUTabs;
    QWidget *Tract1Tab;
    QComboBox *Tract1_AttInBox;
    QLabel *Tract1_AttHFLabel;
    QComboBox *Tract1_AttIFBox;
    QComboBox *Tract1_AttHFBox;
    QLabel *Tract1_kHzLabel;
    QComboBox *Tract1_BandIFBox;
    QLabel *Tract1_AttIFLabel;
    QLabel *Tract1_AttInLabel;
    QLabel *Tract1_BandIFLabel;
    QSpinBox *Tract1_CentralFreqSB;
    QLabel *Tract1_CentralFreqLabel;
    QWidget *Tract2Tab;
    QWidget *Tract3Tab;
    QWidget *Tract4Tab;
    QWidget *KalibratorTab;
    QLabel *KalibKHZLabel;
    QComboBox *KalibExitTypeBox;
    QSpinBox *KalibModulationFrequencyBox;
    QLabel *KalibExitTypeLabel;
    QLabel *KalibAttenLabel;
    QLabel *KalibSignalTypeLabel;
    QComboBox *KalibAttenTypeBox;
    QLabel *KalibModulationFrequencyLabel;
    QLabel *KalibStatusLabel;
    QComboBox *KalibStatusBox;
    QComboBox *KalibSignalTypeBox;
    QFrame *RPUMainTopLine;
    QWidget *SDRTab;
    QPushButton *RefreshButton;
    QLabel *CurrentDevNumLabel;
    QSpinBox *CurrentDevSpinBox;
    QPushButton *ConfirmButton;
    QLabel *SampleRateLabel;
    QSpinBox *SampleRateSB;
    QLabel *GainLabel;
    QComboBox *GainValBox;
    QComboBox *DirectSamplingCB;
    QLabel *DirectSamplingLabel;
    QSpinBox *CentralFreqSB;
    QLabel *CentralFreqLabel;
    QLabel *HzLabel_2;
    QWidget *DSPTab;
    QTabWidget *DSPWidget;
    QWidget *RecordTab;
    QPushButton *CurrentPathButton;
    QCheckBox *FltSignalCB;
    QLabel *RecordTimeLabel;
    QCheckBox *RealSignalCB;
    QLabel *ADCSignalLabel;
    QComboBox *RecordTimeCB;
    QLabel *RealSignalLabel;
    QLabel *FltSignalLabel;
    QCheckBox *ADCSignalCB;
    QPushButton *RecordButton;
    QProgressBar *ADCBar;
    QProgressBar *FltBar;
    QProgressBar *RealBar;
    QWidget *FiltrationTab;
    QCheckBox *FiltrationCB;
    QSpinBox *PassbandValSB;
    QLabel *PassbandLabel;
    QLabel *UseFltLabel;
    QLabel *BoombandLabel;
    QLabel *HzLabel1;
    QSpinBox *BoombandValSB;
    QLabel *HzLabel2;
    QLabel *OutSampleFreqLabel;
    QComboBox *OutSampleFreqCB;
    QLabel *HzLabel3;
    QWidget *FftTab;
    QCheckBox *InversionCB;
    QLabel *FftModeLabel;
    QLabel *InfoLabel;
    QComboBox *WindowCB;
    QSpinBox *AverageNumSB;
    QComboBox *FftModeCB;
    QLabel *AveragesNumLabel;
    QLabel *FftInversionLabel;
    QCheckBox *InfoCB;
    QSpinBox *ReadOutPerSecSB;
    QLabel *MaxAverageNumber;
    QLabel *WindowLabel;
    QLabel *ReadOutPerSecLabel;
    QDoubleSpinBox *ShiftFreqSB;
    QLabel *HzLabel;
    QLabel *ShiftFreqLabel;
    QLabel *WindowAlphaLabel;
    QDoubleSpinBox *WindowAlphaSB;
    QLabel *DCOffsetLabel;
    QCheckBox *FftShiftStepCB;
    QDoubleSpinBox *DCOffsetRe;
    QDoubleSpinBox *DCOffsetIm;
    QLabel *DCOffsetLabel_2;
    QLabel *DCOffsetLabel_3;
    QCheckBox *NullBinCircleCB;
    QWidget *tab;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QLabel *label;
    QLineEdit *lineEdit;
    QWidget *ASKRTab;
    QPushButton *FirstValidatePB;
    QComboBox *comboBox;
    QLabel *BandLabel;
    QLabel *WindowTitleLabel;
    QToolButton *CloseButton;

    void setupUi(QDialog *config_manager)
    {
        if (config_manager->objectName().isEmpty())
            config_manager->setObjectName(QStringLiteral("config_manager"));
        config_manager->resize(400, 302);
        config_manager->setMinimumSize(QSize(400, 302));
        config_manager->setMaximumSize(QSize(400, 324));
        QIcon icon;
        icon.addFile(QStringLiteral(":/buttons/SquidLow.png"), QSize(), QIcon::Normal, QIcon::Off);
        config_manager->setWindowIcon(icon);
        InterfaceWidget = new QWidget(config_manager);
        InterfaceWidget->setObjectName(QStringLiteral("InterfaceWidget"));
        InterfaceWidget->setGeometry(QRect(0, 0, 400, 292));
        InterfaceWidget->setMinimumSize(QSize(400, 1));
        InterfaceWidget->setMaximumSize(QSize(400, 322));
        ConfigTab = new QTabWidget(InterfaceWidget);
        ConfigTab->setObjectName(QStringLiteral("ConfigTab"));
        ConfigTab->setGeometry(QRect(4, 24, 392, 262));
        ConfigTab->setMinimumSize(QSize(392, 200));
        ConfigTab->setMaximumSize(QSize(392, 290));
        QFont font;
        font.setPointSize(10);
        ConfigTab->setFont(font);
        ConfigTab->setTabPosition(QTabWidget::North);
        ConfigTab->setTabShape(QTabWidget::Triangular);
        ConfigTab->setIconSize(QSize(16, 16));
        ConfigTab->setElideMode(Qt::ElideMiddle);
        ConfigTab->setUsesScrollButtons(false);
        ConfigTab->setDocumentMode(true);
        ConfigTab->setTabsClosable(false);
        ConfigTab->setMovable(true);
        ConfigTab->setTabBarAutoHide(false);
        RPUTab = new QWidget();
        RPUTab->setObjectName(QStringLiteral("RPUTab"));
        LPTPortCB = new QComboBox(RPUTab);
        LPTPortCB->setObjectName(QStringLiteral("LPTPortCB"));
        LPTPortCB->setGeometry(QRect(75, 12, 88, 22));
        QFont font1;
        font1.setPointSize(12);
        LPTPortCB->setFont(font1);
        LPTPortL = new QLabel(RPUTab);
        LPTPortL->setObjectName(QStringLiteral("LPTPortL"));
        LPTPortL->setGeometry(QRect(24, 12, 46, 22));
        LPTPortL->setFont(font1);
        ConfigLabel = new QLabel(RPUTab);
        ConfigLabel->setObjectName(QStringLiteral("ConfigLabel"));
        ConfigLabel->setGeometry(QRect(174, 12, 55, 22));
        ConfigLabel->setFont(font1);
        TractConfigCB = new QComboBox(RPUTab);
        TractConfigCB->setObjectName(QStringLiteral("TractConfigCB"));
        TractConfigCB->setGeometry(QRect(231, 12, 130, 22));
        TractConfigCB->setFont(font1);
        RPUTabs = new QTabWidget(RPUTab);
        RPUTabs->setObjectName(QStringLiteral("RPUTabs"));
        RPUTabs->setGeometry(QRect(9, 45, 372, 190));
        RPUTabs->setMinimumSize(QSize(372, 190));
        RPUTabs->setMaximumSize(QSize(372, 190));
        RPUTabs->setFont(font);
        RPUTabs->setTabShape(QTabWidget::Triangular);
        RPUTabs->setElideMode(Qt::ElideNone);
        RPUTabs->setDocumentMode(true);
        RPUTabs->setTabsClosable(false);
        RPUTabs->setTabBarAutoHide(false);
        Tract1Tab = new QWidget();
        Tract1Tab->setObjectName(QStringLiteral("Tract1Tab"));
        Tract1_AttInBox = new QComboBox(Tract1Tab);
        Tract1_AttInBox->setObjectName(QStringLiteral("Tract1_AttInBox"));
        Tract1_AttInBox->setGeometry(QRect(153, 42, 85, 22));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Tract1_AttInBox->sizePolicy().hasHeightForWidth());
        Tract1_AttInBox->setSizePolicy(sizePolicy);
        Tract1_AttInBox->setFont(font1);
        Tract1_AttHFLabel = new QLabel(Tract1Tab);
        Tract1_AttHFLabel->setObjectName(QStringLiteral("Tract1_AttHFLabel"));
        Tract1_AttHFLabel->setGeometry(QRect(27, 78, 124, 22));
        sizePolicy.setHeightForWidth(Tract1_AttHFLabel->sizePolicy().hasHeightForWidth());
        Tract1_AttHFLabel->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font2.setPointSize(12);
        Tract1_AttHFLabel->setFont(font2);
        Tract1_AttIFBox = new QComboBox(Tract1Tab);
        Tract1_AttIFBox->setObjectName(QStringLiteral("Tract1_AttIFBox"));
        Tract1_AttIFBox->setGeometry(QRect(153, 114, 85, 22));
        sizePolicy.setHeightForWidth(Tract1_AttIFBox->sizePolicy().hasHeightForWidth());
        Tract1_AttIFBox->setSizePolicy(sizePolicy);
        Tract1_AttIFBox->setFont(font1);
        Tract1_AttHFBox = new QComboBox(Tract1Tab);
        Tract1_AttHFBox->setObjectName(QStringLiteral("Tract1_AttHFBox"));
        Tract1_AttHFBox->setGeometry(QRect(153, 78, 85, 22));
        sizePolicy.setHeightForWidth(Tract1_AttHFBox->sizePolicy().hasHeightForWidth());
        Tract1_AttHFBox->setSizePolicy(sizePolicy);
        Tract1_AttHFBox->setFont(font1);
        Tract1_kHzLabel = new QLabel(Tract1Tab);
        Tract1_kHzLabel->setObjectName(QStringLiteral("Tract1_kHzLabel"));
        Tract1_kHzLabel->setGeometry(QRect(270, 6, 43, 25));
        QFont font3;
        font3.setPointSize(16);
        Tract1_kHzLabel->setFont(font3);
        Tract1_kHzLabel->setFrameShape(QFrame::NoFrame);
        Tract1_kHzLabel->setFrameShadow(QFrame::Plain);
        Tract1_kHzLabel->setTextFormat(Qt::AutoText);
        Tract1_BandIFBox = new QComboBox(Tract1Tab);
        Tract1_BandIFBox->setObjectName(QStringLiteral("Tract1_BandIFBox"));
        Tract1_BandIFBox->setGeometry(QRect(153, 147, 85, 22));
        sizePolicy.setHeightForWidth(Tract1_BandIFBox->sizePolicy().hasHeightForWidth());
        Tract1_BandIFBox->setSizePolicy(sizePolicy);
        Tract1_BandIFBox->setFont(font1);
        Tract1_AttIFLabel = new QLabel(Tract1Tab);
        Tract1_AttIFLabel->setObjectName(QStringLiteral("Tract1_AttIFLabel"));
        Tract1_AttIFLabel->setGeometry(QRect(24, 111, 124, 22));
        sizePolicy.setHeightForWidth(Tract1_AttIFLabel->sizePolicy().hasHeightForWidth());
        Tract1_AttIFLabel->setSizePolicy(sizePolicy);
        Tract1_AttIFLabel->setFont(font2);
        Tract1_AttInLabel = new QLabel(Tract1Tab);
        Tract1_AttInLabel->setObjectName(QStringLiteral("Tract1_AttInLabel"));
        Tract1_AttInLabel->setGeometry(QRect(12, 40, 139, 22));
        sizePolicy.setHeightForWidth(Tract1_AttInLabel->sizePolicy().hasHeightForWidth());
        Tract1_AttInLabel->setSizePolicy(sizePolicy);
        Tract1_AttInLabel->setFont(font2);
        Tract1_BandIFLabel = new QLabel(Tract1Tab);
        Tract1_BandIFLabel->setObjectName(QStringLiteral("Tract1_BandIFLabel"));
        Tract1_BandIFLabel->setGeometry(QRect(60, 144, 88, 22));
        sizePolicy.setHeightForWidth(Tract1_BandIFLabel->sizePolicy().hasHeightForWidth());
        Tract1_BandIFLabel->setSizePolicy(sizePolicy);
        Tract1_BandIFLabel->setFont(font2);
        Tract1_CentralFreqSB = new QSpinBox(Tract1Tab);
        Tract1_CentralFreqSB->setObjectName(QStringLiteral("Tract1_CentralFreqSB"));
        Tract1_CentralFreqSB->setGeometry(QRect(156, 9, 109, 22));
        Tract1_CentralFreqSB->setFont(font1);
        Tract1_CentralFreqSB->setFrame(false);
        Tract1_CentralFreqSB->setMinimum(150);
        Tract1_CentralFreqSB->setMaximum(32000);
        Tract1_CentralFreqSB->setSingleStep(10);
        Tract1_CentralFreqSB->setValue(16000);
        Tract1_CentralFreqLabel = new QLabel(Tract1Tab);
        Tract1_CentralFreqLabel->setObjectName(QStringLiteral("Tract1_CentralFreqLabel"));
        Tract1_CentralFreqLabel->setGeometry(QRect(4, 7, 145, 22));
        Tract1_CentralFreqLabel->setFont(font1);
        RPUTabs->addTab(Tract1Tab, QString());
        Tract2Tab = new QWidget();
        Tract2Tab->setObjectName(QStringLiteral("Tract2Tab"));
        RPUTabs->addTab(Tract2Tab, QString());
        Tract3Tab = new QWidget();
        Tract3Tab->setObjectName(QStringLiteral("Tract3Tab"));
        RPUTabs->addTab(Tract3Tab, QString());
        Tract4Tab = new QWidget();
        Tract4Tab->setObjectName(QStringLiteral("Tract4Tab"));
        RPUTabs->addTab(Tract4Tab, QString());
        KalibratorTab = new QWidget();
        KalibratorTab->setObjectName(QStringLiteral("KalibratorTab"));
        KalibKHZLabel = new QLabel(KalibratorTab);
        KalibKHZLabel->setObjectName(QStringLiteral("KalibKHZLabel"));
        KalibKHZLabel->setGeometry(QRect(336, 40, 28, 22));
        KalibKHZLabel->setFont(font1);
        KalibExitTypeBox = new QComboBox(KalibratorTab);
        KalibExitTypeBox->setObjectName(QStringLiteral("KalibExitTypeBox"));
        KalibExitTypeBox->setGeometry(QRect(59, 41, 124, 22));
        KalibExitTypeBox->setFont(font1);
        KalibExitTypeBox->setCursor(QCursor(Qt::PointingHandCursor));
        KalibModulationFrequencyBox = new QSpinBox(KalibratorTab);
        KalibModulationFrequencyBox->setObjectName(QStringLiteral("KalibModulationFrequencyBox"));
        KalibModulationFrequencyBox->setGeometry(QRect(237, 41, 97, 22));
        KalibModulationFrequencyBox->setFont(font1);
        KalibModulationFrequencyBox->setCursor(QCursor(Qt::PointingHandCursor));
        KalibModulationFrequencyBox->setMinimum(1);
        KalibModulationFrequencyBox->setMaximum(50);
        KalibExitTypeLabel = new QLabel(KalibratorTab);
        KalibExitTypeLabel->setObjectName(QStringLiteral("KalibExitTypeLabel"));
        KalibExitTypeLabel->setGeometry(QRect(4, 40, 55, 22));
        KalibExitTypeLabel->setFont(font2);
        KalibAttenLabel = new QLabel(KalibratorTab);
        KalibAttenLabel->setObjectName(QStringLiteral("KalibAttenLabel"));
        KalibAttenLabel->setGeometry(QRect(189, 10, 100, 22));
        KalibAttenLabel->setFont(font2);
        KalibSignalTypeLabel = new QLabel(KalibratorTab);
        KalibSignalTypeLabel->setObjectName(QStringLiteral("KalibSignalTypeLabel"));
        KalibSignalTypeLabel->setGeometry(QRect(3, 73, 97, 22));
        KalibSignalTypeLabel->setFont(font2);
        KalibAttenTypeBox = new QComboBox(KalibratorTab);
        KalibAttenTypeBox->setObjectName(QStringLiteral("KalibAttenTypeBox"));
        KalibAttenTypeBox->setGeometry(QRect(291, 10, 70, 22));
        KalibAttenTypeBox->setFont(font1);
        KalibAttenTypeBox->setCursor(QCursor(Qt::PointingHandCursor));
        KalibAttenTypeBox->setMaxVisibleItems(8);
        KalibModulationFrequencyLabel = new QLabel(KalibratorTab);
        KalibModulationFrequencyLabel->setObjectName(QStringLiteral("KalibModulationFrequencyLabel"));
        KalibModulationFrequencyLabel->setGeometry(QRect(190, 40, 43, 22));
        KalibModulationFrequencyLabel->setFont(font2);
        KalibStatusLabel = new QLabel(KalibratorTab);
        KalibStatusLabel->setObjectName(QStringLiteral("KalibStatusLabel"));
        KalibStatusLabel->setGeometry(QRect(4, 7, 58, 22));
        KalibStatusLabel->setFont(font2);
        KalibStatusBox = new QComboBox(KalibratorTab);
        KalibStatusBox->setObjectName(QStringLiteral("KalibStatusBox"));
        KalibStatusBox->setGeometry(QRect(62, 9, 121, 22));
        KalibStatusBox->setFont(font1);
        KalibStatusBox->setCursor(QCursor(Qt::PointingHandCursor));
        KalibSignalTypeBox = new QComboBox(KalibratorTab);
        KalibSignalTypeBox->setObjectName(QStringLiteral("KalibSignalTypeBox"));
        KalibSignalTypeBox->setGeometry(QRect(105, 73, 136, 22));
        KalibSignalTypeBox->setFont(font1);
        KalibSignalTypeBox->setCursor(QCursor(Qt::PointingHandCursor));
        RPUTabs->addTab(KalibratorTab, QString());
        RPUMainTopLine = new QFrame(RPUTab);
        RPUMainTopLine->setObjectName(QStringLiteral("RPUMainTopLine"));
        RPUMainTopLine->setGeometry(QRect(6, 30, 382, 17));
        RPUMainTopLine->setFrameShape(QFrame::HLine);
        RPUMainTopLine->setFrameShadow(QFrame::Sunken);
        ConfigTab->addTab(RPUTab, QString());
        SDRTab = new QWidget();
        SDRTab->setObjectName(QStringLiteral("SDRTab"));
        RefreshButton = new QPushButton(SDRTab);
        RefreshButton->setObjectName(QStringLiteral("RefreshButton"));
        RefreshButton->setGeometry(QRect(10, 15, 25, 25));
        RefreshButton->setIconSize(QSize(19, 19));
        RefreshButton->setFlat(false);
        CurrentDevNumLabel = new QLabel(SDRTab);
        CurrentDevNumLabel->setObjectName(QStringLiteral("CurrentDevNumLabel"));
        CurrentDevNumLabel->setGeometry(QRect(40, 16, 21, 22));
        CurrentDevNumLabel->setFont(font1);
        CurrentDevSpinBox = new QSpinBox(SDRTab);
        CurrentDevSpinBox->setObjectName(QStringLiteral("CurrentDevSpinBox"));
        CurrentDevSpinBox->setGeometry(QRect(63, 16, 35, 22));
        CurrentDevSpinBox->setFont(font1);
        CurrentDevSpinBox->setWrapping(false);
        CurrentDevSpinBox->setFrame(false);
        CurrentDevSpinBox->setMaximum(0);
        ConfirmButton = new QPushButton(SDRTab);
        ConfirmButton->setObjectName(QStringLiteral("ConfirmButton"));
        ConfirmButton->setGeometry(QRect(102, 15, 25, 25));
        ConfirmButton->setIconSize(QSize(19, 19));
        SampleRateLabel = new QLabel(SDRTab);
        SampleRateLabel->setObjectName(QStringLiteral("SampleRateLabel"));
        SampleRateLabel->setGeometry(QRect(132, 16, 29, 25));
        SampleRateLabel->setFont(font1);
        SampleRateSB = new QSpinBox(SDRTab);
        SampleRateSB->setObjectName(QStringLiteral("SampleRateSB"));
        SampleRateSB->setGeometry(QRect(159, 17, 99, 22));
        SampleRateSB->setFont(font1);
        SampleRateSB->setFrame(false);
        SampleRateSB->setReadOnly(false);
        SampleRateSB->setMinimum(230000);
        SampleRateSB->setMaximum(2400000);
        SampleRateSB->setSingleStep(5000);
        SampleRateSB->setValue(240000);
        GainLabel = new QLabel(SDRTab);
        GainLabel->setObjectName(QStringLiteral("GainLabel"));
        GainLabel->setGeometry(QRect(271, 16, 27, 25));
        QFont font4;
        font4.setPointSize(13);
        GainLabel->setFont(font4);
        GainValBox = new QComboBox(SDRTab);
        GainValBox->setObjectName(QStringLiteral("GainValBox"));
        GainValBox->setGeometry(QRect(304, 18, 81, 22));
        GainValBox->setFont(font1);
        GainValBox->setMaxCount(29);
        DirectSamplingCB = new QComboBox(SDRTab);
        DirectSamplingCB->setObjectName(QStringLiteral("DirectSamplingCB"));
        DirectSamplingCB->setGeometry(QRect(157, 83, 154, 23));
        DirectSamplingCB->setFont(font1);
        DirectSamplingCB->setMaxCount(29);
        DirectSamplingLabel = new QLabel(SDRTab);
        DirectSamplingLabel->setObjectName(QStringLiteral("DirectSamplingLabel"));
        DirectSamplingLabel->setGeometry(QRect(10, 82, 151, 22));
        DirectSamplingLabel->setFont(font1);
        CentralFreqSB = new QSpinBox(SDRTab);
        CentralFreqSB->setObjectName(QStringLiteral("CentralFreqSB"));
        CentralFreqSB->setGeometry(QRect(159, 50, 145, 22));
        CentralFreqSB->setFont(font1);
        CentralFreqSB->setFrame(false);
        CentralFreqSB->setMinimum(0);
        CentralFreqSB->setMaximum(999999999);
        CentralFreqSB->setSingleStep(10000);
        CentralFreqSB->setValue(104200000);
        CentralFreqLabel = new QLabel(SDRTab);
        CentralFreqLabel->setObjectName(QStringLiteral("CentralFreqLabel"));
        CentralFreqLabel->setGeometry(QRect(10, 49, 145, 22));
        CentralFreqLabel->setFont(font1);
        HzLabel_2 = new QLabel(SDRTab);
        HzLabel_2->setObjectName(QStringLiteral("HzLabel_2"));
        HzLabel_2->setGeometry(QRect(309, 49, 29, 22));
        HzLabel_2->setFont(font1);
        ConfigTab->addTab(SDRTab, QString());
        DSPTab = new QWidget();
        DSPTab->setObjectName(QStringLiteral("DSPTab"));
        DSPWidget = new QTabWidget(DSPTab);
        DSPWidget->setObjectName(QStringLiteral("DSPWidget"));
        DSPWidget->setGeometry(QRect(6, 14, 376, 205));
        DSPWidget->setTabShape(QTabWidget::Triangular);
        RecordTab = new QWidget();
        RecordTab->setObjectName(QStringLiteral("RecordTab"));
        CurrentPathButton = new QPushButton(RecordTab);
        CurrentPathButton->setObjectName(QStringLiteral("CurrentPathButton"));
        CurrentPathButton->setGeometry(QRect(242, 8, 121, 25));
        CurrentPathButton->setMaximumSize(QSize(16777215, 16777215));
        CurrentPathButton->setFont(font4);
        FltSignalCB = new QCheckBox(RecordTab);
        FltSignalCB->setObjectName(QStringLiteral("FltSignalCB"));
        FltSignalCB->setGeometry(QRect(159, 81, 16, 16));
        FltSignalCB->setFont(font4);
        RecordTimeLabel = new QLabel(RecordTab);
        RecordTimeLabel->setObjectName(QStringLiteral("RecordTimeLabel"));
        RecordTimeLabel->setGeometry(QRect(4, 9, 121, 22));
        RecordTimeLabel->setFont(font4);
        RealSignalCB = new QCheckBox(RecordTab);
        RealSignalCB->setObjectName(QStringLiteral("RealSignalCB"));
        RealSignalCB->setGeometry(QRect(159, 114, 16, 16));
        RealSignalCB->setFont(font4);
        ADCSignalLabel = new QLabel(RecordTab);
        ADCSignalLabel->setObjectName(QStringLiteral("ADCSignalLabel"));
        ADCSignalLabel->setGeometry(QRect(4, 42, 111, 22));
        ADCSignalLabel->setFont(font4);
        RecordTimeCB = new QComboBox(RecordTab);
        RecordTimeCB->setObjectName(QStringLiteral("RecordTimeCB"));
        RecordTimeCB->setGeometry(QRect(123, 10, 111, 22));
        RecordTimeCB->setFont(font1);
        RealSignalLabel = new QLabel(RecordTab);
        RealSignalLabel->setObjectName(QStringLiteral("RealSignalLabel"));
        RealSignalLabel->setGeometry(QRect(4, 108, 131, 22));
        RealSignalLabel->setFont(font4);
        FltSignalLabel = new QLabel(RecordTab);
        FltSignalLabel->setObjectName(QStringLiteral("FltSignalLabel"));
        FltSignalLabel->setGeometry(QRect(4, 75, 151, 22));
        FltSignalLabel->setFont(font4);
        ADCSignalCB = new QCheckBox(RecordTab);
        ADCSignalCB->setObjectName(QStringLiteral("ADCSignalCB"));
        ADCSignalCB->setGeometry(QRect(159, 46, 16, 16));
        ADCSignalCB->setFont(font4);
        RecordButton = new QPushButton(RecordTab);
        RecordButton->setObjectName(QStringLiteral("RecordButton"));
        RecordButton->setGeometry(QRect(138, 147, 75, 25));
        RecordButton->setFont(font1);
        ADCBar = new QProgressBar(RecordTab);
        ADCBar->setObjectName(QStringLiteral("ADCBar"));
        ADCBar->setGeometry(QRect(180, 45, 181, 17));
        ADCBar->setValue(0);
        ADCBar->setTextVisible(false);
        FltBar = new QProgressBar(RecordTab);
        FltBar->setObjectName(QStringLiteral("FltBar"));
        FltBar->setGeometry(QRect(180, 79, 181, 17));
        FltBar->setValue(0);
        FltBar->setTextVisible(false);
        RealBar = new QProgressBar(RecordTab);
        RealBar->setObjectName(QStringLiteral("RealBar"));
        RealBar->setGeometry(QRect(180, 112, 181, 17));
        RealBar->setValue(0);
        RealBar->setTextVisible(false);
        DSPWidget->addTab(RecordTab, QString());
        FiltrationTab = new QWidget();
        FiltrationTab->setObjectName(QStringLiteral("FiltrationTab"));
        FiltrationCB = new QCheckBox(FiltrationTab);
        FiltrationCB->setObjectName(QStringLiteral("FiltrationCB"));
        FiltrationCB->setGeometry(QRect(188, 14, 15, 17));
        FiltrationCB->setLayoutDirection(Qt::RightToLeft);
        FiltrationCB->setTristate(false);
        PassbandValSB = new QSpinBox(FiltrationTab);
        PassbandValSB->setObjectName(QStringLiteral("PassbandValSB"));
        PassbandValSB->setGeometry(QRect(176, 42, 99, 22));
        QFont font5;
        font5.setPointSize(11);
        PassbandValSB->setFont(font5);
        PassbandValSB->setFrame(false);
        PassbandLabel = new QLabel(FiltrationTab);
        PassbandLabel->setObjectName(QStringLiteral("PassbandLabel"));
        PassbandLabel->setGeometry(QRect(4, 42, 171, 22));
        PassbandLabel->setFont(font4);
        UseFltLabel = new QLabel(FiltrationTab);
        UseFltLabel->setObjectName(QStringLiteral("UseFltLabel"));
        UseFltLabel->setGeometry(QRect(4, 9, 177, 22));
        UseFltLabel->setFont(font4);
        BoombandLabel = new QLabel(FiltrationTab);
        BoombandLabel->setObjectName(QStringLiteral("BoombandLabel"));
        BoombandLabel->setGeometry(QRect(4, 74, 171, 22));
        BoombandLabel->setFont(font4);
        HzLabel1 = new QLabel(FiltrationTab);
        HzLabel1->setObjectName(QStringLiteral("HzLabel1"));
        HzLabel1->setGeometry(QRect(280, 42, 25, 22));
        HzLabel1->setFont(font4);
        BoombandValSB = new QSpinBox(FiltrationTab);
        BoombandValSB->setObjectName(QStringLiteral("BoombandValSB"));
        BoombandValSB->setGeometry(QRect(176, 76, 99, 22));
        BoombandValSB->setFont(font5);
        BoombandValSB->setFrame(false);
        HzLabel2 = new QLabel(FiltrationTab);
        HzLabel2->setObjectName(QStringLiteral("HzLabel2"));
        HzLabel2->setGeometry(QRect(280, 76, 25, 22));
        HzLabel2->setFont(font4);
        OutSampleFreqLabel = new QLabel(FiltrationTab);
        OutSampleFreqLabel->setObjectName(QStringLiteral("OutSampleFreqLabel"));
        OutSampleFreqLabel->setGeometry(QRect(4, 106, 147, 22));
        OutSampleFreqLabel->setFont(font4);
        OutSampleFreqCB = new QComboBox(FiltrationTab);
        OutSampleFreqCB->setObjectName(QStringLiteral("OutSampleFreqCB"));
        OutSampleFreqCB->setGeometry(QRect(156, 108, 69, 22));
        OutSampleFreqCB->setFont(font1);
        HzLabel3 = new QLabel(FiltrationTab);
        HzLabel3->setObjectName(QStringLiteral("HzLabel3"));
        HzLabel3->setGeometry(QRect(230, 108, 25, 22));
        HzLabel3->setFont(font4);
        DSPWidget->addTab(FiltrationTab, QString());
        FftTab = new QWidget();
        FftTab->setObjectName(QStringLiteral("FftTab"));
        InversionCB = new QCheckBox(FftTab);
        InversionCB->setObjectName(QStringLiteral("InversionCB"));
        InversionCB->setGeometry(QRect(351, 49, 16, 16));
        InversionCB->setIconSize(QSize(25, 29));
        InversionCB->setTristate(false);
        FftModeLabel = new QLabel(FftTab);
        FftModeLabel->setObjectName(QStringLiteral("FftModeLabel"));
        FftModeLabel->setGeometry(QRect(3, 11, 57, 22));
        FftModeLabel->setFont(font1);
        InfoLabel = new QLabel(FftTab);
        InfoLabel->setObjectName(QStringLiteral("InfoLabel"));
        InfoLabel->setGeometry(QRect(243, 11, 109, 22));
        InfoLabel->setFont(font4);
        WindowCB = new QComboBox(FftTab);
        WindowCB->setObjectName(QStringLiteral("WindowCB"));
        WindowCB->setGeometry(QRect(54, 77, 101, 22));
        WindowCB->setFont(font1);
        AverageNumSB = new QSpinBox(FftTab);
        AverageNumSB->setObjectName(QStringLiteral("AverageNumSB"));
        AverageNumSB->setGeometry(QRect(132, 44, 61, 22));
        AverageNumSB->setFont(font5);
        AverageNumSB->setLayoutDirection(Qt::LeftToRight);
        AverageNumSB->setFrame(false);
        AverageNumSB->setMinimum(1);
        AverageNumSB->setMaximum(100);
        AverageNumSB->setValue(1);
        FftModeCB = new QComboBox(FftTab);
        FftModeCB->setObjectName(QStringLiteral("FftModeCB"));
        FftModeCB->setGeometry(QRect(69, 11, 167, 22));
        FftModeCB->setFont(font5);
        FftModeCB->setMaxCount(29);
        AveragesNumLabel = new QLabel(FftTab);
        AveragesNumLabel->setObjectName(QStringLiteral("AveragesNumLabel"));
        AveragesNumLabel->setGeometry(QRect(94, 44, 36, 22));
        AveragesNumLabel->setFont(font1);
        FftInversionLabel = new QLabel(FftTab);
        FftInversionLabel->setObjectName(QStringLiteral("FftInversionLabel"));
        FftInversionLabel->setGeometry(QRect(267, 44, 81, 22));
        FftInversionLabel->setFont(font4);
        InfoCB = new QCheckBox(FftTab);
        InfoCB->setObjectName(QStringLiteral("InfoCB"));
        InfoCB->setGeometry(QRect(351, 16, 16, 16));
        ReadOutPerSecSB = new QSpinBox(FftTab);
        ReadOutPerSecSB->setObjectName(QStringLiteral("ReadOutPerSecSB"));
        ReadOutPerSecSB->setGeometry(QRect(39, 44, 47, 22));
        ReadOutPerSecSB->setFont(font5);
        ReadOutPerSecSB->setFrame(false);
        ReadOutPerSecSB->setMinimum(1);
        ReadOutPerSecSB->setMaximum(20);
        ReadOutPerSecSB->setValue(10);
        MaxAverageNumber = new QLabel(FftTab);
        MaxAverageNumber->setObjectName(QStringLiteral("MaxAverageNumber"));
        MaxAverageNumber->setGeometry(QRect(196, 44, 49, 22));
        MaxAverageNumber->setFont(font1);
        WindowLabel = new QLabel(FftTab);
        WindowLabel->setObjectName(QStringLiteral("WindowLabel"));
        WindowLabel->setGeometry(QRect(3, 77, 49, 22));
        WindowLabel->setFont(font1);
        ReadOutPerSecLabel = new QLabel(FftTab);
        ReadOutPerSecLabel->setObjectName(QStringLiteral("ReadOutPerSecLabel"));
        ReadOutPerSecLabel->setGeometry(QRect(3, 44, 35, 22));
        ReadOutPerSecLabel->setFont(font4);
        ShiftFreqSB = new QDoubleSpinBox(FftTab);
        ShiftFreqSB->setObjectName(QStringLiteral("ShiftFreqSB"));
        ShiftFreqSB->setGeometry(QRect(141, 110, 136, 22));
        ShiftFreqSB->setFont(font4);
        ShiftFreqSB->setFrame(false);
        ShiftFreqSB->setDecimals(4);
        ShiftFreqSB->setMaximum(15);
        ShiftFreqSB->setSingleStep(1367.19);
        HzLabel = new QLabel(FftTab);
        HzLabel->setObjectName(QStringLiteral("HzLabel"));
        HzLabel->setGeometry(QRect(285, 110, 22, 22));
        HzLabel->setFont(font1);
        ShiftFreqLabel = new QLabel(FftTab);
        ShiftFreqLabel->setObjectName(QStringLiteral("ShiftFreqLabel"));
        ShiftFreqLabel->setGeometry(QRect(3, 110, 132, 22));
        ShiftFreqLabel->setFont(font1);
        WindowAlphaLabel = new QLabel(FftTab);
        WindowAlphaLabel->setObjectName(QStringLiteral("WindowAlphaLabel"));
        WindowAlphaLabel->setGeometry(QRect(165, 77, 64, 22));
        WindowAlphaLabel->setFont(font1);
        WindowAlphaSB = new QDoubleSpinBox(FftTab);
        WindowAlphaSB->setObjectName(QStringLiteral("WindowAlphaSB"));
        WindowAlphaSB->setGeometry(QRect(230, 77, 67, 22));
        WindowAlphaSB->setFont(font4);
        WindowAlphaSB->setFrame(false);
        WindowAlphaSB->setDecimals(3);
        WindowAlphaSB->setMinimum(-1000);
        WindowAlphaSB->setMaximum(1000);
        WindowAlphaSB->setSingleStep(0.5);
        WindowAlphaSB->setValue(0.5);
        DCOffsetLabel = new QLabel(FftTab);
        DCOffsetLabel->setObjectName(QStringLiteral("DCOffsetLabel"));
        DCOffsetLabel->setGeometry(QRect(6, 144, 83, 22));
        DCOffsetLabel->setFont(font1);
        FftShiftStepCB = new QCheckBox(FftTab);
        FftShiftStepCB->setObjectName(QStringLiteral("FftShiftStepCB"));
        FftShiftStepCB->setGeometry(QRect(309, 111, 52, 22));
        FftShiftStepCB->setFont(font1);
        DCOffsetRe = new QDoubleSpinBox(FftTab);
        DCOffsetRe->setObjectName(QStringLiteral("DCOffsetRe"));
        DCOffsetRe->setGeometry(QRect(110, 146, 87, 22));
        sizePolicy.setHeightForWidth(DCOffsetRe->sizePolicy().hasHeightForWidth());
        DCOffsetRe->setSizePolicy(sizePolicy);
        DCOffsetRe->setFont(font4);
        DCOffsetRe->setFrame(false);
        DCOffsetRe->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        DCOffsetRe->setDecimals(3);
        DCOffsetRe->setMinimum(-256);
        DCOffsetRe->setMaximum(256);
        DCOffsetRe->setSingleStep(0.001);
        DCOffsetIm = new QDoubleSpinBox(FftTab);
        DCOffsetIm->setObjectName(QStringLiteral("DCOffsetIm"));
        DCOffsetIm->setGeometry(QRect(224, 145, 87, 22));
        DCOffsetIm->setFont(font4);
        DCOffsetIm->setFrame(false);
        DCOffsetIm->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        DCOffsetIm->setDecimals(3);
        DCOffsetIm->setMinimum(-256);
        DCOffsetIm->setMaximum(256);
        DCOffsetIm->setSingleStep(0.001);
        DCOffsetLabel_2 = new QLabel(FftTab);
        DCOffsetLabel_2->setObjectName(QStringLiteral("DCOffsetLabel_2"));
        DCOffsetLabel_2->setGeometry(QRect(98, 145, 10, 22));
        DCOffsetLabel_2->setFont(font1);
        DCOffsetLabel_3 = new QLabel(FftTab);
        DCOffsetLabel_3->setObjectName(QStringLiteral("DCOffsetLabel_3"));
        DCOffsetLabel_3->setGeometry(QRect(206, 144, 17, 22));
        DCOffsetLabel_3->setFont(font1);
        NullBinCircleCB = new QCheckBox(FftTab);
        NullBinCircleCB->setObjectName(QStringLiteral("NullBinCircleCB"));
        NullBinCircleCB->setGeometry(QRect(320, 138, 49, 33));
        DSPWidget->addTab(FftTab, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        radioButton = new QRadioButton(tab);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(6, 10, 43, 17));
        radioButton->setFont(font4);
        radioButton_2 = new QRadioButton(tab);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(60, 10, 57, 17));
        radioButton_2->setFont(font4);
        radioButton_3 = new QRadioButton(tab);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));
        radioButton_3->setGeometry(QRect(126, 10, 53, 17));
        radioButton_3->setFont(font4);
        radioButton_4 = new QRadioButton(tab);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));
        radioButton_4->setGeometry(QRect(192, 10, 49, 17));
        radioButton_4->setFont(font4);
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(8, 36, 69, 21));
        label->setFont(font4);
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(70, 36, 167, 22));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy1);
        lineEdit->setMinimumSize(QSize(0, 0));
        lineEdit->setFont(font4);
        lineEdit->setFrame(false);
        DSPWidget->addTab(tab, QString());
        ConfigTab->addTab(DSPTab, QString());
        ASKRTab = new QWidget();
        ASKRTab->setObjectName(QStringLiteral("ASKRTab"));
        FirstValidatePB = new QPushButton(ASKRTab);
        FirstValidatePB->setObjectName(QStringLiteral("FirstValidatePB"));
        FirstValidatePB->setGeometry(QRect(2, 10, 103, 25));
        FirstValidatePB->setFont(font1);
        comboBox = new QComboBox(ASKRTab);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(264, 6, 121, 22));
        BandLabel = new QLabel(ASKRTab);
        BandLabel->setObjectName(QStringLiteral("BandLabel"));
        BandLabel->setGeometry(QRect(174, 9, 79, 16));
        ConfigTab->addTab(ASKRTab, QString());
        WindowTitleLabel = new QLabel(config_manager);
        WindowTitleLabel->setObjectName(QStringLiteral("WindowTitleLabel"));
        WindowTitleLabel->setGeometry(QRect(0, 0, 400, 26));
        QFont font6;
        font6.setFamily(QStringLiteral("Comic Sans MS"));
        font6.setPointSize(14);
        WindowTitleLabel->setFont(font6);
        WindowTitleLabel->setFrameShape(QFrame::NoFrame);
        WindowTitleLabel->setAlignment(Qt::AlignCenter);
        CloseButton = new QToolButton(config_manager);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(374, 2, 23, 23));

        retranslateUi(config_manager);

        ConfigTab->setCurrentIndex(2);
        RPUTabs->setCurrentIndex(0);
        GainValBox->setCurrentIndex(0);
        DirectSamplingCB->setCurrentIndex(0);
        DSPWidget->setCurrentIndex(3);
        OutSampleFreqCB->setCurrentIndex(3);
        FftModeCB->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(config_manager);
    } // setupUi

    void retranslateUi(QDialog *config_manager)
    {
        config_manager->setWindowTitle(QApplication::translate("config_manager", "\320\232\320\276\320\275\321\204\320\270\320\263\321\203\321\200\320\260\321\202\320\276\321\200", Q_NULLPTR));
        LPTPortCB->clear();
        LPTPortCB->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "0x378", Q_NULLPTR)
         << QApplication::translate("config_manager", "0xDD00", Q_NULLPTR)
         << QApplication::translate("config_manager", "0xE010", Q_NULLPTR)
         << QApplication::translate("config_manager", "0xDC00", Q_NULLPTR)
         << QApplication::translate("config_manager", "0xD880", Q_NULLPTR)
        );
        LPTPortL->setText(QApplication::translate("config_manager", "\320\237\320\276\321\200\321\202:", Q_NULLPTR));
        ConfigLabel->setText(QApplication::translate("config_manager", "\320\240\320\265\320\266\320\270\320\274:", Q_NULLPTR));
        TractConfigCB->clear();
        TractConfigCB->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "4 \321\201\320\270\320\275\321\205\321\200\320\276\320\275\320\275\321\213\321\205", Q_NULLPTR)
         << QApplication::translate("config_manager", "3 + 1", Q_NULLPTR)
         << QApplication::translate("config_manager", "2 + 2", Q_NULLPTR)
         << QApplication::translate("config_manager", "2 + 1 + 1", Q_NULLPTR)
         << QApplication::translate("config_manager", "1 + 1 + 1 + 1", Q_NULLPTR)
        );
        Tract1_AttInBox->clear();
        Tract1_AttInBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\222\321\213\320\272\320\273.", Q_NULLPTR)
         << QApplication::translate("config_manager", "10 \320\264\320\221", Q_NULLPTR)
        );
        Tract1_AttHFLabel->setText(QApplication::translate("config_manager", "\320\220\321\202\321\202\320\265\320\275\321\216\320\260\321\202\320\276\321\200 \320\222\320\247:", Q_NULLPTR));
        Tract1_AttIFBox->clear();
        Tract1_AttIFBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\222\321\213\320\272\320\273.", Q_NULLPTR)
         << QApplication::translate("config_manager", "2 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "4 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "6 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "8 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "10 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "12 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "14 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "16 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "18 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "20 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "22 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "24 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "26 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "28 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "30 \320\264\320\221", Q_NULLPTR)
        );
        Tract1_AttHFBox->clear();
        Tract1_AttHFBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\222\321\213\320\272\320\273.", Q_NULLPTR)
         << QApplication::translate("config_manager", "6 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "12 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "18 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "24 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "30 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "36 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "42 \320\264\320\221", Q_NULLPTR)
        );
        Tract1_kHzLabel->setText(QApplication::translate("config_manager", "\320\272\320\223\321\206", Q_NULLPTR));
        Tract1_BandIFBox->clear();
        Tract1_BandIFBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "3 \320\272\320\223\321\206", Q_NULLPTR)
         << QApplication::translate("config_manager", "8 \320\272\320\223\321\206", Q_NULLPTR)
         << QApplication::translate("config_manager", "20 \320\272\320\223\321\206", Q_NULLPTR)
         << QApplication::translate("config_manager", "150 \320\272\320\223\321\206", Q_NULLPTR)
        );
        Tract1_AttIFLabel->setText(QApplication::translate("config_manager", "\320\220\321\202\321\202\320\265\320\275\321\216\320\260\321\202\320\276\321\200 \320\237\320\247:", Q_NULLPTR));
        Tract1_AttInLabel->setText(QApplication::translate("config_manager", "\320\220\321\202\321\202\320\265\320\275\321\216\320\260\321\202\320\276\321\200 \320\262\321\205\320\276\320\264:", Q_NULLPTR));
        Tract1_BandIFLabel->setText(QApplication::translate("config_manager", "\320\237\320\276\320\273\320\276\321\201\320\260 \320\237\320\247:", Q_NULLPTR));
        Tract1_CentralFreqLabel->setText(QApplication::translate("config_manager", "\320\247\320\260\321\201\321\202\320\276\321\202\320\260 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270:", Q_NULLPTR));
        RPUTabs->setTabText(RPUTabs->indexOf(Tract1Tab), QApplication::translate("config_manager", "\320\242\321\200\320\260\320\272\321\202 1", Q_NULLPTR));
        RPUTabs->setTabText(RPUTabs->indexOf(Tract2Tab), QApplication::translate("config_manager", "\320\242\321\200\320\260\320\272\321\202 2", Q_NULLPTR));
        RPUTabs->setTabText(RPUTabs->indexOf(Tract3Tab), QApplication::translate("config_manager", "\320\242\321\200\320\260\320\272\321\202 3", Q_NULLPTR));
        RPUTabs->setTabText(RPUTabs->indexOf(Tract4Tab), QApplication::translate("config_manager", "\320\242\321\200\320\260\320\272\321\202 4", Q_NULLPTR));
        KalibKHZLabel->setText(QApplication::translate("config_manager", "\320\272\320\223\321\206", Q_NULLPTR));
        KalibExitTypeBox->clear();
        KalibExitTypeBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\222\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\271", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\222\320\275\320\265\321\210\320\275\320\270\320\271", Q_NULLPTR)
        );
        KalibExitTypeLabel->setText(QApplication::translate("config_manager", "\320\222\321\213\321\205\320\276\320\264:", Q_NULLPTR));
        KalibAttenLabel->setText(QApplication::translate("config_manager", "\320\220\321\202\321\202\320\265\320\275\321\216\320\260\321\202\320\276\321\200:", Q_NULLPTR));
        KalibSignalTypeLabel->setText(QApplication::translate("config_manager", "\320\242\320\270\320\277 \321\201\320\270\320\263\320\275\320\260\320\273\320\260:", Q_NULLPTR));
        KalibAttenTypeBox->clear();
        KalibAttenTypeBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\222\321\213\320\272\320\273.", Q_NULLPTR)
         << QApplication::translate("config_manager", "2 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "4 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "6 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "8 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "10 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "12 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "14 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "16 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "18 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "20 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "22 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "24 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "26 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "28 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "30 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "32 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "34 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "36 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "38 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "40 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "42 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "44 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "46 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "48 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "50 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "52 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "54 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "56 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "58 \320\264\320\221", Q_NULLPTR)
         << QApplication::translate("config_manager", "60 \320\264\320\221", Q_NULLPTR)
        );
        KalibModulationFrequencyLabel->setText(QApplication::translate("config_manager", "F\320\274\320\276\320\264:", Q_NULLPTR));
        KalibStatusLabel->setText(QApplication::translate("config_manager", "\320\241\321\202\320\260\321\202\321\203\321\201:", Q_NULLPTR));
        KalibStatusBox->clear();
        KalibStatusBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\222\321\213\320\272\320\273", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\222\320\272\320\273", Q_NULLPTR)
        );
        KalibSignalTypeBox->clear();
        KalibSignalTypeBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\321\201\320\270\320\275\321\203\321\201\320\276\320\270\320\264\320\260", Q_NULLPTR)
         << QApplication::translate("config_manager", "\321\200\320\260\320\264\320\270\320\276\320\270\320\274\320\277\321\203\320\273\321\214\321\201", Q_NULLPTR)
        );
        RPUTabs->setTabText(RPUTabs->indexOf(KalibratorTab), QApplication::translate("config_manager", "\320\232\320\260\320\273\320\270\320\261\321\200\320\260\321\202\320\276\321\200", Q_NULLPTR));
        ConfigTab->setTabText(ConfigTab->indexOf(RPUTab), QApplication::translate("config_manager", "\320\240\320\237\320\243", Q_NULLPTR));
#ifndef QT_NO_WHATSTHIS
        RefreshButton->setWhatsThis(QApplication::translate("config_manager", "<html><head/><body><p>\320\236\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\320\265 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270 \320\276 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\275\321\213\321\205 SDR \320\277\321\200\320\270\320\265\320\274\320\275\320\270\320\272\320\260\321\205.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_WHATSTHIS
        RefreshButton->setText(QString());
        CurrentDevNumLabel->setText(QApplication::translate("config_manager", "\342\204\226", Q_NULLPTR));
        ConfirmButton->setText(QString());
        SampleRateLabel->setText(QApplication::translate("config_manager", "F\320\264:", Q_NULLPTR));
        GainLabel->setText(QApplication::translate("config_manager", "\320\232\320\243:", Q_NULLPTR));
        GainValBox->clear();
        GainValBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\260\320\262\321\202\320\276", Q_NULLPTR)
         << QApplication::translate("config_manager", "0", Q_NULLPTR)
         << QApplication::translate("config_manager", "0.9", Q_NULLPTR)
         << QApplication::translate("config_manager", "1.4", Q_NULLPTR)
         << QApplication::translate("config_manager", "2.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "3.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "7.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "8.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "12.5", Q_NULLPTR)
         << QApplication::translate("config_manager", "14.4", Q_NULLPTR)
         << QApplication::translate("config_manager", "15.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "16.6", Q_NULLPTR)
         << QApplication::translate("config_manager", "19.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "20.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "22.9", Q_NULLPTR)
         << QApplication::translate("config_manager", "25.4", Q_NULLPTR)
         << QApplication::translate("config_manager", "28", Q_NULLPTR)
         << QApplication::translate("config_manager", "29.7", Q_NULLPTR)
         << QApplication::translate("config_manager", "32.8", Q_NULLPTR)
         << QApplication::translate("config_manager", "33.8", Q_NULLPTR)
         << QApplication::translate("config_manager", "36.4", Q_NULLPTR)
         << QApplication::translate("config_manager", "37.2", Q_NULLPTR)
         << QApplication::translate("config_manager", "38.6", Q_NULLPTR)
         << QApplication::translate("config_manager", "40.2", Q_NULLPTR)
         << QApplication::translate("config_manager", "42.1", Q_NULLPTR)
         << QApplication::translate("config_manager", "43.4", Q_NULLPTR)
         << QApplication::translate("config_manager", "43.9", Q_NULLPTR)
         << QApplication::translate("config_manager", "44.5", Q_NULLPTR)
         << QApplication::translate("config_manager", "49.6", Q_NULLPTR)
        );
        DirectSamplingCB->clear();
        DirectSamplingCB->insertItems(0, QStringList()
         << QApplication::translate("config_manager", " \320\276\320\261\320\265 \320\272\320\262\320\260\320\264\321\200\320\260\321\202\321\203\321\200\321\213", Q_NULLPTR)
         << QApplication::translate("config_manager", " I \321\207\320\260\321\201\321\202\321\214", Q_NULLPTR)
         << QApplication::translate("config_manager", " Q \321\207\320\260\321\201\321\202\321\214", Q_NULLPTR)
        );
        DirectSamplingLabel->setText(QApplication::translate("config_manager", "\320\240\320\265\320\266\320\270\320\274 \320\276\321\206\320\270\321\204\321\200\320\276\320\262\320\272\320\270:", Q_NULLPTR));
        CentralFreqLabel->setText(QApplication::translate("config_manager", "\320\247\320\260\321\201\321\202\320\276\321\202\320\260 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270:", Q_NULLPTR));
        HzLabel_2->setText(QApplication::translate("config_manager", "\320\223\321\206", Q_NULLPTR));
        ConfigTab->setTabText(ConfigTab->indexOf(SDRTab), QApplication::translate("config_manager", "SDR", Q_NULLPTR));
        CurrentPathButton->setText(QApplication::translate("config_manager", "\320\224\320\270\321\200\320\265\320\272\321\202\320\276\321\200\320\270\321\217...", Q_NULLPTR));
        FltSignalCB->setText(QString());
        RecordTimeLabel->setText(QApplication::translate("config_manager", "\320\222\321\200\320\265\320\274\321\217 \320\267\320\260\320\277\320\270\321\201\320\270:", Q_NULLPTR));
        RealSignalCB->setText(QString());
        ADCSignalLabel->setText(QApplication::translate("config_manager", "\320\241\320\270\320\263\320\275\320\260\320\273 \321\201 \320\220\320\246\320\237", Q_NULLPTR));
        RecordTimeCB->clear();
        RecordTimeCB->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "10 \321\201.", Q_NULLPTR)
         << QApplication::translate("config_manager", "30 \321\201.", Q_NULLPTR)
         << QApplication::translate("config_manager", "60 \321\201.", Q_NULLPTR)
         << QApplication::translate("config_manager", "90 \321\201.", Q_NULLPTR)
         << QApplication::translate("config_manager", "120 \321\201.", Q_NULLPTR)
         << QApplication::translate("config_manager", "360 \321\201.", Q_NULLPTR)
        );
        RealSignalLabel->setText(QApplication::translate("config_manager", "\320\235\320\270\320\267\320\272\320\276\321\207\320\260\321\201\321\202\320\276\321\202\320\275\321\213\320\271", Q_NULLPTR));
        FltSignalLabel->setText(QApplication::translate("config_manager", "\320\236\321\202\321\204\320\270\320\273\321\214\321\202\321\200\320\276\320\262\320\260\320\275\320\275\321\213\320\271", Q_NULLPTR));
        ADCSignalCB->setText(QString());
        RecordButton->setText(QApplication::translate("config_manager", "\320\227\320\260\320\277\320\270\321\201\321\214", Q_NULLPTR));
        DSPWidget->setTabText(DSPWidget->indexOf(RecordTab), QApplication::translate("config_manager", "\320\227\320\260\320\277\320\270\321\201\321\214", Q_NULLPTR));
        FiltrationCB->setText(QString());
        PassbandLabel->setText(QApplication::translate("config_manager", "\320\237\320\276\320\273\320\276\321\201\320\260 \320\277\321\200\320\276\320\277\321\203\321\201\320\272\320\260\320\275\320\270\321\217:", Q_NULLPTR));
        UseFltLabel->setText(QApplication::translate("config_manager", "\320\230\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\214 \321\204\320\270\320\273\321\214\321\202\321\200:", Q_NULLPTR));
        BoombandLabel->setText(QApplication::translate("config_manager", "\320\237\320\276\320\273\320\276\321\201\320\260 \320\267\320\260\320\263\321\200\320\260\320\266\320\264\320\265\320\275\320\270\321\217:", Q_NULLPTR));
        HzLabel1->setText(QApplication::translate("config_manager", "\320\223\321\206", Q_NULLPTR));
        HzLabel2->setText(QApplication::translate("config_manager", "\320\223\321\206", Q_NULLPTR));
        OutSampleFreqLabel->setText(QApplication::translate("config_manager", "\320\222\321\213\321\205\320\276\320\264\320\275\320\260\321\217 \321\207\320\260\321\201\321\202\320\276\321\202\320\260:", Q_NULLPTR));
        OutSampleFreqCB->clear();
        OutSampleFreqCB->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "8000", Q_NULLPTR)
         << QApplication::translate("config_manager", "16000", Q_NULLPTR)
         << QApplication::translate("config_manager", "44100", Q_NULLPTR)
         << QApplication::translate("config_manager", "48000", Q_NULLPTR)
        );
        HzLabel3->setText(QApplication::translate("config_manager", "\320\223\321\206", Q_NULLPTR));
        DSPWidget->setTabText(DSPWidget->indexOf(FiltrationTab), QApplication::translate("config_manager", "\320\244\320\270\320\273\321\214\321\202\321\200\320\260\321\206\320\270\321\217", Q_NULLPTR));
        InversionCB->setText(QString());
        FftModeLabel->setText(QApplication::translate("config_manager", "\320\241\320\277\320\265\320\272\321\202\321\200:", Q_NULLPTR));
        InfoLabel->setText(QApplication::translate("config_manager", "\320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217:", Q_NULLPTR));
        WindowCB->clear();
        WindowCB->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\320\275\320\265\321\202", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\221\320\260\321\200\320\273\320\265\321\202\321\202\320\260", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\221\320\273\321\215\320\272\320\274\320\260\320\275\320\260", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\245\321\215\320\274\320\274\320\270\320\275\320\263\320\260", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\245\320\260\320\275\320\275\320\260", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\232\320\260\320\271\320\267\320\265\321\200\320\260", Q_NULLPTR)
        );
        FftModeCB->clear();
        FftModeCB->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "\321\201 \320\220\320\246\320\237", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\277\320\276\321\201\320\273\320\265 \321\204\320\270\320\273\321\214\321\202\321\200\320\260\321\206\320\270\320\270", Q_NULLPTR)
         << QApplication::translate("config_manager", "\320\277\320\265\321\200\320\265\320\275\320\265\321\201\320\265\320\275\320\275\321\213\320\271", Q_NULLPTR)
        );
        AveragesNumLabel->setText(QApplication::translate("config_manager", "Avg:", Q_NULLPTR));
        FftInversionLabel->setText(QApplication::translate("config_manager", "\320\230\320\275\320\262\320\265\321\200\321\201\320\270\321\217:", Q_NULLPTR));
        InfoCB->setText(QString());
        MaxAverageNumber->setText(QApplication::translate("config_manager", "?", Q_NULLPTR));
        WindowLabel->setText(QApplication::translate("config_manager", "\320\236\320\272\320\275\320\276:", Q_NULLPTR));
        ReadOutPerSecLabel->setText(QApplication::translate("config_manager", "FPS:", Q_NULLPTR));
        HzLabel->setText(QApplication::translate("config_manager", "\320\223\321\206", Q_NULLPTR));
        ShiftFreqLabel->setText(QApplication::translate("config_manager", "\320\247\320\260\321\201\321\202\320\276\321\202\320\275\321\213\320\271 \321\201\320\264\320\262\320\270\320\263:", Q_NULLPTR));
        WindowAlphaLabel->setText(QApplication::translate("config_manager", "\320\232\320\276\321\215\321\204-\321\202:", Q_NULLPTR));
        DCOffsetLabel->setText(QApplication::translate("config_manager", "\320\232\320\276\321\200\321\200\320\265\320\272\321\206\320\270\321\217", Q_NULLPTR));
        FftShiftStepCB->setText(QApplication::translate("config_manager", "\321\210\320\260\320\263", Q_NULLPTR));
        DCOffsetLabel_2->setText(QApplication::translate("config_manager", "i:", Q_NULLPTR));
        DCOffsetLabel_3->setText(QApplication::translate("config_manager", "q:", Q_NULLPTR));
        NullBinCircleCB->setText(QApplication::translate("config_manager", "0bin\n"
"circle", Q_NULLPTR));
        DSPWidget->setTabText(DSPWidget->indexOf(FftTab), QApplication::translate("config_manager", "\320\221\320\237\320\244", Q_NULLPTR));
        radioButton->setText(QApplication::translate("config_manager", "\320\220\320\234", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("config_manager", "\320\222\320\221\320\237", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("config_manager", "\320\235\320\221\320\237", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("config_manager", "\320\247\320\234", Q_NULLPTR));
        label->setText(QApplication::translate("config_manager", "\320\237\320\276\320\273\320\276\321\201\320\260", Q_NULLPTR));
        DSPWidget->setTabText(DSPWidget->indexOf(tab), QApplication::translate("config_manager", "\320\224\320\265\320\274\320\276\320\264\321\203\320\273\321\217\321\206\320\270\321\217", Q_NULLPTR));
        ConfigTab->setTabText(ConfigTab->indexOf(DSPTab), QApplication::translate("config_manager", "\320\246\320\236\320\241", Q_NULLPTR));
        FirstValidatePB->setText(QApplication::translate("config_manager", "\320\237\321\200\320\276\320\262\320\265\321\200\320\272\320\260 1", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("config_manager", "200-1000", Q_NULLPTR)
         << QApplication::translate("config_manager", "1000-1500", Q_NULLPTR)
         << QApplication::translate("config_manager", "1500-2200", Q_NULLPTR)
         << QApplication::translate("config_manager", "2200-3200", Q_NULLPTR)
         << QApplication::translate("config_manager", "3200-4700", Q_NULLPTR)
         << QApplication::translate("config_manager", "4700-6800", Q_NULLPTR)
         << QApplication::translate("config_manager", "6800-9900", Q_NULLPTR)
         << QApplication::translate("config_manager", "9900-14400", Q_NULLPTR)
         << QApplication::translate("config_manager", "14400-20700", Q_NULLPTR)
         << QApplication::translate("config_manager", "20700-32000", Q_NULLPTR)
        );
        BandLabel->setText(QApplication::translate("config_manager", "\320\237\321\200\320\265\321\201\320\265\320\273\320\265\320\272\321\202\320\276\321\200", Q_NULLPTR));
        ConfigTab->setTabText(ConfigTab->indexOf(ASKRTab), QApplication::translate("config_manager", "\320\220\320\241\320\232\320\240", Q_NULLPTR));
        WindowTitleLabel->setText(QApplication::translate("config_manager", "\320\232\320\276\320\275\321\204\320\270\320\263\321\203\321\200\320\260\321\202\320\276\321\200", Q_NULLPTR));
        CloseButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class config_manager: public Ui_config_manager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_MANAGER_H
