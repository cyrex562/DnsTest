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

inline pcap_t* UnMarshalPcapHandle(System::IntPtr inPcapHandle) {
	return (pcap_t*)inPcapHandle.ToPointer();
}

inline System::IntPtr MarshalPcapHandle(pcap_t* inPcapHandle) {
	return System::IntPtr(inPcapHandle);
}

System::Int32 PcapWrapper::PcapLookupNet(System::String^ device, System::UInt32^ network, System::UInt32^ mask) {
	char errBuf[PCAP_ERRBUF_SIZE] = {};
	System::IntPtr devicePtr = Marshal::StringToHGlobalAuto(device);
	const char* deviceNative = (const char*)devicePtr.ToPointer();

	UINT32 networkNative = 0;
	UINT32 maskNative = 0;

	int resultNative = pcap_lookupnet(deviceNative, &networkNative, &maskNative, errBuf);
	if (resultNative == -1) {
		fprintf(stderr, "failed to get network number and netmask for device: %s: %s\n", deviceNative, errBuf);
	}

	return resultNative;
}

System::IntPtr PcapWrapper::PcapCreate(System::String^ source) {
	char errBuf[PCAP_ERRBUF_SIZE] = {};

	System::IntPtr sourcePtr = Marshal::StringToHGlobalAuto(source);
	const char* sourceNative = (const char*)sourcePtr.ToPointer();

	pcap_t* nativePcap = pcap_create(sourceNative, errBuf);
	if (nativePcap == NULL) {
		fprintf(stderr, "failed to create live capture handle for source: %s: %s\n", sourceNative, errBuf);
	}

	return MarshalPcapHandle(nativePcap);
}

System::Int32 PcapWrapper::PcapSetSnaplen(System::IntPtr pcapHandle, System::Int32 snaplen) {
	return pcap_set_snaplen(UnMarshalPcapHandle(pcapHandle), snaplen);
}

System::Int32 PcapWrapper::PcapSetPromisc(System::IntPtr pcapHandle, System::Int32 promisc) {
	return pcap_set_promisc(UnMarshalPcapHandle(pcapHandle), promisc);
}

System::Int32 PcapWrapper::PcapSetTimeout(System::IntPtr pcapHandle, System::Int32 timeout) {
	return pcap_set_timeout(UnMarshalPcapHandle(pcapHandle), timeout);
}

System::Int32 PcapWrapper::PcapSetTstampType(System::IntPtr pcapHandle, System::Int32 tstampType) {
	return pcap_set_tstamp_type(UnMarshalPcapHandle(pcapHandle), tstampType);
}

System::Int32 PcapWrapper::PcapSetBufferSize(System::IntPtr pcapHandle, System::Int32 bufferSize) {
	return pcap_set_buffer_size(UnMarshalPcapHandle(pcapHandle), bufferSize);
}

System::Int32 PcapWrapper::PcapSetTstampPrecision(System::IntPtr pcapHandle, System::Int32 tstampPrecision) {
	return pcap_set_tstamp_precision(UnMarshalPcapHandle(pcapHandle), tstampPrecision);
}

System::Int32 PcapWrapper::PcapGetTstampPrecision(System::IntPtr pcapHandle) {
	return pcap_get_tstamp_precision(UnMarshalPcapHandle(pcapHandle));
}

System::Int32 PcapWrapper::PcapActivate(System::IntPtr pcapHandle) {
	return pcap_activate(UnMarshalPcapHandle(pcapHandle));
}

