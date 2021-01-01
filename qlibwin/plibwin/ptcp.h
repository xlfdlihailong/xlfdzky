#ifndef PTCP_H
#define PTCP_H

#include "plib.h"

/*
 * 需要自己关闭socket，调用close
 * 考虑多线程的问题,一个界面如果有刷新线程,建议另来一个连接
 * 一个连接干一个事情,server端是来一个连接新建一个线程单独处理不影响
 * client端则要自己注意,一个线程干一个事情,用recvx的时候注意一下,因为recvx用到了全局变量data,不要多个线程收就ok,不然会错乱
 */
class ptcp
{
public:
    string strCtcptype;
    pstring data;
    plist<ptcp*> listclient;
    string ip;
    int port;
    int sock;
    string type;

    bool bIsConnecting=false;//判断是否在重连,防止多线程同时重连

    //三次有一次通就可以,防止vpn状态不稳不断重启
    static bool ping3()
    {
        return ping("172.16.12.1")||ping("172.16.11.3")||ping("172.16.11.6");
    }

    //直接用tcp connect22端口
    static bool ping(pstring ip)
    {
        ptcp tcpPing(ip,22);
        if(tcpPing.connect()<0)
            return false;
        return true;
    }


    const char *getData();//接收的数据，用于转换
    pstring getDataToString()
    {
        return this->data;
    }

    //因为初始化可能失败,所以吧不会失败的放到这里面,会失败的放到init中.可智能判断是否是域名,
    //若是域名,则connect和reconnect中会自动进行域名解析判断,这样是为了防止没网的时候发数据
    //但没进入到重连阶段,挂在域名解析了
    ptcp(string ip, int port)
    {
        this->init(ip,port);
    }

    ptcp()
    {

    }
    //接受连接时用,只会是server //该方法不需要init
    ptcp(int iSockAccept,string ip,int port);
    ~ptcp();//因为连接的特殊性，一般手动关闭，所以在此不释放，否则导致多次释放的问题，也不能重载<<

    int init(string ip, int port);

    //    bool bConnected()
    //    {
    //        struct tcp_info info;
    //        int len=sizeof(info);
    //        getsockopt(sock, IPPROTO_TCP, tcp_info, &info, (socklen_t *)&len);
    //        if((info.tcpi_state==TCP_ESTABLISHED))  // 则说明未断开  else 断开
    //            return true;
    //        return false;
    //    }

    //默认超时时间1秒
    int connect();//当是客户端的使用用这个
    //针对客户端，断线重连,重试次数，次数之间的间隔,默认参数代表无限重连
    //加了是否在重连判断,因此可以在任何情况下连不上就调用该函数,可以保证
    //同一时间只有一个在重连,重连的标准是发送或者接收失败,只有这两种情况
    int reconnect(int iretrycount=-1,int itimesecond=1);
    //连接失败后线程重连,不需要提示的话就这样,如果需要提示则在ui中封装
    //void MainWindow::slotThreadConnect()
    //{
    //    if(ptc->connect()<0)
    //    {
    //        sigShowStatusBar("连接服务器失败!重连中......");
    //        ptc->reconnect();
    //    }
    //    sigShowStatusBar("连接服务器成功");
    //}
    //或者做个回调函数
    //这个就包括了连接和线程重连,可以每次发送或者接收失败都调用一次connectx
    //同时reconnect中有判断,可以保证始终只有一个线程在重连
    int connectx()
    {
        //先关闭上一个,防止超时接收但是上一个连接还是正常的情况会连接越来越多
        this->close();
        if(this->connect()<0)
        {
            hlog(pstring()<<"connect"<<this->ip<<":"<<this->port<<"fail reconnecting...");
            std::thread(&ptcp::threadReconnect,this).detach();
            return -1;
        }
        hlog(pstring()<<"connect"<<this->ip<<":"<<this->port<<"success!");
        return 1;
    }
    void threadReconnect()
    {
        //        hlog(pstring()<<"连接"<<this->ip<<":"<<this->port<<"失败,重连中...");
        //如果返回-的说明已经有其他线程在重连,那么这个重连判定失败sleep1秒
        if (this->reconnect() < 0) {
            Sleep(1000);
        } else{
            hlog(pstring() << "reconnect" << this->ip << ":" << this->port << "success!");
        }
    }
    //带回调函数的connect


