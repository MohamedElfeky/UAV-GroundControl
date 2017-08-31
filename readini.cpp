#include "readini.h"
QTextCodec *codec3=QTextCodec::codecForName("GBK");
ReadIni::ReadIni()
{

    /*
   QFile file("c:\\config.ini");
   file.open(QIODevice::ReadWrite);
   file.close();
   m_filename = "c:\\config.ini";
    */

}
ReadIni::ReadIni(QString filename, bool bReadFileLater)
{
    m_filename = filename ;
    m_MainKeyCount = 0;
    if(!bReadFileLater)ReadFile();
}


ReadIni::~ReadIni()
{

}

void ReadIni::setFile(QString filename , bool bReadFileLater)
{
    m_filename = filename ;
    if(!bReadFileLater)ReadFile();
}

void ReadIni::setMainKey(QString strMainKey)
{
        m_MainKey =  strMainKey ;
        m_Key = m_MainKey+tr("/");
}

//strValue�� ��ֵ   strKey������  strMainKey������  bAddVerSize���Ƿ�
//��ֵ���ڷ���0����ֵ�����ڷ���-1.
int ReadIni::setKeyValue ( QString strValue ,  QString strKey , QString strMainKey , bool bAddVerSize)
{
    QString key,str;
    if(strMainKey.length() > 0){
        m_MainKey=strMainKey;
        m_Key=m_MainKey+tr("/");
    }

    StringValueMap::Iterator it;

    //��¼��֮ǰ���Ӽ�ֵ��СSYS_ValueLen
    if(bAddVerSize){
        QString tmpKey,tmpStrLen,tmpSubKey;
        tmpSubKey=tr("SYS_ValueLen_")+strKey;
        tmpKey=m_MainKey+tr("/")+tmpSubKey;// ����"SYS_ValueLen_"�ļ�ֵ
        tmpStrLen=QString("%1").arg(strValue.length());
        it= mapValue.find(tmpKey);
         if(it == mapValue.end()){
            // �Ҳ�����Ӽ�ֵ��map��
            //QMessageBox::warning(NULL,"Add SYSTEM_COUNT key:["+tmpKey+"]" , "mainkey=["+m_MainKey+"]  subkey=["+tmpSubKey+ "]  value= ["+tmpStrLen+"]");
            mapValue.insert(tmpKey,tmpStrLen);//key-value ��
            UpdateKeyMap(m_MainKey,tmpSubKey);//mainkey-subkey��
         }else{
            //�м�ֵ��ֱ���滻value���޸ļ���
            mapValue.remove(tmpKey);
            mapValue.insert(tmpKey,tmpStrLen);
        }

    }//¼��SYS_ValueLen���


    //¼�����Ӽ�ֵ������
    key = m_Key+ strKey;
    //key = codec3->toUnicode(key.toLatin1());
    //strValue = codec3->toUnicode(strValue.toLatin1());
    //qDebug()<<"SetKey����  key= "<<key<<tr(" write value�� ")<<strValue;

    //QMessageBox::warning(NULL,"Add  key:["+key+"]" , "mainkey=["+m_MainKey+"]  subkey=["+strKey+ "]  value= ["+strValue+"]");
    //mapValue.begin();
    it= mapValue.find(key);
    if(it==mapValue.end()){
        // �Ҳ�����Ӽ�ֵ��map��
        //QMessageBox::warning(NULL,"Add  NEW key:["+m_Key+"]",strValue);
        mapValue.insert(key,strValue);//key-value ��
        UpdateKeyMap(m_MainKey,strKey);//mainkey-subkey��
        return 0;
    }else{
        //�м�ֵ��ֱ���滻value���޸ļ���
        //QMessageBox::warning(NULL,"REPLACE  OLD key:["+m_Key+"]",strValue);
        mapValue.remove(key);
        mapValue.insert(key,strValue);
        return -1;
    }

}

