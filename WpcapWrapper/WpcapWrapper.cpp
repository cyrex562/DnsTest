#include "stdafx.h"
#include "WpcapWrapper.h"

using namespace std;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;
using namespace System::IO;

Timeval::Timeval() {

}

PcapPktHdr::PcapPktHdr() {

}

WpcapWrapper::WpcapWrapper() {
    this->pcapInstances = gcnew Dictionary<Guid, PcapInstance^>();
    this->bpfProgramInstances = gcnew Dictionary<Guid, BpfProgram^>();
    this->dumperInstances = gcnew Dictionary<Guid, DumperInstance^>();
}

PcapStat::PcapStat() {

}

//inline pcap_t* UnMarshalPcapHandle(IntPtr inPcapHandle) {
//    return (pcap_t*)inPcapHandle.ToPointer();
//}

//inline IntPtr MarshalPcapHandle(pcap_t* inPcapHandle) {
//    return IntPtr(inPcapHandle);
//}

//Int32 WpcapWrapper::PcapLookupNet(String^ device, PcapLookupNetResult^ result) {
//    char errBuf[PCAP_ERRBUF_SIZE] = {};
//    IntPtr devicePtr = Marshal::StringToHGlobalAuto(device);
//    const char* deviceNative = (const char*)devicePtr.ToPointer();
//
//    Int32 networkNative = 0;
//    Int32 maskNative = 0;
//
//    int resultNative = pcap_lookupnet(deviceNative, &networkNative, &maskNative, errBuf);
//    if (resultNative == -1) {
//        fprintf(stderr, "failed to get network number and netmask for device: %s: %s\n", deviceNative, errBuf);
//    }
//
//    result->network = networkNative;
//    result->mask = maskNative;
//
//    return resultNative;
//}

IntPtr WpcapWrapper::PcapCreate(String^ source) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr sourcePtr = Marshal::StringToHGlobalAuto(source);
    const char* sourceNative = (const char*)sourcePtr.ToPointer();

    pcap_t* nativePcap = pcap_create(sourceNative, errBuf);
    if (nativePcap == NULL) {
        fprintf(stderr, "failed to create live capture handle for source: %s: %s\n", sourceNative, errBuf);
    }

    return MarshalPcapHandle(nativePcap);
}

//
Guid WpcapWrapper::PcapOpenLive(String^ device, Int32 snaplen, Int32 promisc, Int32 timeoutMillis) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr deviceStrPtr = Marshal::StringToHGlobalAuto(device);
    const char* deviceStrNative = (const char*)deviceStrPtr.ToPointer();

    pcap_t* nativePcap = pcap_open_live(deviceStrNative, snaplen, promisc, timeoutMillis, errBuf);
    if (nativePcap == NULL) {
        fprintf(stderr, "failed to get live pcap instance for device: %s: %s\n", deviceStrNative, errBuf);
        return Guid::Empty;
    }

    Guid pcapInstanceRef = Guid::NewGuid();
    PcapInstance^ pcapInstance = gcnew PcapInstance();
    pcapInstance->instanceId = pcapInstanceRef;
    pcapInstance->pcap = nativePcap;
    this->pcapInstances->Add(pcapInstanceRef, pcapInstance);
    return pcapInstanceRef;
}

Int32 WpcapWrapper::PcapSetSnaplen(IntPtr pcapHandle, Int32 snaplen) {
    return pcap_set_snaplen(UnMarshalPcapHandle(pcapHandle), snaplen);
}

Int32 WpcapWrapper::PcapSetPromisc(IntPtr pcapHandle, Int32 promisc) {
    return pcap_set_promisc(UnMarshalPcapHandle(pcapHandle), promisc);
}

Int32 WpcapWrapper::PcapSetTimeout(IntPtr pcapHandle, Int32 timeout) {
    return pcap_set_timeout(UnMarshalPcapHandle(pcapHandle), timeout);
}

Int32 WpcapWrapper::PcapSetTstampType(IntPtr pcapHandle, Int32 tstampType) {
    return pcap_set_tstamp_type(UnMarshalPcapHandle(pcapHandle), tstampType);
}

Int32 WpcapWrapper::PcapSetBufferSize(IntPtr pcapHandle, Int32 bufferSize) {
    return pcap_set_buffer_size(UnMarshalPcapHandle(pcapHandle), bufferSize);
}

Int32 WpcapWrapper::PcapSetTstampPrecision(IntPtr pcapHandle, Int32 tstampPrecision) {
    return pcap_set_tstamp_precision(UnMarshalPcapHandle(pcapHandle), tstampPrecision);
}

Int32 WpcapWrapper::PcapGetTstampPrecision(IntPtr pcapHandle) {
    return pcap_get_tstamp_precision(UnMarshalPcapHandle(pcapHandle));
}

