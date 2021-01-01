#ifndef INTERFACECSGL
#define INTERFACECSGL
#include "qlibwin/plibwin/ptcp.h"


#define COMMENTS_LEN               128                          /*描述信息长度*/
#define FMT_TIME_LEN               20                           /*时间格式长度:yyyy-MM-dd hh:mm:ss*/
#define FILENAME_LEN               64                           /*文件名长度*/
#define RAW_FILENAME_LEN           256                          /*RawDataFile文件名长度*/
#define TASK_ID_LEN                42                           /*任务流水号长度*/
#define JOB_ID_LEN                 27                          /*作业任务编号长度*/
#define TASKARRAY_NUM_LEN          18                           /*任务单流水号长度*/
#define ROAD_ID_LEN                (JOB_ID_LEN + 6)             /*路标识长度*/
#define STAR_LEN                   20                           /*卫星名称长度*/
#define TASK_MODE_LEN              9                            /*任务作业方式长度*/
#define DEST_LEN                   6                            /*目的节点长度*/
#define SCHEID_LEN                 13                           //原始计划编号
#define MAX_ROAD_NUM               8                            /*最大路信息个数*/
#define MAX_RAWFILE_NUM           8                           /*最大原始数据传输文件个数*/
#define HTYPEGETCONF 91111111//查询conf
#define HTYPEADDCONF 91111112
#define HTYPEDELCONF 91111113
#define HTYPESETCONF 91111114
#define HTYPE_GETTASKS 0x9115//获取任务信息
#pragma pack(1)

//以下是获取传输所有服务器信息类
class TASK_HEAD {
public:
    int type;
    int length;

    TASK_HEAD() {
        this->length = 0;
    }
};

class TASK_ADDCONF {
public:
    char arrchkey[50];
    char arrchval[50];

    TASK_ADDCONF() {
        bzero(arrchkey, sizeof(arrchkey));
        bzero(arrchval, sizeof(arrchval));
    }
};

//传输通道信息
class ROAD_STATUS_STRUCT {
public:
    char arrchRoadNumber[33];            /*传输通道标识*/
    char arrchTaskSerialNumber[42];      /*任务流水号*/
    char arrchRoadFileName[64];         /*本路传输文件名*/
    int iTransferRate;                           /*传输通道实际带宽*/
    short shFinishPercent;                         /*传输通道完成百分比*/
    unsigned long long ulTransferedData;                        /*传输数据量,数据类型需要修改成unsigned long*/
    char arrchServerIP[20];             /*传输服务器IP*/
    char arrchRealStartTime[20];        /*传输实际开始时间*/
    char arrchRealEndTime[20];          /*传输实际完成时间*/
    int iTransfersTime;                          /*传输时间*/
    int iCompressRatio;                          /*传输压缩比*/
    short shRoadState;                             /*传输通道状态：参见任务状态*/
    char arrchSystem[100];                        /*当前通道的系统GF,KJ,Z3*/
    char arrchTaskInfo[256];         /*通道的备注及说明*/
    short shLink;                                  //哪条链路
    char arrchTimeLast[30];                       //上一次更新时间
    char DTCIP[30];

    bool operator<(const ROAD_STATUS_STRUCT &road) const {
        //        if(strcmp(arrchRoadNumber,road.arrchRoadNumber)<0)
        string strall = string(arrchTaskSerialNumber) + string(arrchRoadNumber);
        string strallalien = string(road.arrchTaskSerialNumber) + string(road.arrchRoadNumber);
        if (strall < strallalien)
            return true;
        return false;
    }