//trValue:���صļ�ֵ����keyvalue   defaultValue���Ҳ�����ֵʱ��Ĭ��ֵ   strKey������  strMainKey�� ������������ϴ�������ͬ���ɺ��ԣ�
int ReadIni::getKeyValue( QString &rtValue , QString defaultValue , QString strKey , QString strMainKey  )
{
  QString key,outStr;
    if(strMainKey.length() > 0){
        m_MainKey=strMainKey;
        m_Key = m_MainKey+tr("/");
    }
     key = m_Key+ strKey;
    // key = codec3->toUnicode(key.toLatin1());
    StringValueMap::Iterator it;
    it= mapValue.find(key);
    //�Ҳ���
    if(it==mapValue.end()){
        rtValue=defaultValue;
        //QMessageBox::warning(NULL,"not find key:["+key+"]",rtValue);
        return -1;
    }
    //�ҵ�
    rtValue=it.value();
    //rtValue = codec3->toUnicode(rtValue.toLatin1());
    //QMessageBox::warning(NULL,"read",rtValue);
    return 0;
}


int ReadIni::Find(QString str , QChar ch , int start)
{
    int len;
    int pos=0;
    QChar c;
    len=str.length();
    if( len<=0 || start >= len )return -1;
    for(pos=start ;pos<len;pos++){
        if(ch == str.at(pos))return pos;
    }
    return -1;
}
int ReadIni::RevFind(QString str , QChar ch , int start)
{
    int len;
    int pos=0;
    QChar c;
    len=str.length();
    if(start<0 )start=len-1;
    if( len<=0 || start >= len )return -1;
    for(pos = len -1 ;pos>0;pos--){
        if(ch == str.at(pos))return pos;
    }
    return -1;
}

