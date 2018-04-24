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

Int32 PcapWrapper::PcapLookupNet(String^ device, UInt32^ network, UInt32^ mask) {
	char errBuf[PCAP_ERRBUF_SIZE] = {};
	IntPtr devicePtr = Marshal::StringToHGlobalAuto(device);
	const char* deviceNative = (const char*)devicePtr.ToPointer();

	UINT32 networkNative = 0;
	UINT32 maskNative = 0;

	int resultNative = pcap_lookupnet(deviceNative, &networkNative, &maskNative, errBuf);
	if (resultNative == -1) {
		fprintf(stderr, "failed to get network number and netmask for device: %s: %s\n", deviceNative, errBuf);
	}

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
	for (int i = 0; i < pktHdrNative->caplen; i++) {
		result[i] = pktData[i];
	}

	return result;
}

Int32 PcapNextEx(IntPtr pcapHandle, PcapPktHdr^ pcapPktHdr, array<Byte>^ pktData) {
	pcap_pkthdr* pktHdrNative = NULL;
	u_char* pktDataNative = NULL;

	int result = pcap_next_ex(UnMarshalPcapHandle(pcapHandle), &pktHdrNative, &pktDataNative);
	if (result == -1) {
		const char* err_msg = pcap_geterr(UnMarshalPcapHandle(pcapHandle));
		fprintf(stderr, "failed to get packet: %s\n", err_msg);
		return result;
	}

	pcapPktHdr = gcnew PcapPktHdr();
	pcapPktHdr->timeVal.seconds = pktHdrNative->ts.tv_sec;
	pcapPktHdr->timeVal.microSeconds = pktHdrNative->ts.tv_usec;
	pcapPktHdr->capLen = pktHdrNative->caplen;
	pcapPktHdr->len = pktHdrNative->len;

	pktData = gcnew array<Byte>(pktHdrNative->caplen);
	for (int i = 0; i < pktHdrNative->caplen; i++) {
		pktData[i] = pktDataNative[i];
	}

	return result;
}

void PcapWrapper::PcapBreakLoop(IntPtr pcapHandle) {
	pcap_breakloop(UnMarshalPcapHandle(pcapHandle));
}

Int32 PcapWrapper::PcapStats(IntPtr pcapHandle, PcapStat^ pcapStats) {
	pcap_stat* ps;
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
Int32 PcapWrapper::PcapInject(IntPtr pcapHandle, )



// TODO: implement remaining functions