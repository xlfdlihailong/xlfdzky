#include "ptcp.h"


//接收一次后使用，根据使用的结构体转换
const char *ptcp::getData()
{
    //    return (char*)(&(*(this->data.begin())));
    return this->data.c_str();
}



ptcp::ptcp(int iSockAccept, string ip, int port)
{
    this->sock=iSockAccept;
    this->init(ip,port);
    this->strCtcptype="server";
}


ptcp::~ptcp()
{

}

int ptcp::init(string ip, int port)
{
    this->ip=ip;
    this->port=port;
    this->strCtcptype="init";
    return 0;
}



//默认超时时间1秒
int ptcp::connect()
{
    this->strCtcptype="client";
    int iSock=ctcp_InitTcpSocket();
    if(iSock<0)
    {
        hlog("iSock<0, connect fail");
        ctcp_CloseConnection(iSock);
        return -1;
    }

    string strRealIP;
    //通过ip中是否有字母来判断是否是域名
    if(plib::isContaiAlpha(this->ip))
        strRealIP=plib::hostToIp(this->ip.c_str());
    else
        strRealIP=this->ip;

    //默认超时时间1秒
    if(CreateConnectionTimeout(iSock,strRealIP.c_str(),port)!=0)
        //    if(CreateConnection(iSock,strRealIP.c_str(),port)!=0)
    {
        //        hlog("CreateConnection fail, connect fail");
        return -2;
    }

    this->sock=iSock;


    return this->sock;
}

int ptcp::close()
{
    return ctcp_CloseConnection(this->sock);
    //shutrdwr双向关闭
    //    return shutdown(this->sock,SHUT_RDWR);
}

plist<ptcp *> ptcp::getAllClients()
{
    return this->listclient;
}

void ptcp::deleteClientByIndex(int i)
{
    this->listclient.deleteByIndex(i);
}








typedef struct STRUCT_PARA
{
    int itype;//server类型,是int类型还是string 0标识string，1标识int,2是nopack
    ptcp* ptcpClient;
    ptcp* ptcpOriginal;
    void (*thread_user)(ptcp* para);
}STRUCT_PARA;


typedef struct STRUCT_AUXTCP_PARA
{
    void (*thread_user)(class ptcp* para);
    int itype;
    ptcp* pOriginal;
}STRUCT_AUXTCP_PARA;

void * threadTcpServerNoDetach(void* para)
{
    STRUCT_AUXTCP_PARA* psp=(STRUCT_AUXTCP_PARA*)para;
    //    hlog(psp->itype);
    //    hlog(psp->pOriginal->ip);
    //    psp->pOriginal->setThreadServer(psp->thread_user,psp->itype);
    psp->pOriginal->setThreadServer(psp->thread_user,psp->itype);
    delete psp;
    return NULL;
}
//设置多线程server，已封装内部释放等，直接获取xptcp指针,供服务端使用
//要增加，定时与客户端通信，发送探测包，如果发送失败则关闭该连接，并从统计的连接中去掉
//使用时，函数中不用再写while循环，已经封装了，直接recvp判断类型就可以了
int ptcp::setThreadServerDetach(void (*fun_thread)(ptcp *), int type)
{
    STRUCT_AUXTCP_PARA *psp=new STRUCT_AUXTCP_PARA;
    psp->itype=type;
    psp->thread_user=fun_thread;
    psp->pOriginal=this;

    std::thread thServer(threadTcpServerNoDetach,psp);
    thServer.detach();
    return 0;
}

