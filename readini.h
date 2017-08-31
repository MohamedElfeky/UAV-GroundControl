/*
  ������ʽΪ  [MainKey] �����У���ȡʱ��������ǰ��ո�
  ������ʽΪ  SubKey = Value
  ��ֵValueһ����һ�У������С�
  ���Value�缸�У�����SubKey=Valueǰ���Ӽ�ֵ��  SYS_ValueLen = ��һ����ֵ��value�ĳ���  �磺
         SYS_ValueLen_strInfo = 135
         strInfo="..."
         ������ȡ��һ��ʱ�������135���ֽڲŻ�ֹͣ���ɿ���С�
         ע��SYS_ValueLen ֻ����һ������Ч.SYS_ValueLen_strInfo ֻ��SYS_ValueLen��Ч������_strInfo������
  ��д��ֵkey=����/���� ��ʽ��д���ֱ������������Ϊ�����������Զ����ӳ� ������/��������ʽ
  д���ֵ��Ҫ����savefile������savefile��true�����Զ�Ϊ���м�ֵ���Ӽ�ֵ���ȱ�ʶ��ֵ���������ݿ��Կ����
  ������ֵҪ�������ļ������ļ����ú��Զ��򿪡�setFile(QString filename,true)  �ڶ�������Ĭ��ΪtrueΪ�Զ��򿪶����ļ����ݡ�false���Ȳ����ļ���Ҫ����ReadFile()
  ���캯��ҲĬ���Զ����ļ���������Զ��򿪶�ȡ�ļ��� ReadIni(QString filename, false);

  *****************************************************************************************************************************************
   QMap<QString, QString>     StringValueMap mapValue;
             �洢���У���key---ֵvalue���б� �� =  ����+QString(tr("/"))+ �Ӽ� �������������Ӽ�Ψһȷ���б�һ�value�洢���Ӽ�������
   QMap<QString, QStringList> StringKeyMap mapKey;
             �洢MAP���� ������--���������Ӽ��б� �б�
   ɾ�������Ӽ��ͼ�ֵ��������ͬ����������MAP��ͬ�����¡�
*/


#ifndef READINI_H
#define READINI_H
#include <QString>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QStringList>




#define MAXLINELEN 1024000
typedef QMap<QString, QString> StringValueMap;
typedef QMap<QString, QStringList> StringKeyMap;

class ReadIni : public QObject
{
    public:
        ReadIni();
        ReadIni(QString filename, bool bReadFileLater = false);
        ~ReadIni();

    public:

        //��дini�ļ�
        void    setFile(QString filename, bool bReadFileLater = false );
        int     ReadFile();
        int     SaveFile(bool bCreatValueLenKey = false);  //��ֵ�����Ϊ�ļ����ļ�����գ�bCreatValueLenKey�Ƿ�Ϊÿһ�����Զ�ǿ�ƴ���������Ϣ

        //¼�롢������ɾ����ֵ
        void    setMainKey(QString strMainKey); //����Ĭ�ϵ�����������ĵ��ÿ���ʡ�Ե�����
        int     setKeyValue(QString strValue ,  QString strKey , QString strMainKey = ""  , bool bAddVerSize=false ); //д���ֵ��bAddVerSize=true ���Զ�д�볤����Ϣ
        int     getKeyValue(QString &rtValue , QString defaultValue , QString strKey , QString strMainKey = "");//���һ����ֵ
        bool    isKeyExist(QString MainKey , QString SubKey = ""); //�ж��Ƿ��иü������ڷ���true
        int     DelKey(QString MainKey , QString SubKey = "");  //ɾ������SubKey = ""��ɾ������
        void    ClearAll();   //����ڴ�����м�ֵ

        //��ü�ֵ��ĸ�����ʽ
        int     GetMainKeyStringList(QStringList &rtList);  //��������б�QStringList
        int     GetValueKeyMap(QString MainKey , StringValueMap &rtMap);//����MainKey��ֵ�µģ�����--ֵ����map
        int     GetSubKeyStringList(QString MainKey , QStringList &rtList);//����MainKey��ֵ�µĸ����б�QStringList
        int     GetMainKeyCount();                          //������������//  ~~~û��ô���Թ�������
        int     GetValueKeyCount();                         //�������и���ֵ���ܸ������������������ڣ��Ƚ�����

       //����͵�����
        void    OutPrintAll();//���������ֵ��

        //��̬�����������ַ����ַ����е�λ��
        static int Find(QString str , QChar ch, int start = 0);// �����ַ����ַ����е�λ��
        static int RevFind(QString str , QChar ch , int start = -1);//��������ַ���

private:
        int UpdateKeyMap(QString MainKey , QString SubKey ); //��ֵ����д���ͬ���ı�����-����map�б�


private:
        QString m_filename;
        QString m_MainKey;
        int m_MainKeyCount;     //��Ч�����ĸ���
        StringValueMap mapValue;//key-value
        StringKeyMap mapKey;    //mainkey-subkeylist
        QString m_Key;


};
#endif // READINI_H
