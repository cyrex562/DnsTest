#include "stdafx.h"
#include "WpcapWrapper.h"

using namespace std;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

Timeval::Timeval() {

}

PcapPktHdr::PcapPktHdr() {

}

PcapWrapper::PcapWrapper() {

}

PcapStat::PcapStat() {

}

inline pcap_t* UnMarshalPcapHandle(IntPtr inPcapHandle) {
    return (pcap_t*)inPcapHandle.ToPointer();
}

inline IntPtr MarshalPcapHandle(pcap_t* inPcapHandle) {
    return IntPtr(inPcapHandle);
}

Int32 PcapWrapper::PcapLookupNet(String^ device, PcapLookupNetResult^ result) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    IntPtr devicePtr = Marshal::StringToHGlobalAuto(device);
    const char* deviceNative = (const char*)devicePtr.ToPointer();

    UINT32 networkNative = 0;
    UINT32 maskNative = 0;

    int resultNative = pcap_lookupnet(deviceNative, &networkNative, &maskNative, errBuf);
    if (resultNative == -1) {
        fprintf(stderr, "failed to get network number and netmask for device: %s: %s\n", deviceNative, errBuf);
    }

    result->network = networkNative;
    result->mask = maskNative;

    return resultNative;
}

IntPtr PcapWrapper::PcapCreate(String^ source) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr sourcePtr = Marshal::StringToHGlobalAuto(source);
    const char* sourceNative = (const char*)sourcePtr.ToPointer();

    pcap_t* nativePcap = pcap_create(sourceNative, errBuf);
    if (nativePcap == NULL) {
        fprintf(stderr, "failed to create live capture handle for source: %s: %s\n", sourceNative, errBuf);
    }

    return MarshalPcapHandle(nativePcap);
}

Int32 PcapWrapper::PcapSetSnaplen(IntPtr pcapHandle, Int32 snaplen) {
    return pcap_set_snaplen(UnMarshalPcapHandle(pcapHandle), snaplen);
}

Int32 PcapWrapper::PcapSetPromisc(IntPtr pcapHandle, Int32 promisc) {
    return pcap_set_promisc(UnMarshalPcapHandle(pcapHandle), promisc);
}

Int32 PcapWrapper::PcapSetTimeout(IntPtr pcapHandle, Int32 timeout) {
    return pcap_set_timeout(UnMarshalPcapHandle(pcapHandle), timeout);
}

Int32 PcapWrapper::PcapSetTstampType(IntPtr pcapHandle, Int32 tstampType) {
    return pcap_set_tstamp_type(UnMarshalPcapHandle(pcapHandle), tstampType);
}

Int32 PcapWrapper::PcapSetBufferSize(IntPtr pcapHandle, Int32 bufferSize) {
    return pcap_set_buffer_size(UnMarshalPcapHandle(pcapHandle), bufferSize);
}

Int32 PcapWrapper::PcapSetTstampPrecision(IntPtr pcapHandle, Int32 tstampPrecision) {
    return pcap_set_tstamp_precision(UnMarshalPcapHandle(pcapHandle), tstampPrecision);
}

Int32 PcapWrapper::PcapGetTstampPrecision(IntPtr pcapHandle) {
    return pcap_get_tstamp_precision(UnMarshalPcapHandle(pcapHandle));
}

Int32 PcapWrapper::PcapActivate(IntPtr pcapHandle) {
    return pcap_activate(UnMarshalPcapHandle(pcapHandle));
}

Int32 PcapWrapper::PcapListTstampTypes(IntPtr pcapHandle, List<PcapTstampTypes>^ tstampTypes) {
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

Int32 PcapWrapper::TstampTypeNameToVal(String^ name) {
    IntPtr namePtr = Marshal::StringToHGlobalAuto(name);
    const char* nameNative = (const char*)namePtr.ToPointer();
    return pcap_tstamp_type_name_to_val(nameNative);
}

String^ PcapWrapper::PcapTstampTypeValToName(Int32 typeVal) {
    const char* typeValNative = pcap_tstamp_type_val_to_name(typeVal);
    if (typeValNative == NULL) {
        fprintf(stderr, "failed to get name for type val: %i\n", typeVal);
        return nullptr;
    }

    String^ result = gcnew String(typeValNative);
    return result;
}

String^ PcapWrapper::PcapTstampTypeValToDesc(Int32 typeVal) {
    const char* typeValNative = pcap_tstamp_type_val_to_description(typeVal);
    if (typeValNative == NULL) {
        fprintf(stderr, "failed to get name for type val: %i\n", typeVal);
        return nullptr;
    }

    String^ result = gcnew String(typeValNative);
    return result;
}

IntPtr PcapWrapper::PcapOpenDead(Int32 linkType, Int32 snaplen) {
    pcap_t* pcapHandleNative = pcap_open_dead(linkType, snaplen);
    IntPtr result = MarshalPcapHandle(pcapHandleNative);
    return result;
}

IntPtr PcapWrapper::PcapOpenDeadWithTstampPrecision(Int32 linkType, Int32 snaplen, UInt32 precision) {
    pcap_t* pcapHandleNative = pcap_open_dead_with_tstamp_precision(linkType, snaplen, precision);
    IntPtr result = MarshalPcapHandle(pcapHandleNative);
    return result;
}

IntPtr PcapWrapper::PcapOpenOffline(String^ fname) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();

    pcap_t* pcapHandleNative = pcap_open_offline(fnameNative, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s: %s\n", fnameNative, errBuf);
    }
    return MarshalPcapHandle(pcapHandleNative);
}