Int32 WpcapWrapper::PcapActivate(IntPtr pcapHandle) {
    return pcap_activate(UnMarshalPcapHandle(pcapHandle));
}

Int32 WpcapWrapper::PcapListTstampTypes(IntPtr pcapHandle, List<PcapTstampTypes>^ tstampTypes) {
    int* tstampTypesNative = NULL;
    int count = pcap_list_tstamp_types(UnMarshalPcapHandle(pcapHandle), &tstampTypesNative);
    if (count == PCAP_ERROR) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to get list of timestamp types: %s\n", errMsg);
        return PCAP_ERROR;
    }
    
    tstampTypes = gcnew List<PcapTstampTypes>();
    for (int i = 0; i < count; i++) {
        tstampTypes->Add((PcapTstampTypes)tstampTypesNative[i]);
    }

    pcap_free_tstamp_types(tstampTypesNative);

    return count;
}

Int32 WpcapWrapper::TstampTypeNameToVal(String^ name) {
    IntPtr namePtr = Marshal::StringToHGlobalAuto(name);
    const char* nameNative = (const char*)namePtr.ToPointer();
    return pcap_tstamp_type_name_to_val(nameNative);
}

String^ WpcapWrapper::PcapTstampTypeValToName(Int32 typeVal) {
    const char* typeValNative = pcap_tstamp_type_val_to_name(typeVal);
    if (typeValNative == NULL) {
        fprintf(stderr, "failed to get name for type val: %i\n", typeVal);
        return nullptr;
    }

    String^ result = gcnew String(typeValNative);
    return result;
}

String^ WpcapWrapper::PcapTstampTypeValToDesc(Int32 typeVal) {
    const char* typeValNative = pcap_tstamp_type_val_to_description(typeVal);
    if (typeValNative == NULL) {
        fprintf(stderr, "failed to get name for type val: %i\n", typeVal);
        return nullptr;
    }

    String^ result = gcnew String(typeValNative);
    return result;
}

Int32 WpcapWrapper::PcapOpenDead(Int32 linkType, Int32 snaplen) {
    pcap_t* pcapHandleNative = pcap_open_dead(linkType, snaplen);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to get dead pcap instance");
        return -1;
    }

    PcapInstance^ pi = gcnew PcapInstance();
    pi->instanceId = Guid::NewGuid();
    pi->pcap = pcapHandleNative;
    return 0;
}

//IntPtr WpcapWrapper::PcapOpenDeadWithTstampPrecision(Int32 linkType, Int32 snaplen, UInt32 precision) {
//    pcap_t* pcapHandleNative = pcap_open_dead_with_tstamp_precision(linkType, snaplen, precision);
//    IntPtr result = MarshalPcapHandle(pcapHandleNative);
//    return result;
//}

Int32 WpcapWrapper::PcapOpenOffline(String^ fname, Guid^ pcapInstId) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();

    pcap_t* pcapHandleNative = pcap_open_offline(fnameNative, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s: %s\n", fnameNative, errBuf);
        return -1;
    }

    PcapInstance^ pi = gcnew PcapInstance();
    pi->instanceId = Guid::NewGuid();
    pi->pcap = pcapHandleNative;
    pcapInstId = pi->instanceId;
    this->pcapInstances->Add(pi->instanceId, pi);
    return 0;
}

IntPtr WpcapWrapper::PcapOpenOfflineWithTstampPrecision(String^ fname, UInt32 precision) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();

    pcap_t* pcapHandleNative = pcap_open_offline_with_tstamp_precision(fnameNative, precision, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s: %s\n", fnameNative, errBuf);
    }
    return MarshalPcapHandle(pcapHandleNative);
}

IntPtr WpcapWrapper::PcapHopenOffline(IntPtr desc) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    intptr_t ptr = (intptr_t)desc.ToPointer();
    pcap_t* pcapHandleNative = pcap_hopen_offline(ptr, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s\n", errBuf);
    }
    return MarshalPcapHandle(pcapHandleNative);
}

IntPtr WpcapWrapper::PcapHopenOfflineWithTstampPrecision(IntPtr desc, UInt32 precision) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    intptr_t ptr = (intptr_t)desc.ToPointer();
    pcap_t* pcapHandleNative = pcap_hopen_offline_with_tstamp_precision(ptr, precision, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s\n", errBuf);
    }
    return MarshalPcapHandle(pcapHandleNative);
}

Int32 WpcapWrapper::PcapClose(Guid instanceId) {
    PcapInstance^ inst = this->pcapInstances[instanceId];
    if (inst == nullptr) {
        fprintf(stderr, "Failed to get pcap instance for GUID: %s\n", instanceId.ToString());
        return -1;
    }

    pcap_close(inst->pcap);
    return 0;
}

// TODO: re-implement user parameter
// int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char * user)

