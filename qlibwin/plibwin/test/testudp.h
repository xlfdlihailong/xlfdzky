//
// Created by Administrator on 2020/12/21.
//

#ifndef PLIB_TESTUDP_H
#define PLIB_TESTUDP_H
#include "../pudp.h"
#include "../ptcp.h"
void testudp()
{


    pudp udp;
    udp.init(3333);
    hlog(udp.bind("224.0.0.4"));
    udp.setHostDES("224.0.0.4");




    double db=3.333;
    hlog(udp.send(db));
    pstring strdata;
    hlog(udp.recv(strdata));
    hlog(strdata.size());
    double dbrecv=*(double*)(strdata.data());
    hlog(dbrecv);
    //序列化
    pstring datatest="3";
    hlog(datatest);
    datatest.resize(4);
    strcpy((char*)datatest.data(),"xlfd");
    hlog(datatest);

    hlog(udp.send(db));
    presult res=udp.recv();
    hlog(res.res);
    double dbres=*(double*)res.data.data();
    hlog(dbres);

    hlog(udp.sendx("test",pliststring()<<"xlfd"<<"zero"));
    presult res2= udp.recvx();
    hlog(res2.res);
    pliststring lres=udp.getClassFromPresult<pliststring>(res2);
    hlog(lres);
//    hlog(udp.sendx("xxx",32.2323));
//    hlog(udp.recvx(3));
}

#endif //PLIB_TESTUDP_H