    //也就是关闭连接
    int close();
    //若想操作应该返回plist<ptcp *>*类型
    plist<ptcp *> getAllClients();
    //内部删除listclient用
    void deleteClientByIndex(int i);
    //设置多线程server，已封装内部释放等，直接获取xptcp指针,供服务端使用
    //要增加，定时与客户端通信，发送探测包，如果发送失败则关闭该连接，并从统计的连接中去掉
    //使用时，函数中不用再写while循环，已经封装了，直接recvp判断类型就可以了
    //只有0和2
    int setThreadServerDetach(void (*fun_thread)(ptcp* ptcp),int type=2);
    //只有0和2,0代表recvx,使用內部data,多线程会覆盖,用2的话
    //recvz,不用内部data,不会覆盖
    //带listclient
    int setThreadServer(void (*fun_thread)(ptcp* ptcp),int type=2);

    //专门为类成员函数准备的
    template <class T>
    int setThreadServerClass(void (T::*funtest)(ptcp* para),T* pthis)
    {
        int iSock=ctcp_InitTcpSocket();
        if(iSock<0)
        {
            hlog("iSock<0");
            ctcp_CloseConnection(iSock);
            return -1;
        }
        int ibind=ctcp_BindSocket(iSock,this->ip.c_str(),this->port);
        if(ibind<0)
        {
            hlog("bind fail");
            hlog(strerror(errno));
            ctcp_CloseConnection(iSock);
            return -2;
        }
        this->sock=iSock;

        //不停等待，来一个，创建一个线程
        while(true)
        {
            int iClientSock=-1;
            int iLength;
            struct sockaddr_in struSin;
            iLength=sizeof(struSin);
            memset(&struSin,0,iLength);
            iClientSock = accept(this->sock, (struct sockaddr *)&struSin, (socklen_t*)&iLength);
            if(iClientSock<0)
                continue;
            //            hlog(iClientSock);
            string clientip=string(inet_ntoa(struSin.sin_addr));
            //            hlog(clientip);
            ptcp* ptnew=new ptcp(iClientSock,clientip,this->port);

            //            std::function<void (void)> func = std::bind(funtest,pthis,ptnew);
            //            std::thread thserver(func);

            std::thread thserver(funtest,pthis,ptnew);
            //        //如果不调用,线程完了不自动释放资源
            thserver.detach();

        }
    }

    template <class T>
    void setThreadServerClassDetach(void (T::*funtest)(ptcp* para),T* pthis)
    {
        std::thread thServer(&ptcp::setThreadServerClass<T>,this,funtest,pthis);
        thServer.detach();
    }

    //要map的key使用自定义的值，则必须要重载<，const必须这样，不然不认
    bool operator<(const ptcp & tcp) const;