IntPtr PcapWrapper::PcapOpenOfflineWithTstampPrecision(String^ fname, UInt32 precision) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};

    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();

    pcap_t* pcapHandleNative = pcap_open_offline_with_tstamp_precision(fnameNative, precision, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s: %s\n", fnameNative, errBuf);
    }
    return MarshalPcapHandle(pcapHandleNative);
}

IntPtr PcapWrapper::PcapHopenOffline(IntPtr desc) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    intptr_t ptr = (intptr_t)desc.ToPointer();
    pcap_t* pcapHandleNative = pcap_hopen_offline(ptr, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s\n", errBuf);
    }
    return MarshalPcapHandle(pcapHandleNative);
}

IntPtr PcapWrapper::PcapHopenOfflineWithTstampPrecision(IntPtr desc, UInt32 precision) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    intptr_t ptr = (intptr_t)desc.ToPointer();
    pcap_t* pcapHandleNative = pcap_hopen_offline_with_tstamp_precision(ptr, precision, errBuf);
    if (pcapHandleNative == NULL) {
        fprintf(stderr, "failed to open file for processing: %s\n", errBuf);
    }
    return MarshalPcapHandle(pcapHandleNative);
}

void PcapWrapper::PcapClose(IntPtr pcapHandle) {
    pcap_close(UnMarshalPcapHandle(pcapHandle));
}

Int32 PcapWrapper::PcapLoop(IntPtr pcapHandle, Int32 count, PcapCallbackDelegate^ d, Byte user[]) {
    pcap_handler cbPtr = (pcap_handler)Marshal::GetFunctionPointerForDelegate(d).ToPointer();
    return pcap_loop(UnMarshalPcapHandle(pcapHandle), count, cbPtr, user);
}

Int32 PcapWrapper::PcapDispatch(IntPtr pcapHandle, Int32 count, PcapCallbackDelegate^ d, Byte user[]) {
    pcap_handler cbPtr = (pcap_handler)Marshal::GetFunctionPointerForDelegate(d).ToPointer();
    return pcap_dispatch(UnMarshalPcapHandle(pcapHandle), count, cbPtr, user);
}

array<Byte>^ PcapWrapper::PcapNext(IntPtr pcapHandle, PcapPktHdr^ pcapPktHdr) {
    pcap_pkthdr* pktHdrNative = NULL;
    const u_char* pktData = pcap_next(UnMarshalPcapHandle(pcapHandle), pktHdrNative);

    pcapPktHdr = gcnew PcapPktHdr();
    pcapPktHdr->timeVal.seconds = pktHdrNative->ts.tv_sec;
    pcapPktHdr->timeVal.microSeconds = pktHdrNative->ts.tv_usec;
    pcapPktHdr->capLen = pktHdrNative->caplen;
    pcapPktHdr->len = pktHdrNative->len;

    array<Byte>^ result = gcnew array<Byte>(pktHdrNative->caplen);
    for (UInt32 i = 0; i < pktHdrNative->caplen; i++) {
        result[i] = pktData[i];
    }

    return result;
}

// TODO: if array is passed in pre-allocated, specify size and check bounds before performing copy operation
Int32 PcapWrapper::PcapNextEx(IntPtr pcapHandle, PcapPktHdr^ pcapPktHdr, array<Byte>^ pktData) {
    pcap_pkthdr* pktHdrNative = NULL;
    const u_char* pktDataNative = NULL;

    pcap_t* pcapHandleNative = (pcap_t*)pcapHandle.ToPointer();

    int result = pcap_next_ex(UnMarshalPcapHandle(pcapHandle), &pktHdrNative, &pktDataNative);
    if (result == -1) {
        const char* err_msg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
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
        pktData[i] = pktDataNative[i];
    }

    return result;
}

