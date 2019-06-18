﻿#include "infodialog.h"
#include "ui_infodialog.h"
#include "sevdevice.h"
#include "deviceidhelper.h"
#include "dbmanager.h"
#include "gtutils.h"

#include <QTableWidgetItem>
#include <QTableWidget>
#include <QDebug>

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    ui->table_Info->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_Info->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::uiInit(const QList<SevDevice*> &devList)
{
    m_devList = devList;
    for (int i = 0; i < m_devList.count(); i++) {
        DeviceIdHelper *idHelper = new DeviceIdHelper(m_devList.at(i)->socketCom(), 0);
        ui->table_Info->insertRow(i);
//        bool hasNickName = m_devList.count() > 1;
        QString prefix;
//        prefix = hasNickName?tr("[%1] ").arg(m_devList.at(i)->aliasName()):"";
        prefix = tr("[%1] ").arg(m_devList.at(i)->aliasName());
        ui->table_Info->setItem(i, COL_NAME, new QTableWidgetItem(prefix + m_devList.at(i)->modelName()));
        bool ok;
        quint32 pid = idHelper->readPwrId(ok);
        QString pwrId;
        if (ok) {
            pwrId = QString::number(pid, 10);
        } else {
            pwrId = "0";
        }
        ui->table_Info->setItem(i, COL_P, new QTableWidgetItem(pwrId));
        quint32 cid = idHelper->readCtrId(ok);
        QString ctrId;
        if (ok) {
            ctrId = QString::number(cid, 10);
        } else {
            ctrId = "0";
        }
        ui->table_Info->setItem(i, COL_C, new QTableWidgetItem(ctrId));
        QString vid;
        QString verId;
        if (m_devList.at(i)->isConnecting()) {
            vid = idHelper->readVersion(ok);
            if (ok) {
                verId = vid.remove(0, 1);
            } else {
                verId = "0";
            }
        } else {
            verId = "0";
        }
        ui->table_Info->setItem(i, COL_V, new QTableWidgetItem(verId));
        QString fpgId = idHelper->readFpgaId(ok);
        ui->table_Info->setItem(i, COL_F, new QTableWidgetItem(fpgId));
        delete idHelper;
        DBManager *dbManager = new DBManager(GTUtils::databasePath() + "Version/", "root", "");
        QStringList verList;
        verList<<ctrId<<verId<<fpgId<<pwrId;
        bool isMatch = dbManager->checkValid(verList);
        delete dbManager;
        if (isMatch) {
            ui->table_Info->setItem(i, COL_MATCH, new QTableWidgetItem(tr("Valid")));
        } else {
            ui->table_Info->setItem(i, COL_MATCH, new QTableWidgetItem(tr("Invalid")));
        }
    }
}