    //基本接收，发送
    int recv(char* acrecv, int len, int timeout=-1)
    {
        if(this->strCtcptype=="init")
        {
            hlog(pstring()<<"TCP未初始化,请先指定tcp的类型是客户端还是服务端 "<<this->ip<<":"<<this->port);
            return -999;
        }
        int ires= RecvFullPacketTimeout(this->sock,acrecv,len,timeout);
        //        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            hlog(ires);
            if(this->strCtcptype=="client")
            {
                this->connectx();
            }
        }
        return ires;
    }


    template <class T>
    int send(T t)
    {
        if(this->strCtcptype=="init")
        {
            hlog(pstring()<<"TCP未初始化,请先指定tcp的类型是客户端还是服务端 "<<this->ip<<":"<<this->port);
            return -999;
        }
        //        return tcp->send(tcp,&t,sizeof(t));
        //        return SendFullPacket(this->sock,(char*)&t,sizeof(t));
        int ires= SendFullPacketTimeout(this->sock,(char*)&t,sizeof(t),5);
        //        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            if(this->strCtcptype=="client")
            {
                this->connectx();
            }
        }
        return ires;
    }
    int send(const char* acsend,int len)
    {
        if(this->strCtcptype=="init")
        {
            hlog(pstring()<<"TCP未初始化,请先指定tcp的类型是客户端还是服务端 "<<this->ip<<":"<<this->port);
            return -999;
        }
        //    return tcp->send(tcp,acsend,len);
        int ires= SendFullPacketTimeout(this->sock,acsend,len,9);
        //        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            if(this->strCtcptype=="client")
            {
                this->connectx();
            }
        }
        return ires;
    }

    int sendstring(string strtype, string content)
    {
        return sendx(strtype,content.c_str(),content.size());
    }
    /*序列化方案
     * //send用这个,支持发送类，字符串，不支持结构体
     * 结构体直接用sendx(三参数形式)
     */
    template <class T>
    int sendx(string type,T t)//############send用这个,只适用于结构体
    {
        /*如果发的是字符串，则直接发送字符串，不转了，如果不是，则要
        //转成字符串发送，发送的字符串是全的，只有反序列化时才会出现不全
        //的情况，因此发送全用string*/
        if(typeid(t)==typeid(string)||typeid(t)==typeid(pstring))
        {
            return this->sendstring(type,*((string*)(&t)));
        }
        return this->sendstring(type,common_toString(t));
    }
    int sendx(pstring  type,const char *pdata=NULL, int length=0)
    {
        //这是为了防止这样的调用:ptcp->sendx("res","xfdsfds")
        if(pdata!=NULL&&length==0)
            return sendx(type,pdata,(string(pdata).size()));

        STRUCT_HEADER sHeader;
        memset(&sHeader,0,sizeof(STRUCT_HEADER));
        //        sHeader.type=type;
        strcpy(sHeader.arrchType,type.c_str());
        sHeader.length=length;
        pstring strall;
        strall.resize(sizeof(STRUCT_HEADER)+length);
        char* pstart=(char*)(&(*(strall.begin())));
        memcpy(pstart,&sHeader,sizeof(STRUCT_HEADER));
        //非空的话要把数据弄上去
        if(pdata!=NULL)
        {
            memcpy(pstart+sizeof(STRUCT_HEADER),pdata,length);
        }
        int iResSend=this->send(strall.c_str(),strall.size());
        if(iResSend!=strall.size())
        {
            hlog("发送数据失败，未全部发送：",iResSend,strall.size());
            return -5;
        }
        if(iResSend==sizeof(STRUCT_HEADER))
        {
            //如果只发了类型，则返回数据长度0
            return 0;
        }
        return sHeader.length;
    }
    //>=0表示接收成功==0表示只发类型
    int recvx(int timeout=-1)
    {
        STRUCT_HEADER sHeader;
        memset(&sHeader,0,sizeof(STRUCT_HEADER));
        int iresHeader=-1;
        if((iresHeader=this->recv((char*)&sHeader,sizeof(STRUCT_HEADER),timeout))<0)
        {
            hlog("###### recv header fail ####");
            //不要打印，不然会一直打印，因为如果接收不到就一直打
            return iresHeader;
        }
        //        hlog(iresHeader);
        this->type.assign(sHeader.arrchType);
        if(sHeader.length==0)
        {
            hlog("recv only type");
            return 0;
        }
        if(sHeader.length<0)
        {
            hlog("recv header len<0,fail");
            return -9;
        }

        this->data.resize(sHeader.length);
        int iRecvDataRes=this->recv((char*)(&(*(this->data.begin()))),sHeader.length,timeout);
        if(iRecvDataRes<0)
        {
             hlog(pstring()<<"recv data fail:"<<iRecvDataRes);
            return iRecvDataRes;
        }
        if(iRecvDataRes!=sHeader.length)
        {
                hlog("the len of data != len in header, recv fail");
            return -8;
        }
        return iRecvDataRes;
    }

    template <class T>
    T getClass()
    {
        //        hlog(typeid(T).name());

        //如果本来就是string，就别转了，不然出问题
        //        if(typeid(T).name()==typeid(string).name())
        //        return strdata;


        return common_toClass<T>(this->data);
    }

    int recv(pstring& strDataRecv,int len,int iTimeoutSecond=-1)
    {
        strDataRecv.resize(len);
        int iResRecv=recv((char*)strDataRecv.data(),len,iTimeoutSecond);
        return iResRecv;
    }
    presult recv(int len,int iTimeoutSecond=-1)
    {
        pstring strDataRecv;
        int ires=recv(strDataRecv,len,iTimeoutSecond);
        return presult(ires,strDataRecv);
    }

    //>=0表示接收成功==0表示只发类型
    presult recvz(int timeout=-1)
    {
        STRUCT_HEADER sHeader;
        memset(&sHeader,0,sizeof(STRUCT_HEADER));
        int iresHeader=-1;
        if((iresHeader=this->recv((char*)&sHeader,sizeof(STRUCT_HEADER),timeout))<0)
        {
            hlog("###### recv header fail ####");
            //不要打印，不然会一直打印，因为如果接收不到就一直打
            return presult(iresHeader);
        }
        //        hlog(iresHeader);
        this->type.assign(sHeader.arrchType);
        if(sHeader.length==0)
        {
            hlog("recv only type");
            return presult(0);
        }
        if(sHeader.length<0)
        {
            hlog("recv header len<0,fail");
            return presult(-9);
        }
        pstring strData;
        strData.resize(sHeader.length);
        int iRecvDataRes=this->recv((char*)strData.data(),sHeader.length,timeout);
        if(iRecvDataRes<0)
        {
            hlog(pstring()<<"recv data fail:"<<iRecvDataRes);
            return presult(iRecvDataRes);
        }
        if(iRecvDataRes!=sHeader.length)
        {
            hlog("the len of data != len in header, recv fail");
            return presult(-8);
        }
        return presult(iRecvDataRes,strData);
    }

    template <class T>
    T getClassFromPresult(presult res)
    {
        return common_toClass<T>(res.data);
    }

    template <class T>
    T getClassFromDataString(pstring strData)
    {
        return common_toClass<T>(strData);
    }

    //支持hlog，重写cout,前提是不能自动释放
    friend ostream& operator<<(ostream &os,ptcp tcp)
    {
        os<<"[ip: "<<tcp.ip<<"   port: "<<tcp.port<<"  sock: "<<tcp.sock<<"   type: "<<tcp.strCtcptype<<"]";
        if(tcp.strCtcptype=="server")
        {
            string allclients;
            if(tcp.listclient.size()>0)
            {
                allclients="  当前连接到本tcp服务的客户端如下:\n{\n";
                if(tcp.listclient.size()>0)
                {
                    for(int i=0;i<tcp.listclient.size()-1;i++)
                    {
                        allclients+=tcp.listclient[i]->ip+" ";
                    }
                    allclients+=tcp.listclient[tcp.listclient.size()-1]->ip+"\n}";
                }
                else
                    allclients+="}";
            }
            else
            {
                allclients=" 当前无客户端连接到本tcp服务";
            }
            os<<allclients;
        }
        return os;
    }

    //可打印ptcp*的，加一个就可以了！
    //支持hlog，重写cout,前提是不能自动释放
    friend ostream& operator<<(ostream &os,ptcp* tcp)
    {
        os<<"[ip: "<<tcp->ip<<"   port: "<<tcp->port<<"  sock: "<<tcp->sock<<"   type: "<<tcp->strCtcptype<<"]";
        if(tcp->strCtcptype=="server")
        {
            string allclients;
            if(tcp->listclient.size()>0)
            {
                allclients="  当前连接到本tcp服务的客户端如下:\n{\n";
                if(tcp->getAllClients().size()>0)
                {
                    for(int i=0;i<tcp->getAllClients().size()-1;i++)
                    {
                        allclients+=tcp->getAllClients()[i]->ip+" ";
                    }
                    allclients+=tcp->getAllClients()[tcp->getAllClients().size()-1]->ip+"\n}";
                }
                else
                    allclients+="}";
            }
            else
            {
                allclients=" 当前无客户端连接到本tcp服务";
            }
            os<<allclients;
        }
        return os;
    }

    void reconnect2()
    {
        while(this->connect()<0)
        {
            Sleep(1000);
        }
        hlog("reconnect2 success");
    }

private:

};


#endif // PTCP_H