void PcapWrapper::PcapBreakLoop(IntPtr pcapHandle) {
    pcap_breakloop(UnMarshalPcapHandle(pcapHandle));
}

Int32 PcapWrapper::PcapStats(IntPtr pcapHandle, PcapStat^ pcapStats) {
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

Int32 PcapWrapper::PcapSetFilter(IntPtr pcapHandle, IntPtr bpfProgram) {
    bpf_program* bpfProgramNative = (bpf_program*)bpfProgram.ToPointer();
    int result = pcap_setfilter(UnMarshalPcapHandle(pcapHandle), bpfProgramNative);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to set filter: %s\n", errMsg);
    }

    return result;
}

Int32 PcapWrapper::PcapSetDirection(IntPtr pcapHandle, PcapDirection pcapDirection) {
    int result = pcap_setdirection(UnMarshalPcapHandle(pcapHandle), (pcap_direction_t)pcapDirection);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to set filter: %s\n", errMsg);
    }
    
    return result;
}

// int pcap_setnonblock(pcap_t *p, int nonblock, char *errbuf);
Int32 PcapWrapper::PcapSetNonBlock(IntPtr pcapHandle, Int32 nonBlock) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    int result = pcap_setnonblock(UnMarshalPcapHandle(pcapHandle), nonBlock, errBuf);
    if (result == -1) {
        fprintf(stderr, "failed to set blocking state: %s\n", errBuf);
    }
    return result;
}

// int pcap_getnonblock(pcap_t *p, char *errbuf);
Int32 PcapWrapper::PcapGetNonBLock(IntPtr pcapHandle) {
    char errBuf[PCAP_ERRBUF_SIZE] = {};
    int nonBlockState = pcap_getnonblock(UnMarshalPcapHandle(pcapHandle), errBuf);
    if (nonBlockState == -1) {
        fprintf(stderr, "failed to get blocking state: %s\n", errBuf);
    }
    return nonBlockState;
}

// const char *pcap_statustostr(int error);
String^ PcapWrapper::PcapStatusToStr(Int32 error) {
    const char* statusStrNative = pcap_statustostr(error);
    String^ result = gcnew String(statusStrNative);
    return result;
}

// int pcap_inject(pcap_t *p, const void *buf, size_t size);
Int32 PcapWrapper::PcapInject(IntPtr pcapHandle, array<Byte>^ packet) {
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
Int32 PcapWrapper::PcapSendPacket(IntPtr pcapHandle, array<Byte>^ packet) {
    int pktBufSz = packet->Length;
    Byte* pktBuf = (Byte*)malloc(pktBufSz);
    if (pktBuf == NULL) {
        fprintf(stderr, "Failed to allocate buffer\n");
        return -1;
    }
    ZeroMemory(pktBuf, pktBufSz);

    for (int i = 0; i < pktBufSz; i++) {
        pktBuf[i] = packet[i];
    }

    int result = pcap_sendpacket(UnMarshalPcapHandle(pcapHandle), pktBuf, pktBufSz);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to send packet :%s\n", errMsg);
    }

    free(pktBuf);
    pktBuf = NULL;
    return result;
}

// int pcap_compile(pcap_t *p, struct bpf_program *fp, const char *str, int optimize, bpf_u_int32 netmask);
Int32 PcapWrapper::PcapCompile(IntPtr pcapHandle, IntPtr bpfProgram, String^ filterStr, Int32 optimize, UInt32 netmask) {
    IntPtr filterStrPtr = Marshal::StringToHGlobalAuto(filterStr);
    const char* filterStrNative = (const char*)filterStrPtr.ToPointer();

    /*bpf_program* fp = (bpf_program*)malloc(sizeof(bpf_program));
    if (fp == NULL) {
        fprintf(stderr, "failed to allocate buffer for bpf_program struct\n");
        return -1;
    }
    ZeroMemory(fp, sizeof(bpf_program));*/
    bpf_program* fp = NULL;
    int result = pcap_compile(UnMarshalPcapHandle(pcapHandle), fp, filterStrNative, optimize, netmask);
    if (result == -1) {
        const char* errMsg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
        fprintf(stderr, "failed to compile filter: %s\n", errMsg);
        return result;
    }

    bpfProgram = IntPtr(fp);

    return  result;
}

void PcapWrapper::PcapFreeCode(IntPtr bpfProgram) {
    bpf_program* fp = (bpf_program*)bpfProgram.ToPointer();
    pcap_freecode(fp);
}