Int32 WpcapWrapper::PcapLoop(Guid pcapInstId, Int32 count, PcapCallbackDelegate^ d) {
    pcap_handler cbPtr = (pcap_handler)Marshal::GetFunctionPointerForDelegate(d).ToPointer();

    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "Failed to get pcap instance for GUID: %s\n", pcapInstId.ToString());
        return -1;
    }

    Byte* userNative = NULL;
    return pcap_loop(pi->pcap, count, cbPtr, userNative);
}

Int32 WpcapWrapper::PcapDispatch(Guid instanceId, Int32 count, PcapCallbackDelegate^ d, array<Byte>^ user) {
    pcap_handler cbPtr = (pcap_handler)Marshal::GetFunctionPointerForDelegate(d).ToPointer();

    PcapInstance^ inst = this->pcapInstances[instanceId];
    if (inst == nullptr) {
        fprintf(stderr, "failed to get pcap instance for GUID: %s\n", instanceId.ToString());
        return -1;
    }

    Byte* userNative = NULL;
    if (user != nullptr) {
        userNative = (Byte*)new Byte[user->Length];
        if (userNative == nullptr) {
            fprintf(stderr, "failed to allocate buffer for userNative\n");
            return -1;
        }
        for (int i = 0; i < user->Length; i++) {
            userNative[i] = user[i];
        }
    }

    return pcap_dispatch(inst->pcap, count, cbPtr, userNative);
}

Int32 WpcapWrapper::PcapNext(Guid pcapInstId, PcapPktHdr^ pcapPktHdr, MemoryStream^ ms) {
    pcap_pkthdr* pktHdrNative = NULL;

    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "failed to get pcap instance for GUID: %s\n", pcapInstId.ToString());
        return -1;
    }

    const Byte* pktData = pcap_next(pi->pcap, pktHdrNative);
    if (pktData == NULL) {
        fprintf(stderr, "failed to get packet\n");
        return -1;
    }
    pcapPktHdr = gcnew PcapPktHdr();
    pcapPktHdr->timeVal.seconds = pktHdrNative->ts.tv_sec;
    pcapPktHdr->timeVal.microSeconds = pktHdrNative->ts.tv_usec;
    pcapPktHdr->capLen = pktHdrNative->caplen;
    pcapPktHdr->len = pktHdrNative->len;

    for (int i = 0; i < pcapPktHdr->len; i++) {
        ms->WriteByte(pktData[i]);
    }

    return 0;
}

// TODO: if array is passed in pre-allocated, specify size and check bounds before performing copy operation
Int32 WpcapWrapper::PcapNextEx(Guid pcapInstId, PcapPktHdr^ pcapPktHdr, MemoryStream^ ms) {
    pcap_pkthdr* pktHdrNative = NULL;
    const Byte* pktDataNative = NULL;

    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "failed to get pcap instance for GUID: %s\n", pcapInstId.ToString());
        return -1;
    }

    int result = pcap_next_ex(pi->pcap, &pktHdrNative, &pktDataNative);
    if (result == -1) {
        const char* err_msg = pcap_geterr(pi->pcap);
        fprintf(stderr, "failed to get packet: %s\n", err_msg);
        return result;
    }
    else if (result == 0) {
        fprintf(stderr, "timeout occurred waiting for packet to arrive\n");
        return -1;
    }
    else if (result == -2) {
        fprintf(stderr, "EOF reached\n");
        return -1;
    }

    //pcapPktHdr = gcnew PcapPktHdr();
    pcapPktHdr->timeVal.seconds = pktHdrNative->ts.tv_sec;
    pcapPktHdr->timeVal.microSeconds = pktHdrNative->ts.tv_usec;
    pcapPktHdr->capLen = pktHdrNative->caplen;
    pcapPktHdr->len = pktHdrNative->len;

    //pktData = gcnew array<Byte>(pktHdrNative->caplen);
    for (UInt32 i = 0; i < pktHdrNative->caplen; i++) {
        ms->WriteByte(pktDataNative[i]);
    }

    return result;
}

Int32 WpcapWrapper::PcapBreakLoop(Guid pcapInstanceId) {
    PcapInstance^ inst = this->pcapInstances[pcapInstanceId];
    if (inst == nullptr) {
        fprintf(stderr, "%s: failed to retrieve PCAP instance for GUID: %s\n", pcapInstanceId.ToString());
        return -1;
    }

    pcap_breakloop(inst->pcap);
    return 0;
}

Int32 WpcapWrapper::PcapStats(IntPtr pcapHandle, PcapStat^ pcapStats) {
    pcap_stat* ps = NULL;
    int result = pcap_stats(UnMarshalPcapHandle(pcapHandle), ps);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to get pcap stats: %s\n", errMsg);
        return result;
    }

    pcapStats = gcnew PcapStat();
    pcapStats->psRecv = ps->ps_recv;
    pcapStats->psDrop = ps->ps_drop;
    pcapStats->psIfDrop = ps->ps_ifdrop;
    
    return result;
}