//���ļ��������ݣ�¼�뵽��ֵ�б��С� ÿ�ζ�һ�У����û����SYS_ValueLenǰ�ü��������м���ֵ��ɾ��ǰ��ո�
//��ǰһ������SYS_ValueLen_������������ļ��ĳ��ȣ���ʱ��=�����������ǰ��ո����ȥ�������ڶ���ʱ�ո�Ҳ��ȥ����
//���󷵻�-1  �ɹ����ؼ�ֵ������������������
int ReadIni::ReadFile()
{
    QString str,strBak;              // ����ÿһ�е����ݣ�strBakΪһ�����ݣ�����ǿ缸�ж�ȡ�򲻽���ǰ��ո�ȥ��
    QString key;                     // ����+���������������������ܼ�
    QString strMainKey,strSubKey,strValue; //��������������ֵ
    int nMainKey=0,nSubKey=0;
    int nValueLen=0;
    bool longValue=false;            //��ֵ�ĸ��ӳ�����Ϣ����SYS_ValueLen_��ֵ�ṩ
    QString strOldMainKey;           //�����ı�ʱ����������Ϊһ���л���־
    QStringList subkeyList;          //�ض������µĸ����б�

    //���ļ�
    if(m_filename.length()<=0)return -1; //�ļ���Ϊ�շ��ض��ļ�ʧ��
    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly))return -1;
    QTextStream out(&file);

    //�������
    mapValue.clear();// key-value
    mapKey.clear();// mainkey-subkeylist
    subkeyList.clear();//subkeylist

    //��ֵ
    strMainKey.clear();
    strSubKey.clear();
    strValue.clear();
    strOldMainKey.clear();


    //��ʼ�����ļ�����
    do{
        str=out.readLine(MAXLINELEN );
        if(nValueLen>0)strBak=str;//�������ģʽ����ԭ�������ո�
        str=str.trimmed();

        //�и���value���ȵĿ��ܿ�Խ���У�ֱ�Ӽ���
        if( (strValue.length()<nValueLen) && (strSubKey.length()>0) ){
            strValue+=tr("\r\n");
            strValue=strValue+strBak;
            longValue=false;
            //QMessageBox::warning(NULL,"["+strMainKey+"]",strSubKey + " == "+ strValue+tr("\r\nnValueLen=%1  strLen=%2").arg(nValueLen).arg(strValue.length()));
            continue;
        }

        if(!longValue)nValueLen=0;//���ǰһ����ֵ��SYS_ValueLenָ�����ݳ��ȵĻ��������nValueLen���ȱ�־��

        //��ʾ��һ�ε�����ֵ,�ڴ˴�¼�뵽����
        if(strMainKey != strOldMainKey){
            if(subkeyList.count()>0){
                mapKey.insert(strOldMainKey,subkeyList);
                subkeyList.clear();
            }
            strOldMainKey=strMainKey;
            subkeyList.clear();
            //QMessageBox::warning(NULL,"MainKey Change to:["+strMainKey+"]",strMainKey);

        }

        //��ʾ��һ�εļ�ֵ,�ڴ˴�¼�뵽����
        if(strSubKey.length()>0){
            key=strMainKey+QString(tr("/"))+strSubKey;
            mapValue.insert(key,strValue);
            subkeyList.append(strSubKey);
            //QMessageBox::warning(NULL,"["+strMainKey+"]",strSubKey + " == "+ strValue);
            strSubKey.clear();
            strValue.clear();
        }


        if(str.length()==0)continue;  //������Ϊ���������һ��ѭ��

        if(longValue == false ) nValueLen=0;
        //�ж��Ƿ�Ϊ����[]
        if( str.at(0) == QChar('['))
        {
            nMainKey = Find(str,QChar(']'),1);
            if(nMainKey>=0){
                strMainKey=str.mid(1,nMainKey-1);
                strMainKey=strMainKey.trimmed();
                nValueLen=0;
                strSubKey.clear();
                strValue.clear();
            }
        }
        else{
            //���������ж��Ƿ񸱼�
            nSubKey = Find(str,QChar('='),1);
            if(nSubKey>=0){
                strSubKey=str.left(nSubKey).trimmed();
                if(nValueLen>0){
                    strValue=strBak.mid(Find(strBak,QChar('='),1)+1);//���Ϊ����ֵ����ȥ���ո�
                    //QMessageBox::warning(NULL,"nValueLen>0",strSubKey + " = "+ strValue + tr(" valuelen=%1").arg(strValue.length()));
                    }else strValue=str.mid(nSubKey+1).trimmed();

                if(strSubKey.left(12)==QString(tr("SYS_ValueLen"))){
                    nValueLen=strValue.toInt();
                    longValue=true;
                    strSubKey.clear();
                    strValue.clear();
                }
                //QMessageBox::warning(NULL,"Find Sub Key",strSubKey + " = "+ strValue);
            }else{
                //������Ҳ���Ӽ������Բ����������һ������
                nValueLen=0;
                strValue.clear();
                strSubKey.clear();
            }
        }

    }while(!out.atEnd());//���ļ�����

    //���һ����ֵ
    if(strSubKey.length()>0){
        key=strMainKey+QString(tr("/"))+strSubKey;
        mapValue.insert(key,strValue);
        subkeyList.append(strSubKey);
        //QMessageBox::warning(NULL,"["+strMainKey+"]",strSubKey + " == "+ strValue);
    }
    //���һ������
    if(subkeyList.count()>0){
                mapKey.insert(strOldMainKey,subkeyList);
            }

    file.close();
    m_MainKeyCount = mapKey.count();
    return mapValue.count();
}



