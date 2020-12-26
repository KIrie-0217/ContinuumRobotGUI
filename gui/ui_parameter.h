/********************************************************************************
** Form generated from reading UI file 'parameter.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETER_H
#define UI_PARAMETER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout_4;
    QLabel *qHzLabel;
    QDoubleSpinBox *fc_qdDoubleSpinBox;
    QLabel *qdLabel;
    QDoubleSpinBox *fc_qddDoubleSpinBox;
    QLabel *uLabel;
    QDoubleSpinBox *fc_uDoubleSpinBox;
    QLabel *label;
    QDoubleSpinBox *fc_fextDoubleSpinBox;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *Kc0DoubleSpinBox;
    QGridLayout *gridLayout_4;
    QLabel *label_11;
    QDoubleSpinBox *Kc1DoubleSpinBox;
    QDoubleSpinBox *Kc2DoubleSpinBox;
    QLabel *label_10;
    QDoubleSpinBox *Bc2DoubleSpinBox;
    QDoubleSpinBox *Bc0DoubleSpinBox;
    QLabel *label_14;
    QDoubleSpinBox *Bc1DoubleSpinBox;
    QLabel *label_12;
    QLabel *label_13;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QDoubleSpinBox *_MrDoubleSpinBox;
    QDoubleSpinBox *mthDoubleSpinBox;
    QDoubleSpinBox *fthDoubleSpinBox;
    QLabel *label_3;
    QDoubleSpinBox *_BrDoubleSpinBox;
    QDoubleSpinBox *_BpDoubleSpinBox;
    QDoubleSpinBox *_MpDoubleSpinBox;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_4;
    QFormLayout *formLayout_5;
    QLabel *samplingPeriodLabel;
    QSpinBox *periodSpinBox;
    QLabel *feedforwardLabel;
    QCheckBox *feedforwardCheckBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(1004, 622);
        verticalLayout = new QVBoxLayout(Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(false);
        formLayout_4 = new QFormLayout(groupBox);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        qHzLabel = new QLabel(groupBox);
        qHzLabel->setObjectName(QString::fromUtf8("qHzLabel"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, qHzLabel);

        fc_qdDoubleSpinBox = new QDoubleSpinBox(groupBox);
        fc_qdDoubleSpinBox->setObjectName(QString::fromUtf8("fc_qdDoubleSpinBox"));
        fc_qdDoubleSpinBox->setAccelerated(true);
        fc_qdDoubleSpinBox->setDecimals(0);
        fc_qdDoubleSpinBox->setMinimum(1.000000000000000);
        fc_qdDoubleSpinBox->setMaximum(1000.000000000000000);
        fc_qdDoubleSpinBox->setValue(100.000000000000000);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, fc_qdDoubleSpinBox);

        qdLabel = new QLabel(groupBox);
        qdLabel->setObjectName(QString::fromUtf8("qdLabel"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, qdLabel);

        fc_qddDoubleSpinBox = new QDoubleSpinBox(groupBox);
        fc_qddDoubleSpinBox->setObjectName(QString::fromUtf8("fc_qddDoubleSpinBox"));
        fc_qddDoubleSpinBox->setDecimals(0);
        fc_qddDoubleSpinBox->setMinimum(1.000000000000000);
        fc_qddDoubleSpinBox->setMaximum(1000.000000000000000);
        fc_qddDoubleSpinBox->setValue(100.000000000000000);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, fc_qddDoubleSpinBox);

        uLabel = new QLabel(groupBox);
        uLabel->setObjectName(QString::fromUtf8("uLabel"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, uLabel);

        fc_uDoubleSpinBox = new QDoubleSpinBox(groupBox);
        fc_uDoubleSpinBox->setObjectName(QString::fromUtf8("fc_uDoubleSpinBox"));
        fc_uDoubleSpinBox->setDecimals(0);
        fc_uDoubleSpinBox->setMaximum(1000.000000000000000);
        fc_uDoubleSpinBox->setValue(100.000000000000000);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, fc_uDoubleSpinBox);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, label);

        fc_fextDoubleSpinBox = new QDoubleSpinBox(groupBox);
        fc_fextDoubleSpinBox->setObjectName(QString::fromUtf8("fc_fextDoubleSpinBox"));
        fc_fextDoubleSpinBox->setDecimals(0);
        fc_fextDoubleSpinBox->setMinimum(1.000000000000000);
        fc_fextDoubleSpinBox->setMaximum(1000.000000000000000);
        fc_fextDoubleSpinBox->setValue(100.000000000000000);

        formLayout_4->setWidget(3, QFormLayout::FieldRole, fc_fextDoubleSpinBox);


        horizontalLayout->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_3 = new QGroupBox(Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_5 = new QGridLayout(groupBox_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        Kc0DoubleSpinBox = new QDoubleSpinBox(groupBox_3);
        Kc0DoubleSpinBox->setObjectName(QString::fromUtf8("Kc0DoubleSpinBox"));
        Kc0DoubleSpinBox->setSingleStep(0.100000000000000);

        gridLayout_5->addWidget(Kc0DoubleSpinBox, 1, 1, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));

        gridLayout_5->addLayout(gridLayout_4, 0, 0, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_5->addWidget(label_11, 1, 0, 1, 1);

        Kc1DoubleSpinBox = new QDoubleSpinBox(groupBox_3);
        Kc1DoubleSpinBox->setObjectName(QString::fromUtf8("Kc1DoubleSpinBox"));
        Kc1DoubleSpinBox->setSingleStep(0.100000000000000);

        gridLayout_5->addWidget(Kc1DoubleSpinBox, 1, 2, 1, 1);

        Kc2DoubleSpinBox = new QDoubleSpinBox(groupBox_3);
        Kc2DoubleSpinBox->setObjectName(QString::fromUtf8("Kc2DoubleSpinBox"));
        Kc2DoubleSpinBox->setSingleStep(0.100000000000000);

        gridLayout_5->addWidget(Kc2DoubleSpinBox, 1, 3, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_5->addWidget(label_10, 0, 1, 1, 1);

        Bc2DoubleSpinBox = new QDoubleSpinBox(groupBox_3);
        Bc2DoubleSpinBox->setObjectName(QString::fromUtf8("Bc2DoubleSpinBox"));
        Bc2DoubleSpinBox->setSingleStep(0.010000000000000);

        gridLayout_5->addWidget(Bc2DoubleSpinBox, 2, 3, 1, 1);

        Bc0DoubleSpinBox = new QDoubleSpinBox(groupBox_3);
        Bc0DoubleSpinBox->setObjectName(QString::fromUtf8("Bc0DoubleSpinBox"));
        Bc0DoubleSpinBox->setSingleStep(0.010000000000000);

        gridLayout_5->addWidget(Bc0DoubleSpinBox, 2, 1, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_5->addWidget(label_14, 2, 0, 1, 1);

        Bc1DoubleSpinBox = new QDoubleSpinBox(groupBox_3);
        Bc1DoubleSpinBox->setObjectName(QString::fromUtf8("Bc1DoubleSpinBox"));
        Bc1DoubleSpinBox->setSingleStep(0.010000000000000);

        gridLayout_5->addWidget(Bc1DoubleSpinBox, 2, 2, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_5->addWidget(label_12, 0, 2, 1, 1);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_5->addWidget(label_13, 0, 3, 1, 1);


        horizontalLayout_2->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        _MrDoubleSpinBox = new QDoubleSpinBox(groupBox_4);
        _MrDoubleSpinBox->setObjectName(QString::fromUtf8("_MrDoubleSpinBox"));
        _MrDoubleSpinBox->setValue(1.000000000000000);

        gridLayout_2->addWidget(_MrDoubleSpinBox, 2, 2, 1, 1);

        mthDoubleSpinBox = new QDoubleSpinBox(groupBox_4);
        mthDoubleSpinBox->setObjectName(QString::fromUtf8("mthDoubleSpinBox"));
        mthDoubleSpinBox->setDecimals(1);
        mthDoubleSpinBox->setMinimum(0.100000000000000);
        mthDoubleSpinBox->setMaximum(1000.000000000000000);
        mthDoubleSpinBox->setSingleStep(0.100000000000000);

        gridLayout_2->addWidget(mthDoubleSpinBox, 4, 2, 1, 1);

        fthDoubleSpinBox = new QDoubleSpinBox(groupBox_4);
        fthDoubleSpinBox->setObjectName(QString::fromUtf8("fthDoubleSpinBox"));
        fthDoubleSpinBox->setMinimum(0.010000000000000);
        fthDoubleSpinBox->setMaximum(100.000000000000000);
        fthDoubleSpinBox->setSingleStep(0.010000000000000);

        gridLayout_2->addWidget(fthDoubleSpinBox, 4, 1, 1, 1);

        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 1, 1, 1);

        _BrDoubleSpinBox = new QDoubleSpinBox(groupBox_4);
        _BrDoubleSpinBox->setObjectName(QString::fromUtf8("_BrDoubleSpinBox"));
        _BrDoubleSpinBox->setValue(1.000000000000000);

        gridLayout_2->addWidget(_BrDoubleSpinBox, 3, 2, 1, 1);

        _BpDoubleSpinBox = new QDoubleSpinBox(groupBox_4);
        _BpDoubleSpinBox->setObjectName(QString::fromUtf8("_BpDoubleSpinBox"));
        _BpDoubleSpinBox->setDecimals(0);
        _BpDoubleSpinBox->setMinimum(1.000000000000000);
        _BpDoubleSpinBox->setMaximum(10000.000000000000000);
        _BpDoubleSpinBox->setValue(100.000000000000000);

        gridLayout_2->addWidget(_BpDoubleSpinBox, 3, 1, 1, 1);

        _MpDoubleSpinBox = new QDoubleSpinBox(groupBox_4);
        _MpDoubleSpinBox->setObjectName(QString::fromUtf8("_MpDoubleSpinBox"));
        _MpDoubleSpinBox->setSuffix(QString::fromUtf8("[1/kg]"));
        _MpDoubleSpinBox->setDecimals(1);
        _MpDoubleSpinBox->setSingleStep(0.100000000000000);
        _MpDoubleSpinBox->setValue(1.000000000000000);

        gridLayout_2->addWidget(_MpDoubleSpinBox, 2, 1, 1, 1);

        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 4, 0, 1, 1);

        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 2, 1, 1);


        horizontalLayout_2->addWidget(groupBox_4);


        verticalLayout->addLayout(horizontalLayout_2);

        formLayout_5 = new QFormLayout();
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        samplingPeriodLabel = new QLabel(Dialog);
        samplingPeriodLabel->setObjectName(QString::fromUtf8("samplingPeriodLabel"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, samplingPeriodLabel);

        periodSpinBox = new QSpinBox(Dialog);
        periodSpinBox->setObjectName(QString::fromUtf8("periodSpinBox"));
        periodSpinBox->setMinimum(10);
        periodSpinBox->setMaximum(10000);
        periodSpinBox->setSingleStep(10);
        periodSpinBox->setValue(1000);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, periodSpinBox);

        feedforwardLabel = new QLabel(Dialog);
        feedforwardLabel->setObjectName(QString::fromUtf8("feedforwardLabel"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, feedforwardLabel);

        feedforwardCheckBox = new QCheckBox(Dialog);
        feedforwardCheckBox->setObjectName(QString::fromUtf8("feedforwardCheckBox"));

        formLayout_5->setWidget(1, QFormLayout::FieldRole, feedforwardCheckBox);


        verticalLayout->addLayout(formLayout_5);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(fc_qdDoubleSpinBox, fc_qddDoubleSpinBox);
        QWidget::setTabOrder(fc_qddDoubleSpinBox, fc_uDoubleSpinBox);
        QWidget::setTabOrder(fc_uDoubleSpinBox, periodSpinBox);
        QWidget::setTabOrder(periodSpinBox, feedforwardCheckBox);
        QWidget::setTabOrder(feedforwardCheckBox, buttonBox);

        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        groupBox->setTitle(QApplication::translate("Dialog", "Cutoff Frequency", nullptr));
        qHzLabel->setText(QApplication::translate("Dialog", "q [Hz]", nullptr));
        qdLabel->setText(QApplication::translate("Dialog", "qd", nullptr));
        uLabel->setText(QApplication::translate("Dialog", "u", nullptr));
        label->setText(QApplication::translate("Dialog", "fext", nullptr));
        groupBox_3->setTitle(QApplication::translate("Dialog", "Impedance", nullptr));
        label_11->setText(QApplication::translate("Dialog", "Kc", nullptr));
        label_10->setText(QApplication::translate("Dialog", "x", nullptr));
        label_14->setText(QApplication::translate("Dialog", "Bc", nullptr));
        label_12->setText(QApplication::translate("Dialog", "y", nullptr));
        label_13->setText(QApplication::translate("Dialog", "z", nullptr));
        groupBox_4->setTitle(QApplication::translate("Dialog", "GroupBox", nullptr));
        _MrDoubleSpinBox->setSuffix(QApplication::translate("Dialog", "[1/gm^2]", nullptr));
        mthDoubleSpinBox->setSuffix(QApplication::translate("Dialog", "[mNm]", nullptr));
        fthDoubleSpinBox->setSuffix(QApplication::translate("Dialog", "[N]", nullptr));
        label_3->setText(QApplication::translate("Dialog", "Translation", nullptr));
        _BrDoubleSpinBox->setSuffix(QApplication::translate("Dialog", "[rad/smNm]", nullptr));
        _BpDoubleSpinBox->setSuffix(QApplication::translate("Dialog", "[mm/sN]", nullptr));
        _MpDoubleSpinBox->setSpecialValueText(QString());
        label_2->setText(QApplication::translate("Dialog", "1/M", nullptr));
        label_5->setText(QApplication::translate("Dialog", "1/B", nullptr));
        label_6->setText(QApplication::translate("Dialog", "fth", nullptr));
        label_4->setText(QApplication::translate("Dialog", "Rotation", nullptr));
        samplingPeriodLabel->setText(QApplication::translate("Dialog", "sampling period", nullptr));
        periodSpinBox->setSuffix(QApplication::translate("Dialog", " [ms]", nullptr));
        periodSpinBox->setPrefix(QString());
        feedforwardLabel->setText(QApplication::translate("Dialog", "feedforward", nullptr));
        feedforwardCheckBox->setText(QApplication::translate("Dialog", "feedforward", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETER_H