void* thread_process_para(void* para)
{
    STRUCT_PARA* pmix=(STRUCT_PARA*)para;
    //    hlog(pmix->itype);

    hlog(pstring()<<"####### new client "<<pmix->ptcpClient->ip<<" thread id:"<<(plib::getTid())<<",now count of clients:"<<
         pmix->ptcpOriginal->getAllClients().size()<<" #########");
    //    hlog(*(pmix->ptcpOriginal));
    //    hlog(pmix->ptcpOriginal->listclient.size());

    //没有封装int类型或者string类型，纯发送接收，适合有固定接口的对外项目
    if(pmix->itype==2)
    {
        pmix->thread_user(pmix->ptcpClient);
    }
    else
    {
        while(true)
        {//执行子函数
            if(pmix->itype==0)
            {
                if(pmix->ptcpClient->recvx()<0)
                    break;
                pmix->thread_user(pmix->ptcpClient);
            }
        }
    }

    string strIpExit;
    //删除列表中的客户端
    for(int i=0;i<pmix->ptcpOriginal->listclient.size();i++)
    {
        //找到然后删除
        ptcp* pti=pmix->ptcpOriginal->listclient[i];
        if(pti->sock==pmix->ptcpClient->sock)
        {
            strIpExit=pti->ip;
            mut.lock();
            //如果提取出来再删除就没用，必须得内部删除，一定要注意
            pmix->ptcpOriginal->deleteClientByIndex(i);
            mut.unlock();
            break;
        }
    }
    hlog(pstring()<<"####### client "<<strIpExit<<" exit threadID:"
         <<(plib::getTid())<<",current client count:"
         <<pmix->ptcpOriginal->getAllClients().size()<<" #########");
    //关闭这个socket
    pmix->ptcpClient->close();
    delete (pmix->ptcpClient);
    delete (pmix);
    return NULL;
}

int ptcp::setThreadServer(void (*fun_thread)(ptcp *), int type)
{
    int iSock=ctcp_InitTcpSocket();
    if(iSock<0)
    {
        hlog("iSock<0");
        ctcp_CloseConnection(iSock);
        return -1;
    }

    int ibind=ctcp_BindSocket(iSock,this->ip.c_str(),port);
    if(ibind<0)
    {
        hlog("bind fail");
        hlog(strerror(errno));
        ctcp_CloseConnection(iSock);
        return -2;
    }
    this->sock=iSock;
    this->strCtcptype="server";
    //不停等待，来一个，创建一个线程
    while(true)
    {
        int iClientSock=-1;
        int iLength;
        struct sockaddr_in struSin;
        iLength=sizeof(struSin);
        memset(&struSin,0,iLength);
        iClientSock = accept(this->sock, (struct sockaddr *)&struSin, &iLength);

        string clientip=string(inet_ntoa(struSin.sin_addr));
        if(iClientSock<0)
        {
            //            HLOG("accept error:%s\n",strerror(errno));
            continue;
        }
        //        hlog(clientip);

        //这个地方必须要new，不然传参之后会被释放，覆盖,在线程中释放去
        STRUCT_PARA *sPara=new STRUCT_PARA;
        memset(sPara,0,sizeof(STRUCT_PARA));
        sPara->ptcpClient=new ptcp(iClientSock,clientip,this->port);

        sPara->ptcpOriginal=this;
        sPara->thread_user=fun_thread;
        sPara->itype=type;

        mut.lock();
        listclient.append((sPara->ptcpClient));
        mut.unlock();
        //        HANDLE handle = CreateThread(NULL, 0, thread_process, (LPVOID)sPara, 0, NULL);
        std::thread thServer(thread_process_para,sPara);
        thServer.detach();
    }
}







bool ptcp::operator<(const ptcp &tcp) const
{
    return this->sock<tcp.sock;
}




int ptcp::reconnect(int iretrycount, int itimesecond)
{
    //先判断是否在重连,如果在重连,说明别的线程在连,就不重连了
    if(this->bIsConnecting)
    {
        hlog("已经有线程在重连,无需重连");
        return -78;
    }
    //如果没有再重连,则置为true标示我在重连
    mut.lock();;
    this->bIsConnecting=true;
    mut.unlock();;
    string strRealIP;
    //若是域名,则先解析
    if(plib::isContaiAlpha(this->ip))
    {
        int count=0;
        while((strRealIP=plib::hostToIp(this->ip.c_str()))=="")
        {
            if(count==iretrycount)
            {
                hlog("exceed fail time limit");
                return -1;
            }
            hlog("analysis remote host fail retry");
            Sleep(itimesecond*1000);
            count++;
        }
    }
    else
        strRealIP=this->ip;

    int iSocketReconnect=-4;
    iSocketReconnect=reconnectTcp(this->sock,strRealIP.c_str(),this->port,iretrycount,itimesecond);
    if(iSocketReconnect<0)
        return -1;
    this->sock=iSocketReconnect;
    mut.lock();;
    this->bIsConnecting=false;
    mut.unlock();;
    return this->sock;
}