    friend ostream &operator<<(ostream &os, ROAD_STATUS_STRUCT x) {
        os << "arrchRoadNumber[33]: ";
        os.write(x.arrchRoadNumber, 33 * sizeof(char));
        os << " ";
        os << "arrchTaskSerialNumber[42]: ";
        os.write(x.arrchTaskSerialNumber, 42 * sizeof(char));
        os << " ";
        os << "arrchRoadFileName[64]: ";
        os.write(x.arrchRoadFileName, 64 * sizeof(char));
        os << " ";
        os << "iTransferRate: " << x.iTransferRate << " ";
        os << "shFinishPercent: " << x.shFinishPercent << " ";
        os << "ulTransferedData: " << x.ulTransferedData << " ";
        os << "arrchServerIP[20]: ";
        os.write(x.arrchServerIP, 20 * sizeof(char));
        os << " ";
        os << "arrchRealStartTime[20]: ";
        os.write(x.arrchRealStartTime, 20 * sizeof(char));
        os << " ";
        os << "arrchRealEndTime[20]: ";
        os.write(x.arrchRealEndTime, 20 * sizeof(char));
        os << " ";
        os << "iTransfersTime: " << x.iTransfersTime << " ";
        os << "iCompressRatio: " << x.iCompressRatio << " ";
        os << "shRoadState: " << x.shRoadState << " ";
        os << "arrchSystem[100]: ";
        os.write(x.arrchSystem, 100 * sizeof(char));
        os << " ";
        os << "arrchTaskInfo[256]: ";
        os.write(x.arrchTaskInfo, 256 * sizeof(char));
        os << " ";
        os << "shLink: " << x.shLink << " ";
        os << "arrchTimeLast[30]: ";
        os.write(x.arrchTimeLast, 30 * sizeof(char));
        os << " ";
        os << "DTCIP[30]: ";
        os.write(x.DTCIP, 30 * sizeof(char));
        os << " ";;
        return os;
    }

    friend istream &operator>>(istream &is, ROAD_STATUS_STRUCT &x) {
        plib::skipCountch(is, 21);
        is.read((char *) x.arrchRoadNumber, 33);
        is.get();
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchTaskSerialNumber, 42);
        is.get();
        plib::skipCountch(is, 23);
        is.read((char *) x.arrchRoadFileName, 64);
        is.get();
        plib::skipCountch(is, 15);
        is >> x.iTransferRate;
        is.get();
        plib::skipCountch(is, 17);
        is >> x.shFinishPercent;
        is.get();
        plib::skipCountch(is, 18);
        is >> x.ulTransferedData;
        is.get();
        plib::skipCountch(is, 19);
        is.read((char *) x.arrchServerIP, 20);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchRealStartTime, 20);
        is.get();
        plib::skipCountch(is, 22);
        is.read((char *) x.arrchRealEndTime, 20);
        is.get();
        plib::skipCountch(is, 16);
        is >> x.iTransfersTime;
        is.get();
        plib::skipCountch(is, 16);
        is >> x.iCompressRatio;
        is.get();
        plib::skipCountch(is, 13);
        is >> x.shRoadState;
        is.get();
        plib::skipCountch(is, 18);
        is.read((char *) x.arrchSystem, 100);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchTaskInfo, 256);
        is.get();
        plib::skipCountch(is, 8);
        is >> x.shLink;
        is.get();
        plib::skipCountch(is, 19);
        is.read((char *) x.arrchTimeLast, 30);
        is.get();
        plib::skipCountch(is, 11);
        is.read((char *) x.DTCIP, 30);
        is.get();
        return is;
    }

};


/*任务状态数据结构*/
class TASK_STATUS_STRUCT {
public:
    char arrchTaskSerialNumber[42];      /*任务流水号*/
    char arrchTaskArraySerialNumber[18]; //任务单流水号
    char arrchJobTaskID[27];              /*作业任务编号*/
    short shTaskPriority;                          //数据传输级别(0 一般 1较高 2 最高)
    short shInexecutingState;                      /*任务状态*/
    char arrchScheID[13];                 //原始计划编号
    char arrchTaskMode[9];            /*作业方式*/
    char arrchDataFileName[256];     /*原始数据文件名(T51时出现)*/
    char arrchSatelliteName[20];            /*卫星名称*/
    char arrchDataSource[6];               /*数据源地址*/
    char arrchDataDestination[6];          /*数据传输目的节点*/
    char arrchForeseeStartTime[20];     /*传输预计开始时间*/
    char arrchForeseeEndTime[20];       /*传输预计开始时间*/
    short shDestNum;                               //改过了，多目的地个数 %%非实时传输状态(3：上传OK  2：上传数据文件  1：上传DESC  0：未开始)
    short iFileExist;                              //数据文件是否存在(0：存在 1：不存在)
    char arrchFailReason[128];           //失败原因(用于判断T51源的标志和填写失败原因)
    int iRoadNumber;                             /*传输通道信息个数*/
    ROAD_STATUS_STRUCT arrstruRoadStatus[8];      /*传输通道信息*/
    short shSendDESC;                              //是否发送DESC文件()
    short shWhichLink;                             //哪个链路，0表示第一个，1表示第二个
    char arrchTaskInfo[128];             //通过第一个字节转int判断T51是否从接收站重传，已封装如下函数