// int pcap_offline_filter(const struct bpf_program *fp, const struct pcap_pkthdr *h, const u_char *pkt)
Int32 PcapWrapper::PcapOfflineFilter(IntPtr bpfProgram, PcapPktHdr^ pktHdr, array<Byte>^ pkt) {
    bpf_program* fp = (bpf_program*)bpfProgram.ToPointer();
    pcap_pkthdr pktHdrNative = {};
    pktHdrNative.caplen = pktHdr->capLen;
    pktHdrNative.len = pktHdr->len;
    pktHdrNative.ts.tv_sec = pktHdr->timeVal.seconds;
    pktHdrNative.ts.tv_usec = pktHdr->timeVal.microSeconds;

    Byte* pktNative = (Byte*)malloc(pktHdrNative.caplen);
    if (pktNative == NULL) {
        fprintf(stderr, "failed to allocate buffer for packet\n");
        return -1;
    }
    ZeroMemory(pktNative, pktHdrNative.caplen);
    for (UInt32 i = 0; i < pktHdrNative.caplen; i++) {
        pktNative[i] = pkt[i];
    }

    int result = pcap_offline_filter(fp, &pktHdrNative, pktNative);
    
    free(pktNative);
    pktNative = NULL;

    return result;
}

// int pcap_datalink(pcap_t *p);
Int32 PcapWrapper::PcapDatalink(IntPtr pcapHandle) {
    return pcap_datalink(UnMarshalPcapHandle(pcapHandle));
}

