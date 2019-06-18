﻿#ifndef FLASHCLASS_H
#define FLASHCLASS_H
#include <QTreeWidget>
#include <QString>

class JsonReader;

class FlashClass : public QObject
{
    Q_OBJECT
public:
    FlashClass();
    void flash(int netId, QString hexPath, QString xmlPath, int dspNum, int axisNum, int fpgaNum, bool hexChecked, bool rpdChecked, bool xmlChecked, QObject *proBar);
signals:
    void changeBarCount(int value);
    void sendWarnMsg(QString msg);
private:
    static void updateProgessBarWhenRestoreClicked(void *arg, qint16 *value);
    static void updateProgessValueFPGA(void *arg, qint16 *value);
    static void processCallBack(void *argv, short *value);
    void delayms(quint16 ms);
    qint16 uBoot(QString ldrPath, JsonReader net, QObject *proBar);
    bool downloadHex(QObject *proBar, const QString &hexPath);
    bool downloadXmlFiles(const QString &desPath, QObject* proBar);
    bool downloadFPGA(QObject *proBar, const QString &rpdPath);
    bool deleteDir(const QString &path);
    QStringList getFilesFromExt(const QString &fileExt, const QString &filePath, int num);
private:
    QTreeWidget* mp_funcExtension;
    static int m_dspNum;
    int m_axisNum;
    int m_fpgaNum;
    static int m_step;
    static int m_progessValue;
    QString m_hexPath;
    QString m_xmlPath;
    int m_netId;
    qint16 m_netRnStation;
private slots:
    void onXmlPrmToServo(int value);
};

#endif // FLASHCLASS_H