Int32 WpcapWrapper::PcapSetFilter(IntPtr pcapHandle, IntPtr bpfProgram) {
    bpf_program* bpfProgramNative = (bpf_program*)bpfProgram.ToPointer();
    int result = pcap_setfilter(UnMarshalPcapHandle(pcapHandle), bpfProgramNative);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to set filter: %s\n", errMsg);
    }

    return result;
}

Int32 WpcapWrapper::PcapSetDirection(IntPtr pcapHandle, PcapDirection pcapDirection) {
    int result = pcap_setdirection(UnMarshalPcapHandle(pcapHandle), (pcap_direction_t)pcapDirection);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to set filter: %s\n", errMsg);
    }
    
    return result;
}

// int pcap_setnonblock(pcap_t *p, int nonblock, char *errbuf);
Int32 WpcapWrapper::PcapSetNonBlock(IntPtr pcapHandle, Int32 nonBlock) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    int result = pcap_setnonblock(UnMarshalPcapHandle(pcapHandle), nonBlock, errBuf);
    if (result == -1) {
        fprintf(stderr, "failed to set blocking state: %s\n", errBuf);
    }
    return result;
}

// int pcap_getnonblock(pcap_t *p, char *errbuf);
Int32 WpcapWrapper::PcapGetNonBLock(Guid pcapInstId) {
    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "Failed to get PCAP instance for GUID: %s\n", pcapInstId.ToString());
        return -1;
    }

    char errBuf[PCAP_ERRBUF_SIZE] = {};
    int nonBlockState = pcap_getnonblock(pi->pcap, errBuf);
    if (nonBlockState == -1) {
        fprintf(stderr, "failed to get blocking state: %s\n", errBuf);
    }
    return nonBlockState;
}

// const char *pcap_statustostr(int error);
String^ WpcapWrapper::PcapStatusToStr(Int32 error) {
    const char* statusStrNative = pcap_statustostr(error);
    String^ result = gcnew String(statusStrNative);
    return result;
}

// int pcap_inject(pcap_t *p, const void *buf, size_t size);
Int32 WpcapWrapper::PcapInject(IntPtr pcapHandle, array<Byte>^ packet) {
    UInt32 pktBufSz = (UInt32)packet->Length;
    Byte* pktBuf = (Byte*)malloc(pktBufSz);
    if (pktBuf == NULL) {
        fprintf(stderr, "Failed to allocate buffer\n");
        return -1;
    }
    ZeroMemory(pktBuf, pktBufSz);

    for (UInt32 i = 0; i < pktBufSz; i++) {
        pktBuf[i] = packet[i];
    }

    int result = pcap_inject(UnMarshalPcapHandle(pcapHandle), pktBuf, pktBufSz);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to inject packet :%s\n", errMsg);
    }

    free(pktBuf);
    pktBuf = NULL;
    return result;
}

// int pcap_sendpacket(pcap_t *p, const u_char *buf, int size);
Int32 WpcapWrapper::PcapSendPacket(Guid pcapInstId, array<Byte>^ packet) {
    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "failed to get pcap instance for GUID %s\n", pcapInstId.ToString());
        return -1;
    }

    Byte* pktBuf = new Byte[packet->Length];
    if (pktBuf == NULL) {
        fprintf(stderr, "Failed to allocate buffer\n");
        return -1;
    }
    for (int i = 0; i < packet->Length; i++) {
        pktBuf[i] = packet[i];
    }

    int result = pcap_sendpacket(pi->pcap, pktBuf, packet->Length);
    if (result == -1) {
        const char* errMsg = pcap_geterr(pi->pcap);
        fprintf(stderr, "failed to send packet :%s\n", errMsg);

    }

    delete pktBuf;
    pktBuf = NULL;
    return result;
}

// int pcap_compile(pcap_t *p, struct bpf_program *fp, const char *str, int optimize, bpf_u_int32 netmask);
Int32 WpcapWrapper::PcapCompile(Guid instanceId, Guid^ bpfProgramId, String^ filterStr, Int32 optimize, UInt32 netmask) {
    IntPtr filterStrPtr = Marshal::StringToHGlobalAuto(filterStr);
    const char* filterStrNative = (const char*)filterStrPtr.ToPointer();

    PcapInstance^ inst = this->pcapInstances[instanceId];
    if (inst == nullptr) {
        fprintf(stderr, "failed to get pcap instance for GUID %s\n", instanceId.ToString());
        return -1;
    }

    bpf_program* fp = NULL;
    int result = pcap_compile(inst->pcap, fp, filterStrNative, optimize, netmask);
    if (result == -1) {
        fprintf(stderr, "failed to compile filter: %s\n", pcap_geterr(inst->pcap));
        return result;
    }

    Guid programId = Guid::NewGuid();
    BpfProgram^ prog = gcnew BpfProgram();
    prog->programId = programId;
    prog->program = fp;
    this->bpfProgramInstances->Add(programId, prog);

    bpfProgramId = programId;

    return  result;
}