System::Int32 PcapWrapper::PcapListTstampTypes(System::IntPtr pcapHandle, List<PcapTstampTypes>^ tstampTypes) {
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

System::Int32 PcapWrapper::TstampTypeNameToVal(System::String^ name) {
	System::IntPtr namePtr = Marshal::StringToHGlobalAuto(name);
	const char* nameNative = (const char*)namePtr.ToPointer();
	return pcap_tstamp_type_name_to_val(nameNative);
}

System::String^ PcapWrapper::PcapTstampTypeValToName(System::Int32 typeVal) {
	const char* typeValNative = pcap_tstamp_type_val_to_name(typeVal);
	if (typeValNative == NULL) {
		fprintf(stderr, "failed to get name for type val: %i\n", typeVal);
		return nullptr;
	}

	String^ result = gcnew String(typeValNative);
	return result;
}

System::String^ PcapWrapper::PcapTstampTypeValToDesc(System::Int32 typeVal) {
	const char* typeValNative = pcap_tstamp_type_val_to_description(typeVal);
	if (typeValNative == NULL) {
		fprintf(stderr, "failed to get name for type val: %i\n", typeVal);
		return nullptr;
	}

	String^ result = gcnew String(typeValNative);
	return result;
}

System::IntPtr PcapWrapper::PcapOpenDead(System::Int32 linkType, System::Int32 snaplen) {
	pcap_t* pcapHandleNative = pcap_open_dead(linkType, snaplen);
	System::IntPtr result = MarshalPcapHandle(pcapHandleNative);
	return result;
}

System::IntPtr PcapWrapper::PcapOpenDeadWithTstampPrecision(System::Int32 linkType, System::Int32 snaplen, System::UInt32 precision) {
	pcap_t* pcapHandleNative = pcap_open_dead_with_tstamp_precision(linkType, snaplen, precision);
	System::IntPtr result = MarshalPcapHandle(pcapHandleNative);
	return result;
}

System::IntPtr PcapWrapper::PcapOpenOffline(System::String^ fname) {
	char errBuf[PCAP_ERRBUF_SIZE] = {};

	System::IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
	const char* fnameNative = (const char*)fnamePtr.ToPointer();

	pcap_t* pcapHandleNative = pcap_open_offline(fnameNative, errBuf);
	if (pcapHandleNative == NULL) {
		fprintf(stderr, "failed to open file for processing: %s: %s\n", fnameNative, errBuf);
	}
	return MarshalPcapHandle(pcapHandleNative);
}

System::IntPtr PcapWrapper::PcapOpenOfflineWithTstampPrecision(System::String^ fname, System::UInt32 precision) {
	char errBuf[PCAP_ERRBUF_SIZE] = {};

	System::IntPtr fnamePtr = Marshal::StringToHGlobalAuto(fname);
	const char* fnameNative = (const char*)fnamePtr.ToPointer();

	pcap_t* pcapHandleNative = pcap_open_offline_with_tstamp_precision(fnameNative, precision, errBuf);
	if (pcapHandleNative == NULL) {
		fprintf(stderr, "failed to open file for processing: %s: %s\n", fnameNative, errBuf);
	}
	return MarshalPcapHandle(pcapHandleNative);
}

System::IntPtr PcapWrapper::PcapHopenOffline(System::IntPtr desc) {
	char errBuf[PCAP_ERRBUF_SIZE] = {};
	intptr_t ptr = (intptr_t)desc.ToPointer();
	pcap_t* pcapHandleNative = pcap_hopen_offline(ptr, errBuf);
	if (pcapHandleNative == NULL) {
		fprintf(stderr, "failed to open file for processing: %s\n", errBuf);
	}
	return MarshalPcapHandle(pcapHandleNative);
}

System::IntPtr PcapWrapper::PcapHopenOfflineWithTstampPrecision(System::IntPtr desc, System::UInt32 precision) {
	char errBuf[PCAP_ERRBUF_SIZE] = {};
	intptr_t ptr = (intptr_t)desc.ToPointer();
	pcap_t* pcapHandleNative = pcap_hopen_offline_with_tstamp_precision(ptr, precision, errBuf);
	if (pcapHandleNative == NULL) {
		fprintf(stderr, "failed to open file for processing: %s\n", errBuf);
	}
	return MarshalPcapHandle(pcapHandleNative);
}

void PcapWrapper::PcapClose(System::IntPtr pcapHandle) {
	pcap_close(UnMarshalPcapHandle(pcapHandle));
}

System::Int32 PcapWrapper::PcapLoop(System::IntPtr pcapHandle, System::Int32 count, PcapCallbackDelegate^ d, System::Byte user[]) {
	pcap_handler cbPtr = (pcap_handler)Marshal::GetFunctionPointerForDelegate(d).ToPointer();
	return pcap_loop(UnMarshalPcapHandle(pcapHandle), count, cbPtr, user);
}

System::Int32 PcapWrapper::PcapDispatch(System::IntPtr pcapHandle, System::Int32 count, PcapCallbackDelegate^ d, System::Byte user[]) {
	pcap_handler cbPtr = (pcap_handler)Marshal::GetFunctionPointerForDelegate(d).ToPointer();
	return pcap_dispatch(UnMarshalPcapHandle(pcapHandle), count, cbPtr, user);
}

array<System::Byte>^ PcapWrapper::PcapNext(System::IntPtr pcapHandle, PcapPktHdr^ pcapPktHdr) {
	pcap_pkthdr* pktHdrNative = NULL;
	const u_char* pktData = pcap_next(UnMarshalPcapHandle(pcapHandle), pktHdrNative);

	pcapPktHdr = gcnew PcapPktHdr();
	pcapPktHdr->timeVal.seconds = pktHdrNative->ts.tv_sec;
	pcapPktHdr->timeVal.microSeconds = pktHdrNative->ts.tv_usec;
	pcapPktHdr->capLen = pktHdrNative->caplen;
	pcapPktHdr->len = pktHdrNative->len;

	array<System::Byte>^ result = gcnew array<System::Byte>(pktHdrNative->caplen);
	for (int i = 0; i < pktHdrNative->caplen; i++) {
		result[i] = pktData[i];
	}

	return result;
}

// TODO: implement remaining functions