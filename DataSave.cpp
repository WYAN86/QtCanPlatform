#include "DataSave.h"
#include <windows.h>
#include <QFileDialog>
#include "QsLog.h"
DataSave::DataSave(QObject *parent)
	: QThread(parent)
{}

DataSave::~DataSave()
{
    isRun = false;
    msleep(1000);
}

void DataSave::SaveData(QStringList list, int rows, QString path)
{
    dataList = list;
    this->rows = rows;
    this->filepath = path;
    this->start();
}

bool DataSave::setTitle(QString title)
{
    datalist_name = title;
    return true;
}

void DataSave::run()
{

    QList<QList<QVariant>> bb = paraString(dataList);
    writeExcelFast(this->filepath, bb);
    return;
    if (dataList.count() > 0)
    {
        if (!filepath.isEmpty()) {
            HRESULT re = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            QAxObject* excel = new QAxObject(this);
            //����Excel�ؼ�
            excel->setControl("Excel.Application");
            //����ʾ����
            excel->dynamicCall("SetVisible (bool Visible)", "false");
            //����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ
            excel->setProperty("DisplayAlerts", false);
            //��ȡ����������
            QAxObject* workbooks = excel->querySubObject("WorkBooks");
            //�½�һ��������
            workbooks->dynamicCall("Add");
            //��ȡ��ǰ������
            QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
            //��ȡ����������
            QAxObject* worksheets = workbook->querySubObject("Sheets");
            //��ȡ���������ϵĹ�����1����sheet1
            QAxObject* worksheet = worksheets->querySubObject("Item(int)", 1);




            //��ͷ
            QString str = datalist_name;
            QStringList strlist = str.split(",");


            strlist.removeAll("");
            int listCount = strlist.count();
            QString cellCol;
            for (int j = 0; j < listCount; j++)
            {
                if (j > 25)
                    cellCol = "A" + QString('A' + j - 26) + QString::number(1);          //excel���񣬵�27����AA��
                else if (j <= 25)
                    cellCol = QString('A' + j) + QString::number(1);
                else
                    return;


                QAxObject* cellRow = worksheet->querySubObject("Range(QVariant, QVariant)", cellCol);
                cellRow->dynamicCall("SetValue(const QVariant&)", QVariant(strlist[j]));


                QAxObject* font = cellRow->querySubObject("Font");  //��ȡ��Ԫ������
                font->setProperty("Bold", true);  //���õ�Ԫ������Ӵ�
                if (!isRun)
                    return;
            }




            //���ñ�������
            if (dataList.count() > 0)
                for (int i = 0; i < rows; i++) {
                    QString str = dataList.at(i);
                    QStringList strlist = str.split(",");
                    //            QString str = model->data(model->index(i),Qt::DisplayRole).toString();
                    //            QStringList strlist = str.split(" ");


                    strlist.removeAll("");
                    for (int j = 0; j < strlist.count(); j++)
                    {
                        //QString cellCol = QString('A'+j)+QString::number(i+2);
                        if (j > 25)
                            cellCol = "A" + QString('A' + j - 26) + QString::number(i + 2);    //excel���񣬵�27����AA��
                        else if (j <= 25)
                            cellCol = QString('A' + j) + QString::number(i + 2);
                        else
                            return;
                        QAxObject* cellRow = worksheet->querySubObject("Range(QVariant, QVariant)", cellCol);
                        cellRow->dynamicCall("SetValue(const QVariant&)", QVariant(strlist[j]));
                        if (!isRun)
                        {
                            workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filepath));//������filepath��ע��һ��Ҫ��QDir::toNativeSeparators��·���е�"/"ת��Ϊ"\"����Ȼһ�����治�ˡ�
                            workbook->dynamicCall("Close()");//�رչ�����
                            excel->dynamicCall("Quit()");//�ر�excel
                            delete excel;
                            excel = NULL;
                            return;
                        }
                            
                    }
                }


            workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filepath));//������filepath��ע��һ��Ҫ��QDir::toNativeSeparators��·���е�"/"ת��Ϊ"\"����Ȼһ�����治�ˡ�
            workbook->dynamicCall("Close()");//�رչ�����
            excel->dynamicCall("Quit()");//�ر�excel
            delete excel;
            excel = NULL;
        }
    }
    dataList.clear();
    rows = 0;
}
QList<QList<QVariant>> DataSave::paraString(QStringList strlists)
{
    QList<QList<QVariant>> bb;

    QList<QVariant> kk;
    
    //���ñ�������
    QStringList str = datalist_name.split(",");
    for (int m = 0; m < str.size(); m++)
        kk.append(str.at(m));
    bb.append(kk);
    for (int i = 0; i < dataList.count(); i++) {
        QString str2 = dataList.at(i);
        QStringList strlist = str2.split(",");
        QList<QVariant> aa;
        for (int k = 0; k < strlist.count(); k++)
        {
            aa.append(strlist.at(k));
        }
        int n = aa.size();
        int b = str.size();
        if (aa.size() < str.size())
        {
            for (int i = 0; i < b - n; i++)
                aa.append("--");
        }
        bb.append(aa);
    }
    return bb;
        
}
void DataSave::writeExcelFast(QString fileName, QList<QList<QVariant>>& x_y)
{
    HRESULT re = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    QAxObject* excel = new QAxObject(this);
    //����Excel�ؼ�
    excel->setControl("Excel.Application");
    //����ʾ����
    excel->dynamicCall("SetVisible (bool Visible)", "false");
    //����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ
    excel->setProperty("DisplayAlerts", false);
    //��ȡ����������
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Add");//�½�һ���������� �¹���������Ϊ�������

    QAxObject*  workbook = excel->querySubObject("ActiveWorkBook");// ��ȡ������� 
    QAxObject*  worksheet = workbook->querySubObject("Sheets(int)", 1); //��ȡ��һ������������������1

    int row = x_y.size();//����
    int col = x_y.at(x_y.size()-1).size();//����
    /*������ת����EXCEL���е���ĸ��ʽ*/
    QString rangStr;
    convert2ColName(col, rangStr);
    rangStr += QString::number(row);
    rangStr = "A1:" + rangStr;
    QLOG_INFO() << "rangStr:" << rangStr;
    QAxObject*  usedrange_Write = worksheet->querySubObject("Range(const QString&)", rangStr);

    QVariant var;
    castListListVariant2Variant(x_y, var);
    usedrange_Write->setProperty("Value", var);

    workbook->dynamicCall("SaveCopyAs(QString)", QDir::toNativeSeparators(fileName));
    workbook->dynamicCall("Close(bool)", false);  //�ر��ļ�
    excel->dynamicCall("Quit()");//�ر�excel
}

void DataSave::castListListVariant2Variant(QList<QList<QVariant>>& cells, QVariant& res)
{
    QVariantList vars;
    const int rowCount = cells.size();
    for (int i = 0; i < rowCount; ++i)
    {
        vars.append(QVariant(cells[i]));
    }
    res = QVariant(vars);
}

// brief ������ת��Ϊexcel����ĸ�к�
// param data ����0����
// return ��ĸ�кţ���1->A 26->Z 27 AA
void DataSave::convert2ColName(int data, QString& res)
{
    Q_ASSERT(data > 0 && data < 65535);
    int tempData = data / 26;
    if (tempData > 0)
    {
        int mode = data % 26;
        convert2ColName(mode, res);
        convert2ColName(tempData, res);
    }
    else
    {
        res = (to26AlphabetString(data) + res);
    }
}

// brief ����ת��Ϊ26��ĸ
// 1->A 26->Z
QString DataSave::to26AlphabetString(int data)
{
    QChar ch = data + 0x40;//A��Ӧ0x41
    return QString(ch);
}