Int32 WpcapWrapper::PcapCompileNoPcap(Int32 snaplen, Int32 linkType, Guid^ bpfProgramId, String^ filterStr, Int32 optimize, UInt32 mask) {
    IntPtr filterStrPtr = Marshal::StringToHGlobalAuto(filterStr);
    const char* filterStrNative = (const char*)filterStrPtr.ToPointer();

    bpf_program* fp = NULL;
    int result = pcap_compile_nopcap(snaplen, linkType, fp, filterStrNative, optimize, mask);
    if (result == -1) {
        return result;
    }

    Guid programId = Guid::NewGuid();
    BpfProgram^ prog = gcnew BpfProgram();
    prog->programId = programId;
    prog->program = fp;

    bpfProgramId = programId;

    return  result;
}

Int32 WpcapWrapper::PcapCreateSrcStr(String^ src, Int32 type, String^ host, String^ port, String^ name) {
    /*IntPtr srcStrPtr = Marshal::StringToHGlobalAuto(src);
    const char* srcStrNative = (const char*)srcStrPtr.ToPointer();*/
    IntPtr hostStrPtr = Marshal::StringToHGlobalAuto(host);
    const char* hostStrNative = (const char*)hostStrPtr.ToPointer();
    IntPtr portStrPtr = Marshal::StringToHGlobalAuto(port);
    const char* portStrNative = (const char*)portStrPtr.ToPointer();
    IntPtr nameStrPtr = Marshal::StringToHGlobalAuto(name);
    const char* nameStrNative = (const char*)nameStrPtr.ToPointer();
    char srcStrNative[PCAP_BUF_SIZE] = {};
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    int result = pcap_createsrcstr(srcStrNative, type, hostStrNative, portStrNative, nameStrNative, errBuf);
    if (result == -1) {
        fprintf(stderr, "failed to create source string: %s\n", errBuf);
    }
    return result;
}

Int32 WpcapWrapper::PcapFreeCode(Guid bpfInstId) {
    BpfProgram^ bp = this->bpfProgramInstances[bpfInstId];
    if (bp == nullptr) {
        fprintf(stderr, "failed to get BpfProgram for GUID %s\n", bpfInstId.ToString());
        return -1;
    }

    pcap_freecode(bp->program);
    return 0;
}

// int pcap_offline_filter(const struct bpf_program *fp, const struct pcap_pkthdr *h, const u_char *pkt)
Int32 WpcapWrapper::PcapOfflineFilter(Guid bpfInstId, PcapPktHdr^ pktHdr, array<Byte>^ pkt) {
    BpfProgram^ bp = this->bpfProgramInstances[bpfInstId];
    if (bp == nullptr) {
        fprintf(stderr, "failed to get BpfProgram for GUID %s\n", bpfInstId.ToString());
        return -1;
    }

    pcap_pkthdr pktHdrNative = {};
    pktHdrNative.caplen = pktHdr->capLen;
    pktHdrNative.len = pktHdr->len;
    pktHdrNative.ts.tv_sec = pktHdr->timeVal.seconds;
    pktHdrNative.ts.tv_usec = pktHdr->timeVal.microSeconds;

    Byte* pktNative = new Byte[pktHdrNative.caplen];
    if (pktNative == NULL) {
        fprintf(stderr, "failed to allocate buffer for packet\n");
        return -1;
    }
    for (UInt32 i = 0; i < pktHdrNative.caplen; i++) {
        pktNative[i] = pkt[i];
    }

    int result = pcap_offline_filter(bp->program, &pktHdrNative, pktNative);
    
    delete pktNative;
    pktNative = NULL;

    return result;
}

// int pcap_datalink(pcap_t *p);
Int32 WpcapWrapper::PcapDatalink(Guid instanceId) {
    PcapInstance^ inst = this->pcapInstances[instanceId];
    if (inst == nullptr) {
        fprintf(stderr, "failed to get instance for GUID: %s\n", instanceId.ToString());
        return -1;
    }
    return pcap_datalink(inst->pcap);
}

// int pcap_list_datalinks(pcap_t *p, int **dlt_buf);
Int32 WpcapWrapper::PcapListDatalinks(Guid pcapInstId, List<Int32>^ datalinks) {
    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "failed to get instance for GUID: %s\n", instanceId.ToString());
        return -1;
    }
    
    int* datalinksNative = NULL;
    int result = pcap_list_datalinks(pi->pcap, &datalinksNative);
    if (result == -1) {
        fprintf(stderr, "failed to get list of datalinks: %s\n", pcap_geterr(pi->pcap));
        return result;
    }

    datalinks = gcnew List<Int32>();
    for (int i = 0; i < result; i++) {
        datalinks->Add(datalinksNative[i]);
    }

    pcap_free_datalinks(datalinksNative);

    return result;
}