// int pcap_list_datalinks(pcap_t *p, int **dlt_buf);
Int32 PcapWrapper::PcapListDatalinks(IntPtr pcapHandle, List<Int32>^ datalinks) {
    int* datalinksNative = NULL;
    int result = pcap_list_datalinks(UnMarshalPcapHandle(pcapHandle), &datalinksNative);
    if (result == -1) {
        fprintf(stderr, "failed to get list of datalinks: %s\n", pcap_geterr(UnMarshalPcapHandle(pcapHandle)));
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
Int32 PcapWrapper::PcapSetDatalink(IntPtr pcapHandle, Int32 datalinkType) {
    return pcap_set_datalink(UnMarshalPcapHandle(pcapHandle), datalinkType);
}

// int pcap_datalink_name_to_val(const char *name);
Int32 PcapWrapper::PcapDatalinkNameToVal(String^ name) {
    IntPtr namePtr = Marshal::StringToHGlobalAuto(name);
    const char* nameNative = (const char*)namePtr.ToPointer();
    return pcap_datalink_name_to_val(nameNative);
}

String^ PcapWrapper::PcapDatalinkValToName(Int32 datalinkType) {
    const char* result = pcap_datalink_val_to_name(datalinkType);
    if (result == NULL) {
        fprintf(stderr, "failed to convert datalink type to name\n");
        return nullptr;
    }
    String^ resultManaged = gcnew String(result);
    return resultManaged;
}

String^ PcapWrapper::PcapDatalinkValToDescription(Int32 datalinkType) {
    const char* result = pcap_datalink_val_to_description(datalinkType);
    if (result == NULL) {
        fprintf(stderr, "failed to convert datalink type to name\n");
        return nullptr;
    }
    String^ resultManaged = gcnew String(result);
    return resultManaged;
}

// int pcap_snapshot(pcap_t *p);
Int32 PcapWrapper::PcapSnapshot(IntPtr pcapHandle) {
    return pcap_snapshot(UnMarshalPcapHandle(pcapHandle));
}

// int pcap_is_swapped(pcap_t *p);
Int32 PcapWrapper::PcapIsSwapped(IntPtr pcapHandle) {
    return pcap_is_swapped(UnMarshalPcapHandle(pcapHandle));
}

// int pcap_major_version(pcap_t *p);
Int32 PcapWrapper::PcapMajorVersion(IntPtr pcapHandle) {
    return pcap_major_version(UnMarshalPcapHandle(pcapHandle));
}

// int pcap_minor_version(pcap_t *p);
Int32 PcapWrapper::PcapMinorVersion(IntPtr pcapHandle) {
    return pcap_minor_version(UnMarshalPcapHandle(pcapHandle));
}

// FILE *pcap_file(pcap_t *p);
IntPtr PcapWrapper::PcapFile(IntPtr pcapHandle) {
    FILE* fp = pcap_file(UnMarshalPcapHandle(pcapHandle));
    return IntPtr(fp);
}

// pcap_dumper_t *pcap_dump_open(pcap_t *p, const char *fname);
IntPtr PcapWrapper::PcapDumpOpen(IntPtr pcapHandle, String^ fname) {
    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();
    pcap_dumper_t* pdt = pcap_dump_open(UnMarshalPcapHandle(pcapHandle), fnameNative);
    if (pdt == NULL) {
        fprintf(stderr, "failed to open file %s for writing packets: %s\n", fnameNative, pcap_geterr(UnMarshalPcapHandle(pcapHandle)));
    }
    return IntPtr(pdt);
}

IntPtr PcapWrapper::PcapDumpOpenAppend(IntPtr pcapHandle, String^ fname) {
    IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
    const char* fnameNative = (const char*)fnamePtr.ToPointer();
    pcap_dumper_t* pdt = pcap_dump_open_append(UnMarshalPcapHandle(pcapHandle), fnameNative);
    if (pdt == NULL) {
        fprintf(stderr, "failed to open file %s for writing packets: %s\n", fnameNative, pcap_geterr(UnMarshalPcapHandle(pcapHandle)));
    }
    return IntPtr(pdt);
}

// pcap_dumper_t *pcap_dump_fopen(pcap_t *p, FILE *fp);
IntPtr PcapWrapper::PcapDumpFopen(IntPtr pcapHandle, IntPtr fp) {
    FILE* fpNative = (FILE*)fp.ToPointer();
    pcap_dumper_t* pdt = pcap_dump_fopen(UnMarshalPcapHandle(pcapHandle), fpNative);
    if (pdt == NULL) {
        fprintf(stderr, "failed to open file for writing packets: %s\n", pcap_geterr(UnMarshalPcapHandle(pcapHandle)));
    }
    return IntPtr(pdt);
}

// FILE *pcap_dump_file(pcap_dumper_t *p);
IntPtr PcapWrapper::PcapDumpFile(IntPtr pcapDumper) {
    pcap_dumper_t* pdt = (pcap_dumper_t*)pcapDumper.ToPointer();
    FILE* fp = pcap_dump_file(pdt);
    if (fp == NULL) {
        fprintf(stderr, "failed to open I/O stream for writing savefile\n");
    }
    return IntPtr(fp);
}

// long pcap_dump_ftell(pcap_dumper_t *p);
Int32 PcapWrapper::PcapDumpFtell(IntPtr pcapDumper) {
    pcap_dumper_t* pdt = (pcap_dumper_t*)pcapDumper.ToPointer();
    return pcap_dump_ftell(pdt);
}

// void pcap_dump_close(pcap_dumper_t *p);
void PcapWrapper::PcapDumpClose(IntPtr pcapDumper) {
    pcap_dumper_t* pdt = (pcap_dumper_t*)pcapDumper.ToPointer();
    pcap_dump_close(pdt);
}

// int pcap_dump_flush(pcap_dumper_t *p);
Int32 PcapWrapper::PcapDumpFlush(IntPtr pcapDumper) {
    pcap_dumper_t* pdt = (pcap_dumper_t*)pcapDumper.ToPointer();
    return pcap_dump_flush(pdt);
}

// void pcap_dump(u_char *user, struct pcap_pkthdr *h, u_char *sp);
void PcapWrapper::PcapDump(IntPtr user, PcapPktHdr^ pktHdr, array<Byte>^ pkt) {
    Byte* userNative = (Byte*)user.ToPointer();
    pcap_pkthdr pktHdrNative = {};
    pktHdrNative.caplen = pktHdr->capLen;
    pktHdrNative.len = pktHdr->len;
    pktHdrNative.ts.tv_sec = pktHdr->timeVal.seconds;
    pktHdrNative.ts.tv_usec = pktHdr->timeVal.microSeconds;
    Byte* pktNative = (Byte*)malloc(pktHdr->capLen);
    if (pktNative == NULL) {
        fprintf(stderr, "failed to allocate buffer for pktNative\n");
        return;
    }
    ZeroMemory(pktNative, pktHdr->capLen);
    for (UInt32 i = 0; i < pktHdr->capLen; i++) {
        pktNative[i] = pkt[i];
    }
    pcap_dump(userNative, &pktHdrNative, pktNative);
}

// int pcap_findalldevs(pcap_if_t **alldevsp, char *errbuf);
Int32 PcapWrapper::PcapFindAllDevs(List<PcapDeviceInfo^>^ devices) {
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

// const char *pcap_lib_version(void);
String^ PcapWrapper::PcapLibVersion() {
    const char* libVersionNative = pcap_lib_version();
    String^ libVersionManaged = gcnew String(libVersionNative);
    return libVersionManaged;
}


// TODO: implement remaining functions