//���̡����� bCreatValueLenKey Ϊ�Ƿ���ÿһ����ֵǰ����һ���� ��SYS_ValueLen_���� = len ����¼������Ϣ����ģʽ���Զ��м�¼
//���ش�����Ӽ�����.������������
int ReadIni::SaveFile(bool bCreatValueLenKey)
{
    QString strMainKey,strSubKey,key;
    int nWriteKeyNum=0;

    if(m_filename.length()<=0)return -1; //�ļ���Ϊ�շ��ض��ļ�ʧ��
    QFile file(m_filename);
    if(!file.open(QIODevice::WriteOnly|QFile::Truncate))return -1;
    QTextStream out(&file);

    //��ȡ�����б�������¸�����value�б�map
    QStringList rtMainKeyList;
    StringValueMap rtSubKeyList;

    int rt=GetMainKeyStringList(rtMainKeyList); //��ȡ�����б�
     for(int i=0;i<rtMainKeyList.count();i++)
    {

        strMainKey=rtMainKeyList.at(i);
        out<<"["<<strMainKey<<"]"<<"\r\n";

        rt=GetValueKeyMap(strMainKey,rtSubKeyList);
        StringValueMap::Iterator it;
        //�������������µ��Ӽ���Ŀ
        for ( it = rtSubKeyList.begin(); it != rtSubKeyList.end(); ++it ) {
            strSubKey=it.key();
            if(strSubKey.left(12)==QString(tr("SYS_ValueLen")))  continue;//"SYS_ValueLen_"�ļ�������д��
            key=strMainKey+tr("/SYS_ValueLen_")+strSubKey;// ����"SYS_ValueLen_"�ļ�ֵ
            if(!bCreatValueLenKey)
            {
                //���û��ͳһǿ��д�볤�ȼ�������
                StringValueMap::Iterator itsys;
                itsys=mapValue.find(key);                      //�����Ƿ��ж�Ӧ���"SYS_ValueLen_"��ֵ
                //QMessageBox::warning(NULL,"SaveFile() TO Find SYS_VALUELEN Key["+m_MainKey+"]",key);
                if(itsys != mapValue.end()){
                    //QMessageBox::warning(NULL,"SaveFile() Find! SYS_VALUELEN Key["+m_MainKey+"]",key);
                    //"SYS_ValueLen_"�ļ�ֵ������д��
                    out<<tr("SYS_ValueLen_")+strSubKey<<tr("=")<<itsys.value()<<tr("\r\n");
                    nWriteKeyNum++;
                }
            }else{
                //ͳһǿ��д�볤�ȼ�
                out<<tr("SYS_ValueLen_")+strSubKey << tr("=") <<it.value().length()<<tr("\r\n");
                nWriteKeyNum++;

            }
            //��"SYS_ValueLen_"��д��
             out<<it.key()<<tr("=")<<it.value()<<tr("\r\n");
             nWriteKeyNum++;

        }

    }
    out.flush();
    file.close();
    return nWriteKeyNum;
}


//������������
int ReadIni::GetMainKeyCount()
{
    return m_MainKeyCount;
}

//���ظ���ֵ���ܸ���
int ReadIni::GetValueKeyCount()
{
    return mapValue.count();
}

//������Ч�����б������������������ԡ� ���û���򷵻�-1�����򷵻�����������
//rtList�����ص������ִ��б�
int ReadIni::GetMainKeyStringList(QStringList &rtList)
{
    StringKeyMap::Iterator itkey;
    if(m_MainKeyCount<=0)return -1;
    rtList.clear();
    for ( itkey = mapKey.begin(); itkey != mapKey.end(); ++itkey ) {
         rtList.append(itkey.key());
      }
      return rtList.count();
}

//����MainKey��ֵ�µĸ����б�
int ReadIni::GetSubKeyStringList(QString MainKey , QStringList &rtList)
{
    if(mapValue.count()<=0)return -1;
    StringKeyMap::Iterator it;
    it=mapKey.find(MainKey);
    if(it != mapKey.end()){
        rtList=it.value();
        return rtList.count();
    }
    return -1;

}


//����MainKey��ֵ�µģ�����--ֵ����map,���û�з���-1������з�������
//rtMap Ϊ���ص� map�б�
int ReadIni::GetValueKeyMap(QString MainKey , StringValueMap& rtMap)
{
    if(mapValue.count()<=0)return -1;
    StringValueMap::Iterator it;
    QString key;
    rtMap.clear();
    for ( it = mapValue.begin(); it != mapValue.end(); ++it ) {
            key=it.key();
            if(key.left(MainKey.length())== MainKey){
                rtMap.insert(key.mid(MainKey.length()+1),it.value());
            }
    }
    return rtMap.count();
}



//���Ӽ�ֵ����ֵmap������
int ReadIni::UpdateKeyMap(QString MainKey , QString SubKey )
{
        QStringList lst;
        StringKeyMap::Iterator itkey;
        itkey = mapKey.find(MainKey);
        if(itkey == mapKey.end() ){
            //û�ҵ�������ֱ���������
            lst.append(SubKey);
            mapKey.insert(MainKey,lst);
            m_MainKeyCount++;
            return 1;
        }else{
            //�ҵ�������û�д˸���.��ȡ�����б���ӡ�Ȼ���ٲ��롣
            lst=itkey.value();
            lst.append(SubKey);
            mapKey.remove(MainKey);
            mapKey.insert(MainKey,lst);
            return 0;
        }
}