// int pcap_set_datalink(pcap_t *p, int dlt);
Int32 WpcapWrapper::PcapSetDatalink(IntPtr pcapHandle, Int32 datalinkType) {
    return pcap_set_datalink(UnMarshalPcapHandle(pcapHandle), datalinkType);
}

// int pcap_datalink_name_to_val(const char *name);
Int32 WpcapWrapper::PcapDatalinkNameToVal(String^ name) {
    IntPtr namePtr = Marshal::StringToHGlobalAuto(name);
    const char* nameNative = (const char*)namePtr.ToPointer();
    return pcap_datalink_name_to_val(nameNative);
}

// TODO: test to ensure string passage works
String^ WpcapWrapper::PcapDatalinkValToName(Int32 datalinkType) {
    const char* result = pcap_datalink_val_to_name(datalinkType);
    if (result == NULL) {
        fprintf(stderr, "failed to convert datalink type to name\n");
        return nullptr;
    }
    String^ resultManaged = gcnew String(result);
    return resultManaged;
}

// TODO: test to ensure string passage works
String^ WpcapWrapper::PcapDatalinkValToDescription(Int32 datalinkType) {
    const char* result = pcap_datalink_val_to_description(datalinkType);
    if (result == NULL) {
        fprintf(stderr, "failed to convert datalink type to name\n");
        return nullptr;
    }
    String^ resultManaged = gcnew String(result);
    return resultManaged;
}

// int pcap_snapshot(pcap_t *p);
Int32 WpcapWrapper::PcapSnapshot(IntPtr pcapHandle) {
    return pcap_snapshot(UnMarshalPcapHandle(pcapHandle));
}

// int pcap_is_swapped(pcap_t *p);
Int32 WpcapWrapper::PcapIsSwapped(Guid pcapInstId) {
    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "Failed to get PCAP instance for GUID: %s\n", pcapInstId.ToString());
        return -1;
    }
    return pcap_is_swapped(pi->pcap);
}

// int pcap_major_version(pcap_t *p);
Int32 WpcapWrapper::PcapMajorVersion(Guid pcapInstId) {
    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "Failed to get PCAP instance for GUID: %s\n", pcapInstId.ToString());
        return -1;
    }
    return pcap_major_version(pi->pcap);
}

// int pcap_minor_version(pcap_t *p);
Int32 WpcapWrapper::PcapMinorVersion(Guid pcapInstId) {
    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "Failed to get PCAP instance for GUID: %s\n", pcapInstId.ToString());
        return -1;
    }
    return pcap_minor_version(pi->pcap);
}

// deprecated
// FILE *pcap_file(pcap_t *p);
//IntPtr WpcapWrapper::PcapFile(IntPtr pcapHandle) {
//    FILE* fp = pcap_file(UnMarshalPcapHandle(pcapHandle));
//    return IntPtr(fp);
//}

// pcap_dumper_t *pcap_dump_open(pcap_t *p, const char *fname);
Int32 WpcapWrapper::PcapDumpOpen(Guid pcapInstId, String^ fname, Guid^ dumperInstId) {
    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();

    PcapInstance^ pi = this->pcapInstances[pcapInstId];
    if (pi == nullptr) {
        fprintf(stderr, "failed to get PCAP instance for guid %s\n", pcapInstId.ToString());
        return -1;
    }
    
    
    pcap_dumper_t* pdt = pcap_dump_open(pi->pcap, fnameNative);
    if (pdt == NULL) {
        fprintf(stderr, "failed to open file %s for writing packets: %s\n", fnameNative, pcap_geterr(pi->pcap));
    }

    DumperInstance^ di = gcnew DumperInstance();
    di->dumper = pdt;
    di->dumperId = Guid::NewGuid();
    this->dumperInstances->Add(di->dumperId, di);
    return 0;
}

IntPtr WpcapWrapper::PcapDumpOpenAppend(IntPtr pcapHandle, String^ fname) {
    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();
    pcap_dumper_t* pdt = pcap_dump_open_append(UnMarshalPcapHandle(pcapHandle), fnameNative);
    if (pdt == NULL) {
        fprintf(stderr, "failed to open file %s for writing packets: %s\n", fnameNative, pcap_geterr(UnMarshalPcapHandle(pcapHandle)));
    }
    return IntPtr(pdt);
}