    //是否从DTC开始重传
    bool isRetransFromDTC() {
        char arrchTmp[4];
        memset(arrchTmp, 0, sizeof(arrchTmp));
        memcpy(arrchTmp, this->arrchTaskInfo, 1);
        int iRet = atoi(arrchTmp);
        //1 从DTC 0从接受站
        if (iRet == 1)
            return true;
        return false;
    }

    bool operator<(const TASK_STATUS_STRUCT &t2) const {
        if (strcmp(arrchTaskSerialNumber, t2.arrchTaskSerialNumber) < 0)
            return true;
        return false;
    }

    friend ostream &operator<<(ostream &os, TASK_STATUS_STRUCT x) {
        os << "arrchTaskSerialNumber[42]: ";
        os.write(x.arrchTaskSerialNumber, 42 * sizeof(char));
        os << " ";
        os << "arrchTaskArraySerialNumber[18]: ";
        os.write(x.arrchTaskArraySerialNumber, 18 * sizeof(char));
        os << " ";
        os << "arrchJobTaskID[27]: ";
        os.write(x.arrchJobTaskID, 27 * sizeof(char));
        os << " ";
        os << "shTaskPriority: " << x.shTaskPriority << " ";
        os << "shInexecutingState: " << x.shInexecutingState << " ";
        os << "arrchScheID[13]: ";
        os.write(x.arrchScheID, 13 * sizeof(char));
        os << " ";
        os << "arrchTaskMode[9]: ";
        os.write(x.arrchTaskMode, 9 * sizeof(char));
        os << " ";
        os << "arrchDataFileName[256]: ";
        os.write(x.arrchDataFileName, 256 * sizeof(char));
        os << " ";
        os << "arrchSatelliteName[20]: ";
        os.write(x.arrchSatelliteName, 20 * sizeof(char));
        os << " ";
        os << "arrchDataSource[6]: ";
        os.write(x.arrchDataSource, 6 * sizeof(char));
        os << " ";
        os << "arrchDataDestination[6]: ";
        os.write(x.arrchDataDestination, 6 * sizeof(char));
        os << " ";
        os << "arrchForeseeStartTime[20]: ";
        os.write(x.arrchForeseeStartTime, 20 * sizeof(char));
        os << " ";
        os << "arrchForeseeEndTime[20]: ";
        os.write(x.arrchForeseeEndTime, 20 * sizeof(char));
        os << " ";
        os << "shDestNum: " << x.shDestNum << " ";
        os << "iFileExist: " << x.iFileExist << " ";
        os << "arrchFailReason[128]: ";
        os.write(x.arrchFailReason, 128 * sizeof(char));
        os << " ";
        os << "iRoadNumber: " << x.iRoadNumber << " ";
        os << "arrstruRoadStatus[8]: ";
        for (int i = 0; i < 8; i++) { os << x.arrstruRoadStatus[i] << " "; }
        os << " ";
        os << "shSendDESC: " << x.shSendDESC << " ";
        os << "shWhichLink: " << x.shWhichLink << " ";
        os << "arrchTaskInfo[128]: ";
        os.write(x.arrchTaskInfo, 128 * sizeof(char));
        os << " ";;
        return os;
    }