bool ReadIni::isKeyExist(QString MainKey , QString SubKey )
{
    StringKeyMap::Iterator it;
    QStringList rtSubKeyList;
    if(MainKey=="")return false;
    it = mapKey.find(MainKey);
    if(SubKey==""){
         if(it==mapKey.end())return false;
         return true;
    }else{
        QString key=MainKey+tr("/")+SubKey;// ����"SYS_ValueLen_"�ļ�ֵ
        if( mapValue.find(key)!= mapValue.end())return true ;
        return false;
    }
}

//ɾ����ֵ������ɾ������Ŀ����
int ReadIni::DelKey(QString MainKey , QString SubKey )
{
    //��ȡ�����б�������¸�����value�б�map
    QStringList rtMainKeyList;
    QStringList rtSubKeyList;
    QString key;

    if(!isKeyExist(MainKey , SubKey ))return 0;
    int rt=GetMainKeyStringList(rtMainKeyList); //��ȡ�����б�

    //���������б�͸��ÿһ���������ٻ�ø��������Ӽ���ֵ���б�map��ɾ��mapValue �� mapKey ���������
    for(int i=0;i<rtMainKeyList.count();i++)
    {
        QString mainkey;
        mainkey=rtMainKeyList.at(i);
        if(MainKey!=mainkey)continue;

        //�ҵ������ɾ�����з���Ҫ�����
        rt=GetSubKeyStringList(mainkey,rtSubKeyList);
        if( SubKey == "" ){
            //ɾ��һ���������������������Ӽ����ݴ�mapValue�����Ƴ�
            for ( int k=0;k<rtSubKeyList.count(); k++ ) {
                key=MainKey+QString(tr("/"))+ rtSubKeyList.at(k);//�������+�Ӽ�
                //QMessageBox::warning(NULL,"ReMoveKey() delkey=["+key+"]","subkey="+rtSubKeyList.at(k));
                mapValue.remove(key);
            }
            //�Ƴ�����
            mapKey.remove(MainKey);
            m_MainKeyCount=mapKey.count();
            //QMessageBox::warning(NULL,"ReMoveKey() delkey ["+key+"]" ,tr("total=%1").arg(rtSubKeyList.count()));
            return rtSubKeyList.count();
        }else{
            //�Ƴ��ض���ϼ�
            rtSubKeyList.removeOne(SubKey);
            mapKey.remove(MainKey);
            mapKey.insert(MainKey,rtSubKeyList);
            key=MainKey+QString(tr("/"))+ SubKey;//�������+�Ӽ�
            mapValue.remove(key);//�Ƴ�ֵ
            return 1;
        }

    }


}

//�Ի���������е������ͼ�ֵ��������
void ReadIni::OutPrintAll()
{

    //��ȡ�����б�������¸�����value�б�map
    QStringList rtMainKeyList;
    StringValueMap rtSubKeyList;

    int rt=GetMainKeyStringList(rtMainKeyList); //��ȡ�����б�

    //���������б�͸��ÿһ���������ٻ�ø��������Ӽ���ֵ���б�map
    for(int i=0;i<rtMainKeyList.count();i++)
    {
        QString mainkey;
        mainkey=rtMainKeyList.at(i);
        rt=GetValueKeyMap(mainkey,rtSubKeyList);

        StringValueMap::Iterator it;
        for ( it = rtSubKeyList.begin(); it != rtSubKeyList.end(); ++it ) {
             QMessageBox::warning(NULL,"OutPrintAll() ","MainKey=["+mainkey+"]\r\n SubKey=["+ it.key()+"] :\r\n"+ it.value()+tr("\r\nvalueLen= %1").arg(it.value().length()));
        }

    }
}

//������е��ڴ���������
void ReadIni::ClearAll()
{
      mapValue.clear();//key-value
      mapKey.clear();;    //mainkey-subkeylist
      m_MainKeyCount=0;     //��Ч�����ĸ���
}