// pcap_dumper_t *pcap_dump_fopen(pcap_t *p, FILE *fp);
IntPtr WpcapWrapper::PcapDumpFopen(IntPtr pcapHandle, IntPtr fp) {
    FILE* fpNative = (FILE*)fp.ToPointer();
    pcap_dumper_t* pdt = pcap_dump_fopen(UnMarshalPcapHandle(pcapHandle), fpNative);
    if (pdt == NULL) {
        fprintf(stderr, "failed to open file for writing packets: %s\n", pcap_geterr(UnMarshalPcapHandle(pcapHandle)));
    }
    return IntPtr(pdt);
}

// FILE *pcap_dump_file(pcap_dumper_t *p);
Int32 WpcapWrapper::PcapDumpFile(Guid instanceId, Guid^ fileInstanceId) {
    DumperInstance^ instance = this->dumperInstances[instanceId];
    if (instance == nullptr) {
        fprintf(stderr, "failed to get dumper instance for GUID %s\n", instanceId);
        return -1;
    }

    FILE* fp = pcap_dump_file(instance->dumper);
    if (fp == NULL) {
        fprintf(stderr, "failed to open I/O stream for writing savefile\n");
        return -1;
    }

    FileInstance^ fi = gcnew FileInstance();
    fi->fileId = Guid::NewGuid();
    fi->fp = fp;
    this->fileInstances->Add(fi->fileId, fi);
    fileInstanceId = fi->fileId;
    return 0;
}

// long pcap_dump_ftell(pcap_dumper_t *p);
Int32 WpcapWrapper::PcapDumpFtell(Guid instanceId) {
    DumperInstance^ instance = this->dumperInstances[instanceId];
    if (instance == nullptr) {
        fprintf(stderr, "failed to get dumper instance for GUID %s\n", instanceId);
        return -1;
    }

    return pcap_dump_ftell(instance->dumper);
}

// void pcap_dump_close(pcap_dumper_t *p);
Int32 WpcapWrapper::PcapDumpClose(Guid instanceId) {
    DumperInstance^ instance = this->dumperInstances[instanceId];
    if (instance == nullptr) {
        fprintf(stderr, "failed to get dumper instance for GUID %s\n", instanceId);
        return -1;
    }

    pcap_dump_close(instance->dumper);
    return 0;
}

// int pcap_dump_flush(pcap_dumper_t *p);
Int32 WpcapWrapper::PcapDumpFlush(Guid instanceId) {
    DumperInstance^ instance = this->dumperInstances[instanceId];
    if (instance == nullptr) {
        fprintf(stderr, "failed to get dumper instance for GUID %s\n", instanceId);
        return -1;
    }
    return pcap_dump_flush(instance->dumper);
}

// void pcap_dump(u_char *user, struct pcap_pkthdr *h, u_char *sp);
Int32 WpcapWrapper::PcapDump(Guid instanceId, PcapPktHdr^ pktHdr, array<Byte>^ pkt) {
    pcap_pkthdr pktHdrNative = {};
    pktHdrNative.caplen = pktHdr->capLen;
    pktHdrNative.len = pktHdr->len;
    pktHdrNative.ts.tv_sec = pktHdr->timeVal.seconds;
    pktHdrNative.ts.tv_usec = pktHdr->timeVal.microSeconds;
    
    DumperInstance^ inst = this->dumperInstances[instanceId];
    if (inst == nullptr) {
        fprintf(stderr, "failed to get dumper instance for GUID %s\n", instanceId.ToString());
        return -1;
    }

    Byte* pktNative = new Byte[pktHdr->capLen];
    if (pktNative == nullptr) {
        fprintf(stderr, "failed to allocate buffer for pktNative\n");
        return -1;
    }
    for (UInt32 i = 0; i < pktHdr->capLen; i++) {
        pktNative[i] = pkt[i];
    }

    pcap_dump((Byte*)inst->dumper, &pktHdrNative, pktNative);

    delete[] pktNative;
    return 0;
}