    friend istream &operator>>(istream &is, TASK_STATUS_STRUCT &x) {
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchTaskSerialNumber, 42);
        is.get();
        plib::skipCountch(is, 32);
        is.read((char *) x.arrchTaskArraySerialNumber, 18);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchJobTaskID, 27);
        is.get();
        plib::skipCountch(is, 16);
        is >> x.shTaskPriority;
        is.get();
        plib::skipCountch(is, 20);
        is >> x.shInexecutingState;
        is.get();
        plib::skipCountch(is, 17);
        is.read((char *) x.arrchScheID, 13);
        is.get();
        plib::skipCountch(is, 18);
        is.read((char *) x.arrchTaskMode, 9);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchDataFileName, 256);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchSatelliteName, 20);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchDataSource, 6);
        is.get();
        plib::skipCountch(is, 25);
        is.read((char *) x.arrchDataDestination, 6);
        is.get();
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchForeseeStartTime, 20);
        is.get();
        plib::skipCountch(is, 25);
        is.read((char *) x.arrchForeseeEndTime, 20);
        is.get();
        plib::skipCountch(is, 11);
        is >> x.shDestNum;
        is.get();
        plib::skipCountch(is, 12);
        is >> x.iFileExist;
        is.get();
        plib::skipCountch(is, 22);
        is.read((char *) x.arrchFailReason, 128);
        is.get();
        plib::skipCountch(is, 13);
        is >> x.iRoadNumber;
        is.get();
        plib::skipCountch(is, 22);
        for (int i = 0; i < 8; i++) {
            is >> x.arrstruRoadStatus[i];
            is.get();
        }
        is.get();
        plib::skipCountch(is, 12);
        is >> x.shSendDESC;
        is.get();
        plib::skipCountch(is, 13);
        is >> x.shWhichLink;
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchTaskInfo, 128);
        is.get();
        return is;
    }

};


class ArrstruTask {
public:
    int length;
    TASK_STATUS_STRUCT tasks[400];

    ArrstruTask() {
        this->length = 0;
        for (int i = 0; i < 400; i++) {
            bzero(&tasks[i], sizeof(TASK_STATUS_STRUCT));
        }
    }
    plist<TASK_STATUS_STRUCT> toListTask()
    {
        plist<TASK_STATUS_STRUCT> listTask;
        for (int i = 0; i < this->length; i++)
            listTask.append(this->tasks[i]);
        return listTask;
    }
};

//获取配置文件端口8888
class CSGLStation {
public:
    pstring strName;//站点名称
    pstring strip;//站点ip
    pmap<pstring, pstring> mapconf;//该站点所有配置
    pstring strPathFullConf;//配置文件路径

    ptcp *tcpc;

    CSGLStation() {
        strPathFullConf = "/opt/sc/csgl/SCService/sc.conf";
    }

    CSGLStation(pstring name, pstring strip, pstring pathconf = "/opt/sc/csgl/SCService/sc.conf") {
        this->strName = name;
        this->strip = strip;
        this->strPathFullConf = pathconf;
        this->tcpc = new ptcp(strip, 8888);
    }

    pmap<pstring, pstring> getConfMap() {
        return mapconf;
    }

    void connect() {
        (this->tcpc->connectx());
    }

