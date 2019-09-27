#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::initTableWidgetCoils()
{
    //Init table Coils w/ m_bitCoil[]
    ui->tableWidgetCoils->clear();
    ui->tableWidgetCoils->setRowCount(16);
    ui->tableWidgetCoils->setColumnCount(2);
    ui->tableWidgetCoils->setHorizontalHeaderLabels(QStringList() << "#" << "Modbus 1bit Coil");
    ui->tableWidgetCoils->setStyleSheet("QHeaderView::section { background-color:#EEEEEE }");
    ui->tableWidgetCoils->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetCoils->setAlternatingRowColors(true);
    ui->tableWidgetCoils->setSelectionBehavior(QAbstractItemView::SelectRows);
    //fill items
    m_bitCoil.resize(16);
    for (auto r=0; r<16; r++){
        QString sN = QString("%1").arg(r, 5, 10, QChar('0')); //adding prefix 0
        QTableWidgetItem *itemSN = new QTableWidgetItem(sN);
        itemSN->setTextAlignment(Qt::AlignCenter);
        itemSN->setFlags(itemSN->flags() &  ~Qt::ItemIsEditable);//Not editable
        ui->tableWidgetCoils->setItem( r, 0, itemSN);

        m_bitCoil[r]= r%2;
        QTableWidgetItem *itemCheck = new QTableWidgetItem(QString::number(m_bitCoil[r],2));
        itemCheck->setCheckState(m_bitCoil[r]?Qt::Checked:Qt::Unchecked);
        itemCheck->setTextAlignment(Qt::AlignCenter);
        itemCheck->setBackground(m_bitCoil[r] ? QColor("lightsteelblue") : QColor("whitesmoke"));
        itemCheck->setFlags(itemCheck->flags() &  ~Qt::ItemIsEditable);//Not editable
        ui->tableWidgetCoils->setItem( r, 1, itemCheck);
        }
    //checkbox toggling
    connect(this, SIGNAL(sigWriteCoil(int)), this, SLOT(slotWriteModbusCoil(int) ));
    QObject::connect(ui->tableWidgetCoils, &QTableWidget::itemClicked, this, [=](QTableWidgetItem * item) {
            int r = item->row(); int c = item->column();
            if (c==0) return; //nothing to do with column 0
            //qDebug() << "widget item cell clicked:"<< r<< c;
            if ((item->checkState()==Qt::Checked) && (m_bitCoil[r]==0)) {
               item->setBackground(QColor("lightsteelblue"));//blue
               m_bitCoil[r] = 1;
               emit sigWriteCoil(r);
               ui->tableWidgetCoils->item(r, c)->setText(QString::number(m_bitCoil[r],2));
               }
            else if ((item->checkState()==Qt::Unchecked) && (m_bitCoil[r]==1)) {
               item->setBackground(QColor("whitesmoke"));
               m_bitCoil[r] = 0;
               emit sigWriteCoil(r);
               ui->tableWidgetCoils->item(r, c)->setText(QString::number(m_bitCoil[r],2));
               }
            });
}

void MainWindow::initTableWidgetRegs()
{
    //Init table Regs w/ m_int16Registers
    ui->tableWidgetRegisters->clear();
    ui->tableWidgetRegisters->setRowCount(16);
    ui->tableWidgetRegisters->setColumnCount(2);
    ui->tableWidgetRegisters->setHorizontalHeaderLabels(QStringList() << "####" << "Modbus 16bits Register(Hex)");
    ui->tableWidgetRegisters->setStyleSheet("QHeaderView::section { background-color:#EEEEEE }");
    ui->tableWidgetRegisters->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetRegisters->setAlternatingRowColors(true);
    ui->tableWidgetRegisters->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_int16Registers.resize(16);
    for (auto r=0; r<16; r++){
        QString sN = QString("%1").arg(r, 5, 10, QChar('0')); //adding prefix 0
        QTableWidgetItem *itemSN = new QTableWidgetItem(sN);
        itemSN->setTextAlignment(Qt::AlignCenter);
        itemSN->setFlags(itemSN->flags() &  ~Qt::ItemIsEditable); //Not editable
        ui->tableWidgetRegisters->setItem( r, 0, itemSN);

        m_int16Registers[r]= static_cast<quint16>(r);
        QTableWidgetItem *itemReg = new QTableWidgetItem(QString::number(m_int16Registers[r], 10));
        itemReg->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetRegisters->setItem( r, 1, itemReg);
        }
    //checkbox click
    connect(this, SIGNAL(sigWriteReg(int)), this, SLOT(slotWriteModbusReg(int) ));
    QObject::connect(ui->tableWidgetRegisters, &QTableWidget::itemChanged, this, [=](QTableWidgetItem * item) {
            int r = item->row(); int c = item->column();
            if (c==0) return; //nothing to do with column 0
            //qDebug() << "widget item cell clicked:"<< r<< c;
            quint16 v16;
            bool valid;
            v16 = item->text().toUShort(&valid,16);
            if (valid) {
                m_int16Registers[r] = v16;
                emit sigWriteReg(r);
                }
            else {
                qDebug()<<item->text() << "Not a valid hex value!";
                }
            });
}