// int pcap_findalldevs(pcap_if_t **alldevsp, char *errbuf);
Int32 WpcapWrapper::PcapFindAllDevs(List<PcapDeviceInfo^>^ devices) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    pcap_if_t* devicesNative = NULL;
    int result = pcap_findalldevs(&devicesNative, errBuf);
    if (result == -1) {
        fprintf(stderr, "failed to get list of devices: %s\n", errBuf);
        return -1;
    }

    //devices = gcnew List<PcapDeviceInfo^>();

    pcap_if_t* currentDevice = devicesNative;
    while (currentDevice != NULL) {
        PcapDeviceInfo^ devInfo = gcnew PcapDeviceInfo();
        devInfo->name = gcnew String(currentDevice->name);
        devInfo->description = nullptr;
        if (currentDevice->description != NULL) {
            devInfo->description = gcnew String(currentDevice->description);
        }
        devInfo->addresses = gcnew List<PcapAddrInfo^>();
        
        pcap_addr_t* currentAddr = currentDevice->addresses;
        while (currentAddr != NULL) {
            PcapAddrInfo^ addrInfo = gcnew PcapAddrInfo();
            addrInfo->address = gcnew SockAddr();
            addrInfo->address->addressFamily = currentAddr->addr->sa_family;
            addrInfo->address->addressData = gcnew array<Byte>(14);
            for (int i = 0; i < 14; i++) {
                addrInfo->address->addressData[i] = currentAddr->addr->sa_data[i];
            }

            addrInfo->broadcastAddress = nullptr;
            if (currentAddr->broadaddr != NULL) {
                addrInfo->broadcastAddress = gcnew SockAddr();
                addrInfo->broadcastAddress->addressFamily = currentAddr->broadaddr->sa_family;
                addrInfo->broadcastAddress->addressData = gcnew array<Byte>(14);
                for (int i = 0; i < 14; i++) {
                    addrInfo->broadcastAddress->addressData[i] = currentAddr->broadaddr->sa_data[i];
                }
            }

            addrInfo->netmask = nullptr;
            if (currentAddr->netmask != NULL) {
                addrInfo->netmask = gcnew SockAddr();
                addrInfo->netmask->addressFamily = currentAddr->netmask->sa_family;
                addrInfo->netmask->addressData = gcnew array<Byte>(14);
                for (int i = 0; i < 14; i++) {
                    addrInfo->netmask->addressData[i] = currentAddr->netmask->sa_data[i];
                }
            }

            addrInfo->destinationAddress = nullptr;
            if (currentAddr->dstaddr != NULL) {
                addrInfo->destinationAddress = gcnew SockAddr();
                addrInfo->destinationAddress->addressFamily = currentAddr->dstaddr->sa_family;
                addrInfo->destinationAddress->addressData = gcnew array<Byte>(14);
                for (int i = 0; i < 14; i++) {
                    addrInfo->destinationAddress->addressData[i] = currentAddr->dstaddr->sa_data[i];
                }
            }

            currentAddr = currentAddr->next;
        }

        devices->Add(devInfo);
        currentDevice = currentDevice->next;
    }

    pcap_freealldevs(devicesNative);
    return result;
}

// TODO: implement pcap_findalldevs_ex

// const char *pcap_lib_version(void);
String^ WpcapWrapper::PcapLibVersion() {
    const char* libVersionNative = pcap_lib_version();
    String^ libVersionManaged = gcnew String(libVersionNative);
    return libVersionManaged;
}

//Int32 WpcapWrapper::PcapLiveDump(Guid pcapInstId, String^ fileName, Int32 maxSize, Int32 maxPacks) {
//    PcapInstance^ pi = this->pcapInstances[pcapInstId];
//    if (pi == nullptr) {
//        fprintf(stderr, "failed to get pcap instance for GUID %s\n", pcapInstId.ToString());
//        return -1;
//    }
//
//    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fileName);
//    const char* fnameStrNative = (const char*)fnamePtr.ToPointer();
//
//    int result = 
//}

// TODO: implement rmt auth param
Int32 WpcapWrapper::PcapOpen(String^ source, Int32 snapLen, Int32 flags, Int32 readTimeout, Guid^ pcapInstId) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr srcPtr = Marshal::StringToHGlobalAuto(source);
    const char* srcNative = (const char*)srcPtr.ToPointer();

    pcap_t* pcap = pcap_open(srcNative, snapLen, flags, readTimeout, NULL, errBuf);
    if (pcap == NULL) {
        fprintf(stderr, "failed to open pcap instance: %s\n", errBuf);
        return -1;
    }

    PcapInstance^ pi = gcnew PcapInstance();
    pi->instanceId = Guid::NewGuid();
    pi->pcap = pcap;
    this->pcapInstances->Add(pi->instanceId, pi);
    pcapInstId = pi->instanceId;
    return 0;
}

Int32 WpcapWrapper::PcapParseSrcStr(String^ source, Int32^ type, String^ host, String^ port, String^ name) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr srcPtr = Marshal::StringToHGlobalAuto(source);
    const char* srcNative = (const char*)srcPtr.ToPointer();
    int typeNative = 0;
    char hostNative[PCAP_BUF_SIZE] = {};
    char* portNative[PCAP_BUF_SIZE] = {};
    char* nameNative[PCAP_BUF_SIZE] = {};
    
    int result = pcap_parsesrcstr(srcNative, &typeNative, hostNative, portNative, nameNative, errBuf);
    if (result == -1) {
        fprintf(stderr, "failed to parse source string: %s: %s\n", srcNative, errBuf);
        return result;
    }

    type = typeNative;
    host = hostNative;
    port = portNative;
    name = nameNative;
    return 0;
}


// TODO: implement remaining functions