    bool delConf(pstring key) {
        hlog(key);
        TASK_HEAD head;
        head.type = HTYPEDELCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv((char*)&bRes,1) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    bool setConf(pstring key, pstring val) {
        hlog(key, val);
        TASK_HEAD head;
        head.type = HTYPESETCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        strcpy(struAddConf.arrchval, val.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv((char*)&bRes,1) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    bool addConf(pstring key, pstring val) {
        hlog(key, val);
        TASK_HEAD head;
        head.type = HTYPEADDCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        strcpy(struAddConf.arrchval, val.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv((char*)&bRes,1) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    ArrstruTask getTasks() {
        ArrstruTask arrstruTaskRes;
        TASK_HEAD head;
        head.type = HTYPE_GETTASKS;
        hlog(head.length);
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return arrstruTaskRes;
        }
        hlog("send finishi");
        int ires = -1;
        if(tcpc->recv((char*)&arrstruTaskRes.length,4)<0)
        {
            plib::sleep(1000);
            return arrstruTaskRes;
        }
        hlog(arrstruTaskRes.length);
        if((ires=tcpc->recv((char*)&arrstruTaskRes.tasks,arrstruTaskRes.length*sizeof(TASK_STATUS_STRUCT)))<0)
        {
            plib::sleep(1000);
            return arrstruTaskRes;
        }
        hlog(ires);
        return arrstruTaskRes;
    }


    //获取配置到map中,用tcp形式,需要server配合
    pmap<pstring, pstring> getConfStation() {
        //先清空,不然一直都有
        this->mapconf.clear();
        //获取配置文件各个行,用xsh返回太慢,改成tcp获取
        pstring strContentConf = getConfUseTcp();
//        pstring strContentConf=plib::xsh(this->strip,"cat "+this->strPathFullConf);
//        hlog(strContentConf.contain("\0"));
//        hlog(strContentConf.size(),strContentConf.find("\0"));
//按行分割,顺便去掉空行
        pliststring listConf = strContentConf.split("\n");

//        hlog(listConf);
        //去掉带#的
        pliststring listConfNoComment;
        for (int i = 0; i < listConf.size(); i++) {
            if (!listConf[i].contain("#"))
                listConfNoComment.append(listConf[i]);
        }
//        hlog(listConfNoComment);
        //然后可以逐条解析然后赋值给map了
        for (int i = 0; i < listConfNoComment.size(); i++) {
            pstring strConfi = listConfNoComment[i];//这就是一条配置A=B
            pliststring listConfi = strConfi.split("=");
//            hlog(listConfi);
//要加个判断,因为还有[basic]这样的就一个,要略过,不然崩溃
            if (listConfi.size() == 2) {
                //                mapconf[listConfi[0]] = listConfi[1];
//                hlog(listConfi);
//                mut.lock();
                mapconf.add(listConfi[0], listConfi[1]);
//                mapconf[listConfi[0]] = listConfi[1];
//                mut.unlock();
            }
        }
        return mapconf;
//        hlog(mapconf);
//        pstring strtest=plib::toString(mapconf);
//        hlog(strtest,strtest.size(),strtest.find("\0"),string::npos);
//        hlog(plib::toClass<pmap<pstring,pstring>>(strtest));
    }

    friend ostream &operator<<(ostream &os, CSGLStation x) {
        os << "strName: " << x.strName << " " << "strip: " << x.strip << " " << "mapconf: " << x.mapconf << " "
           << "strPathFullConf: " << x.strPathFullConf << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLStation &x) {
        plib::skipCountch(is, 9);
        is >> x.strName;
        is.get();
        plib::skipCountch(is, 7);
        is >> x.strip;
        is.get();
        plib::skipCountch(is, 9);
        is >> x.mapconf;
        is.get();
        plib::skipCountch(is, 17);
        is >> x.strPathFullConf;
        is.get();
        return is;
    }

private:

    //要有服务端,超时1秒
    pstring getConfUseTcp() {
        //获取配置文件
        TASK_HEAD head;
        head.type = HTYPEGETCONF;
        head.length = 0;
        if (tcpc->send(head) < 0) {
//            hlog("send head fail");
            plib::sleep(1000);
            return "";
        }
        //等待接收配置文件
//    先接包头
        TASK_HEAD pheadRecv;
        if (tcpc->recv((char *) &pheadRecv, sizeof(TASK_HEAD), 3) < 0) {
            hlog("recv head fail");
//            plib::sleep(1000);
            return "";
        }
//        hlog(pheadRecv.length);
        char *pdataRecv = HMALLOC(char, pheadRecv.length);
        if (tcpc->recv(pdataRecv, pheadRecv.length, 3) < 0) {
//            hlog("recv data fail");
            return "";
        }
        return (pdataRecv);
    }
};

class CSGLSystem {
public:
    pstring strName;    //系统名称
    pmap<pstring, CSGLStation> mapStation;//各个站点
    CSGLSystem() {

    }

    CSGLSystem(pstring name, pmap<pstring, CSGLStation> mapStation) {
        this->strName = name;
        this->mapStation = mapStation;
    }

    CSGLStation getStationByName(pstring strNameStation) {
        return mapStation[strNameStation];
    }

    void connectAllStation() {
        for (int i = 0; i < mapStation.size(); i++) {
            mapStation.getValue(i).connect();
        }
    }

    //当前所有系统站获取配置函数
    void getConfSystem() {
        for (int i = 0; i < mapStation.size(); i++) {
            mapStation.getValue(i).getConfStation();
        }
    }

    pstring getStationNameByIndex(int index) {
        return mapStation.getKey(index);
    }

    CSGLStation &getStationByIndex(int index) {
        return mapStation.getValue(index);
    }

    size_t size() {
        return mapStation.size();
    }

    friend ostream &operator<<(ostream &os, CSGLSystem x) {
        os << "strName: " << x.strName << " " << "mapStation: " << x.mapStation << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLSystem &x) {
        plib::skipCountch(is, 9);
        is >> x.strName;
        is.get();
        plib::skipCountch(is, 12);
        is >> x.mapStation;
        is.get();
        return is;
    }

};

class CSGLAll {
public:
    pmap<pstring, CSGLSystem> mapSystem;//各个系统
    //先根据host确定个数,后边更新,是为了提高速度,后边是信息
    //先是根目录使用比,第二个是总控运行状态
    pmap<pstring, pliststring> mapInfo;

    CSGLAll() {
        pmap<pstring, CSGLStation> mapStationGF;
        mapStationGF.add("DTC", CSGLStation("DTC", "172.16.11.3"));
        mapStationGF.add("KSC", CSGLStation("KSC", "172.16.141.5"));
        mapStationGF.add("MYC", CSGLStation("MYC", "172.16.77.6"));
        mapStationGF.add("SYC", CSGLStation("SYC", "172.16.171.5"));
        mapStationGF.add("KR1", CSGLStation("KR1", "130.242.60.241"));
        mapStationGF.add("YD1", CSGLStation("YD1", "172.16.112.2"));
        mapStationGF.add("YE2", CSGLStation("YE2", "172.17.2.2"));
        CSGLSystem systemGF("高分系统", mapStationGF);

//        this->mapSystem.add("高分系统",systemGF);
        pmap<pstring, CSGLStation> mapStationXD;
        mapStationXD.add("YMY", CSGLStation("YMY", "172.16.78.2"));
        mapStationXD.add("YKS", CSGLStation("YKS", "172.16.172.1"));
        mapStationXD.add("YSY", CSGLStation("YSY", "172.16.142.1"));
        mapStationXD.add("DTC", CSGLStation("DTC", "172.16.12.3"));
        CSGLSystem systemXD("先导系统", mapStationXD);
        this->mapSystem.add("先导系统", systemXD);

        pmap<pstring, CSGLStation> mapStationKJ;
        mapStationKJ.add("DTC", CSGLStation("DTC", "172.16.14.2"));
        mapStationKJ.add("KSC", CSGLStation("KSC", "172.16.144.2"));
        mapStationKJ.add("MYC", CSGLStation("MYC", "172.16.81.2"));
        mapStationKJ.add("SYC", CSGLStation("SYC", "172.16.174.2"));
        mapStationKJ.add("KR1", CSGLStation("KR1", "130.242.60.243"));
        mapStationKJ.add("YD1", CSGLStation("YD1", "172.16.112.1"));
        mapStationKJ.add("DTCZ3", CSGLStation("DTCZ3", "172.16.13.2"));
        mapStationKJ.add("SYCZ3", CSGLStation("SYCZ3", "172.16.173.1"));
        mapStationKJ.add("KSCZ3", CSGLStation("KSCZ3", "172.16.143.1"));
        mapStationKJ.add("MYCZ3", CSGLStation("MYCZ3", "172.16.80.1"));
        mapStationKJ.add("YE2Z3", CSGLStation("YE2Z3", "172.17.2.1"));
        mapStationKJ.add("YF3",CSGLStation("YF3","172.16.120.2"));
        CSGLSystem systemKJ("空基系统", mapStationKJ);

        this->mapSystem.add("空基系统", systemKJ);

        pliststring listhost = this->gethostAll();
//        hlog(listhost);
        for (int i = 0; i < listhost.size(); i++) {
            this->mapInfo.add(listhost[i], pliststring());
        }
//        hlog(this->mapInfo);
    }

    pstring getHostBySystemAndStationName(pstring system,pstring station)
    {
        return this->getSystemByName(system).getStationByName(station).strip;
    }

    pliststring getHostsBySystemName(pstring sys)
    {
        pliststring lres;
        CSGLSystem system=this->getSystemByName(sys);
        for(int i=0;i<system.size();i++)
        {
            CSGLStation station=system.getStationByIndex(i);
            lres.append(station.strip);
        }
        return lres;
    }

    pstring getSystemNameByHost(pstring ip)
    {
        for(int i=0;i<this->size();i++)
        {
            CSGLSystem sys=this->getSystemByIndex(i);
            for(int j=0;j<sys.size();j++)
            {
                CSGLStation stat=sys.getStationByIndex(j);
                if(stat.strip==ip)
                {
                    return sys.strName;
                }
            }
        }
        return pstring();
    }
    pstring getStationNameByHost(pstring ip)
    {
        for(int i=0;i<this->size();i++)
        {
            CSGLSystem sys=this->getSystemByIndex(i);
            for(int j=0;j<sys.size();j++)
            {
                CSGLStation stat=sys.getStationByIndex(j);
                if(stat.strip==ip)
                {
                    return stat.strName;
                }
            }
        }
        return pstring();
    }
    pliststring getSystemAndStationByHost(pstring ip) {
        for (int i = 0; i < this->size(); i++) {
            CSGLSystem sys = this->getSystemByIndex(i);
            for (int j = 0; j < sys.size(); j++) {
                CSGLStation stat = sys.getStationByIndex(j);
                if (stat.strip == ip) {
                    return pliststring() << sys.strName << stat.strName;
                }
            }
        }
        return pliststring();
    }

    pliststring gethostAll() {
        pliststring lmall;
        for (int i = 0; i < this->size(); i++) {
            CSGLSystem sys = this->getSystemByIndex(i);
            for (int j = 0; j < sys.size(); j++) {
                CSGLStation station = sys.getStationByIndex(j);
                pstring strip = station.strip;
                lmall.append(strip);
            }
        }
        return lmall;
    }

    CSGLStation getStation(pstring strNameSystem, pstring strNameStation) {
        return mapSystem[strNameSystem].getStationByName(strNameStation);
    }

    CSGLStation &getStation(pstring strIpStation) {
        for (int i = 0; i < mapSystem.size(); i++) {
            CSGLSystem &systemi = this->getSystemByIndex(i);
            for (int j = 0; j < systemi.size(); j++) {
                CSGLStation &stationj = systemi.getStationByIndex(j);
                if (stationj.strip == strIpStation)
                    return stationj;
            }
        }
    }

    CSGLSystem getSystemByName(pstring strNameSystem) {
        return mapSystem[strNameSystem];
    }

    pstring getSystemNameByIndex(int index) {
        return mapSystem.getKey(index);
    }

    CSGLSystem &getSystemByIndex(int index) {
        return mapSystem.getValue(index);
    }

    size_t size() {
        return mapSystem.size();
    }

    void connectAll() {
        for (int i = 0; i < mapSystem.size(); i++) {
            mapSystem.getValue(i).connectAllStation();
        }

    }

    void getConfAll() {
        for (int i = 0; i < mapSystem.size(); i++) {
            mapSystem.getValue(i).getConfSystem();
        }
    }

    friend ostream &operator<<(ostream &os, CSGLAll x) {
        os << "mapSystem: " << x.mapSystem << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLAll &x) {
        plib::skipCountch(is, 11);
        is >> x.mapSystem;
        is.get();
        return is;
    }

};

#pragma pack()
#endif // INTERFACECSGL

