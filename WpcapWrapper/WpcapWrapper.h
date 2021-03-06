#pragma once

using namespace std;
using namespace System;
using namespace System::Collections::Generic;

public enum class PcapTstampTypes {
    PcapTstampHost = PCAP_TSTAMP_HOST,
    PcapTstampHostLowPrec = PCAP_TSTAMP_HOST_LOWPREC,
    PcapTstampAdapter = PCAP_TSTAMP_ADAPTER,
    PcapTstampAdapterUnsynced = PCAP_TSTAMP_ADAPTER_UNSYNCED
};

public enum class PcapDirection {
    PcapDirInOut = PCAP_D_INOUT,
    PcapDirIn = PCAP_D_IN,
    PcapDirOut = PCAP_D_OUT
};

public enum class PcapDataLinkType {
    DltNull = DLT_NULL,
    DltEn10Mb = DLT_EN10MB,
    DltIeee802 = DLT_IEEE802,
    DltArcNet = DLT_ARCNET,
    DltSlip = DLT_SLIP,
    DltPpp = DLT_PPP,
    DltFddi = DLT_FDDI,
    DltAtmRfc1483 = DLT_ATM_RFC1483,
    DltRaw = DLT_RAW,
    DltPppSerial = DLT_PPP_SERIAL,
    DltPppEther = DLT_PPP_ETHER,
    DltCHdlc = DLT_C_HDLC,
    DltIeee80211 = DLT_IEEE802_11,
    DltFRelay = DLT_FRELAY,
    DLtLoop = DLT_LOOP,
    DltLinuxSll = DLT_LINUX_SLL,
    DltLTalk = DLT_LTALK,
    DltPFLog = DLT_PFLOG,
    DltPrismHeader = DLT_PRISM_HEADER,
    DltIpOverFc = DLT_IP_OVER_FC,
    DltSunAtm = DLT_SUNATM,
    DltIeee80211Radio = DLT_IEEE802_11_RADIO,
    DltArcNetLinux = DLT_ARCNET_LINUX,
    DltLinuxIrda = DLT_LINUX_IRDA,
};

public ref class Timeval {
public:
    Timeval();
    Int32 seconds;
    Int32 microSeconds;
};

public ref class PcapPktHdr {
public:
    PcapPktHdr();
    Timeval timeVal;
    UInt32 capLen;
    UInt32 len;
};

public ref class PcapStat {
public:
    PcapStat();
    UInt32 psRecv;
    UInt32 psDrop;
    UInt32 psIfDrop;
};

public ref class SockAddr {
public:
    UInt16 addressFamily;
    array<Byte>^ addressData;
};

public ref class PcapAddrInfo {
public:
    SockAddr^ address;
    SockAddr^ netmask;
    SockAddr^ broadcastAddress;
    SockAddr^ destinationAddress;
};

public ref class PcapDeviceInfo {
public:
    String^ name;
    String^ description;
    List<PcapAddrInfo^>^ addresses;
    UInt32 flags;
};

//public ref class PcapLookupNetResult {
//public:
//    UInt32 network;
//    UInt32 mask;
//};

public ref class PcapInstance {
public:
    Guid instanceId;
    pcap_t* pcap;
};

public ref class BpfProgram {
public:
    Guid programId;
    bpf_program* program;
};

public ref class DumperInstance {
public:
    Guid dumperId;
    pcap_dumper_t* dumper;
};

public ref class FileInstance {
public:
    Guid fileId;
    FILE* fp;
};

public delegate void PcapCallbackDelegate(Byte user[], PcapPktHdr^ pktHdr, Byte packet[]);

public ref class WpcapWrapper
{
public:

    WpcapWrapper();

    // obsolete
    // PCAP_API char	*pcap_lookupdev(char *);
    //String^ PcapLookupDev();

    // PCAP_API int	pcap_lookupnet(const char *, bpf_u_int32 *, bpf_u_int32 *, char *);
    //Int32 PcapLookupNet(String^ device, PcapLookupNetResult^ result);

    // PCAP_API pcap_t	*pcap_create(const char *, char *);
    IntPtr PcapCreate(String^ source);

    // PCAP_API int	pcap_set_snaplen(pcap_t *, int);
    Int32 PcapSetSnaplen(IntPtr PcapHandle, Int32 snaplen);
        
    // PCAP_API int	pcap_set_promisc(pcap_t *, int);
    Int32 PcapSetPromisc(IntPtr pcapHandle, Int32 promisc);

    // pcap_set_rfmon
    // PCAP_API int	pcap_can_set_rfmon(pcap_t *);
    // PCAP_API int	pcap_set_rfmon(pcap_t *, int);
        
    // PCAP_API int	pcap_set_timeout(pcap_t *, int);
    Int32 PcapSetTimeout(IntPtr pcapHandle, Int32 timeout);
        
    // PCAP_API int	pcap_set_tstamp_type(pcap_t *, int);
    Int32 PcapSetTstampType(IntPtr pcapHandle, Int32 tstampType);

    // TODO: unk func
    // PCAP_API int	pcap_set_immediate_mode(pcap_t *, int);
    //int PcapSetImmediateMode(pcap_t*, int);

    // PCAP_API int	pcap_set_buffer_size(pcap_t *, int);
    Int32 PcapSetBufferSize(IntPtr pcapHandle, Int32 bufferSize);

    // PCAP_API int	pcap_set_tstamp_precision(pcap_t *, int);
    Int32 PcapSetTstampPrecision(IntPtr pcapHandle, Int32 tstampPrecision);

    // PCAP_API int	pcap_get_tstamp_precision(pcap_t *);
    Int32 PcapGetTstampPrecision(IntPtr pcapHandle);

    // PCAP_API int	pcap_activate(pcap_t *);
    Int32 PcapActivate(IntPtr pcapHandle);

    // PCAP_API int	pcap_list_tstamp_types(pcap_t *, int **);
    Int32 PcapListTstampTypes(IntPtr pcapHandle, List<PcapTstampTypes>^);

    // PCAP_API void	pcap_free_tstamp_types(int *);
    
    // PCAP_API int	pcap_tstamp_type_name_to_val(const char *);
    Int32 TstampTypeNameToVal(String^ name);

    // PCAP_API const char *pcap_tstamp_type_val_to_name(int);
    String^ PcapTstampTypeValToName(Int32 typeVal);

    // PCAP_API const char *pcap_tstamp_type_val_to_description(int);
    String^ PcapTstampTypeValToDesc(Int32 typeVal);

    // winpcap
    // PCAP_API pcap_t	*pcap_open_live(const char *, int, int, int, char *);
    Guid WpcapWrapper::PcapOpenLive(String^ device, Int32 snaplen, Int32 promisc, Int32 timeoutMillis);

    // PCAP_API pcap_t	*pcap_open_dead(int, int);
    Int32 WpcapWrapper::PcapOpenDead(Int32 linkType, Int32 snaplen);

    // PCAP_API pcap_t	*pcap_open_dead_with_tstamp_precision(int, int, u_int);
    IntPtr PcapOpenDeadWithTstampPrecision(Int32 linkType, Int32 snaplen, UInt32 precision);

    // PCAP_API pcap_t	*pcap_open_offline_with_tstamp_precision(const char *, u_int, char *);
    IntPtr PcapOpenOfflineWithTstampPrecision(String^ fname, UInt32 precision);

    // PCAP_API pcap_t	*pcap_open_offline(const char *, char *);
    Int32 WpcapWrapper::PcapOpenOffline(String^ fname, Guid^ pcapInstId);

    // PCAP_API pcap_t  *pcap_hopen_offline_with_tstamp_precision(intptr_t, u_int, char *);
    IntPtr PcapHopenOfflineWithTstampPrecision(IntPtr desc, UInt32 precision);

    // PCAP_API pcap_t  *pcap_hopen_offline(intptr_t, char *);
    IntPtr PcapHopenOffline(IntPtr desc);

    // PCAP_API void	pcap_close(pcap_t *);
    Int32 WpcapWrapper::PcapClose(Guid instanceId);

    // PCAP_API int	pcap_loop(pcap_t *, int, pcap_handler, u_char *);
    Int32 WpcapWrapper::PcapLoop(Guid pcapInstId, Int32 count, PcapCallbackDelegate^ d);

    // PCAP_API int	pcap_dispatch(pcap_t *, int, pcap_handler, u_char *);
    Int32 WpcapWrapper::PcapDispatch(Guid instanceId, Int32 count, PcapCallbackDelegate^ d, array<Byte>^ user);

    // PCAP_API const u_char *pcap_next(pcap_t *, struct pcap_pkthdr *);
    Int32 WpcapWrapper::PcapNext(Guid pcapInstId, PcapPktHdr^ pcapPktHdr, MemoryStream^ ms);

    // PCAP_API int 	pcap_next_ex(pcap_t *, struct pcap_pkthdr **, const u_char **);
    Int32 WpcapWrapper::PcapNextEx(Guid pcapInstId, PcapPktHdr^ pcapPktHdr, MemoryStream^ ms);

    // PCAP_API void	pcap_breakloop(pcap_t *);
    Int32 WpcapWrapper::PcapBreakLoop(Guid pcapInstanceId);

    // PCAP_API int	pcap_stats(pcap_t *, struct pcap_stat *);
    Int32 PcapStats(IntPtr pcapHandle, PcapStat^ pcapStats);

    // PCAP_API int	pcap_setfilter(pcap_t *, struct bpf_program *);
    Int32 PcapSetFilter(IntPtr pcapHandle, IntPtr bpfProgram);

    // PCAP_API int 	pcap_setdirection(pcap_t *, pcap_direction_t);
    Int32 PcapSetDirection(IntPtr pcapHandle, PcapDirection pcapDirection);

    // PCAP_API int	pcap_getnonblock(pcap_t *, char *);
    Int32 WpcapWrapper::PcapGetNonBLock(Guid pcapInstId);

    // PCAP_API int	pcap_setnonblock(pcap_t *, int, char *);
    Int32 PcapSetNonBlock(IntPtr pcapHandle, Int32 nonBlock);

    // PCAP_API int	pcap_inject(pcap_t *, const void *, size_t);
    Int32 PcapInject(IntPtr pcapHandle, array<Byte>^ packet);

    // PCAP_API int	pcap_sendpacket(pcap_t *, const u_char *, int);
    Int32 WpcapWrapper::PcapSendPacket(Guid pcapInstId, array<Byte>^ packet);

    // PCAP_API const char *pcap_statustostr(int);
    String^ PcapStatusToStr(Int32 error);

    // PCAP_API const char *pcap_strerror(int);
    //const char* PcapStrError(int);
    
    // PCAP_API char	*pcap_geterr(pcap_t *);
    //char* PcapGetError(pcap_t*);
    
    // PCAP_API void	pcap_perror(pcap_t *, const char *);
    //void PcapPerror(pcap_t*);
    
    // PCAP_API int	pcap_compile(pcap_t *, struct bpf_program *, const char *, int, bpf_u_int32);
    Int32 WpcapWrapper::PcapCompile(Guid instanceId, Guid^ bpfProgramId, String^ filterStr, Int32 optimize, UInt32 netmask);

    // int pcap_compile_nopcap (int snaplen_arg, int linktype_arg, struct bpf_program *program, char * buf, int optimize, bpf_u_int32 mask)
    Int32 WpcapWrapper::PcapCompileNoPcap(Int32 snaplen, Int32 linkType, Guid^ bpfProgramId, String^ filterStr, Int32 optimize, UInt32 mask);

    Int32 PcapCreateSrcStr(String ^ src, Int32 type, String ^ host, String ^ port, String ^ name);
    
    // PCAP_API int	pcap_compile_nopcap(int, int, struct bpf_program *, const char *, int, bpf_u_int32);
    //int PcapCompileNoPcap(int, int, struct bpf_program*, const char*, int, bpf_u_int32);
    
    // PCAP_API void	pcap_freecode(struct bpf_program *);
    Int32 WpcapWrapper::PcapFreeCode(Guid bpfInstId);
    
    // PCAP_API int	pcap_offline_filter(const struct bpf_program *, const struct pcap_pkthdr *, const u_char *);
    Int32 WpcapWrapper::PcapOfflineFilter(Guid bpfInstId, PcapPktHdr^ pktHdr, array<Byte>^ pkt);
    
    // PCAP_API int	pcap_datalink(pcap_t *);
    Int32 PcapDatalink(Guid instanceId);
    
    // PCAP_API int	pcap_datalink_ext(pcap_t *);
    //int PcapDataLinkExt(pcap_t*);
    
    // PCAP_API int	pcap_list_datalinks(pcap_t *, int **);
    Int32 WpcapWrapper::PcapListDatalinks(Guid pcapInstId, List<Int32>^ datalinks);
    
    // PCAP_API int	pcap_set_datalink(pcap_t *, int);
    Int32 PcapSetDatalink(IntPtr pcapHandle, Int32 datalinkType);
    
    // PCAP_API void	pcap_free_datalinks(int *);
    //void PcapFreeDataLinks(int*);
    
    // PCAP_API int	pcap_datalink_name_to_val(const char *);
    Int32 PcapDatalinkNameToVal(String^ name);
    
    // PCAP_API const char *pcap_datalink_val_to_name(int);
    String^ PcapDatalinkValToName(Int32 datalinkType);
    
    // PCAP_API const char *pcap_datalink_val_to_description(int);
    String^ PcapDatalinkValToDescription(Int32 datalinkType);
    
    // PCAP_API int	pcap_snapshot(pcap_t *);
    Int32 PcapSnapshot(IntPtr pcapHandle);
    
    // PCAP_API int	pcap_is_swapped(pcap_t *);
    Int32 WpcapWrapper::PcapIsSwapped(Guid pcapInstId);
    
    // PCAP_API int	pcap_major_version(pcap_t *);
    Int32 WpcapWrapper::PcapMajorVersion(Guid pcapInstId);
    
    // PCAP_API int	pcap_minor_version(pcap_t *);
    Int32 WpcapWrapper::PcapMinorVersion(Guid pcapInstId);
    
    // PCAP_API FILE	*pcap_file(pcap_t *);
    IntPtr PcapFile(IntPtr pcapHandle);
    
    // PCAP_API int	pcap_fileno(pcap_t *);
    //int PcapFileNo(pcap_t*);
    
    // PCAP_API int	pcap_wsockinit(void);
    //int PcapWsockInit(void);
    
    // PCAP_API pcap_dumper_t *pcap_dump_open(pcap_t *, const char *);
    Int32 WpcapWrapper::PcapDumpOpen(Guid pcapInstId, String^ fname, Guid^ dumperInstId);
    
    //PCAP_API pcap_dumper_t *pcap_dump_fopen(pcap_t *, FILE *fp);
    IntPtr PcapDumpFopen(IntPtr pcapHandle, IntPtr fp);
    
    //PCAP_API pcap_dumper_t *pcap_dump_open_append(pcap_t *, const char *);
    IntPtr PcapDumpOpenAppend(IntPtr pcapHandle, String^ fname);
    
    //PCAP_API FILE	*pcap_dump_file(pcap_dumper_t *);
    Int32 WpcapWrapper::PcapDumpFile(Guid instanceId, Guid^ fileInstanceId);
    
    //PCAP_API long	pcap_dump_ftell(pcap_dumper_t *);
    Int32 PcapDumpFtell(IntPtr pcapDumper);
    
    //PCAP_API int	pcap_dump_flush(pcap_dumper_t *);
    Int32 WpcapWrapper::PcapDumpFlush(Guid instanceId);
    
    //PCAP_API void	pcap_dump_close(pcap_dumper_t *);
    Int32 WpcapWrapper::PcapDumpClose(Guid instanceId);
    
    //PCAP_API void	pcap_dump(u_char *, const struct pcap_pkthdr *, const u_char *);
    Int32 WpcapWrapper::PcapDump(Guid instanceId, PcapPktHdr^ pktHdr, array<Byte>^ pkt);
    
    //PCAP_API int	pcap_findalldevs(pcap_if_t **, char *);
    Int32 PcapFindAllDevs(List<PcapDeviceInfo^>^ devices);
    
    //PCAP_API void	pcap_freealldevs(pcap_if_t *);
    /*void PcapFreeAllDevs(pcap_if_t*);*/
    
    //PCAP_API const char *pcap_lib_version(void);
    String^ PcapLibVersion();

    /*Int32 WpcapWrapper::PcapLiveDump(Guid pcapInstId, String^ fileName, Int32 maxSize, Int32 maxPacks);*/
    
    // PCAP_API u_int	bpf_filter(const struct bpf_insn *, const u_char *, u_int, u_int);
    //u_int BpfFilter(const struct bpf_insn*, const u_char*, u_int, u_int);
    
    //PCAP_API int	bpf_validate(const struct bpf_insn *f, int len);
    //int BpfValidate(const struct bpf_insn *f, int len);
    
    //PCAP_API char	*bpf_image(const struct bpf_insn *, int);
    //char* BpfImage(const struct bpf_insn*, int);
    
    //PCAP_API void	bpf_dump(const struct bpf_program *, int);
    //void BpfDump(const struct bpf_program*, int);
    
    //PCAP_API int pcap_setbuff(pcap_t *p, int dim);
    //int PcapSetBuff(pcap_t* p, int dim);
    
    //PCAP_API int pcap_setmode(pcap_t *p, int mode);
    //int PcapSetMode(pcap_t* p, int mode);
    
    //PCAP_API int pcap_setmintocopy(pcap_t *p, int size);
    //int PcapSetMinToCopy(pcap_t* p, int size);

    //PCAP_API HANDLE pcap_getevent(pcap_t *p);
    //HANDLE PcapGetEvent(pcap_t* p);

    //PCAP_API int pcap_oid_get_request(pcap_t *, bpf_u_int32, void *, size_t *);
    //int PcapOldGetRequest(pcap_t*, bpf_u_int32, void*, size_t*);

    //PCAP_API int pcap_oid_set_request(pcap_t *, bpf_u_int32, const void *, size_t *);
    //int PcapOldSetRequest(pcap_t*, bpf_u_int32, const void*, size_t*);

    //PCAP_API pcap_send_queue* pcap_sendqueue_alloc(u_int memsize);
    //pcap_send_queue* PcapSendQueueAlloc(u_int memsize);

    //PCAP_API void pcap_sendqueue_destroy(pcap_send_queue* queue);
    //void PcapSendQueueDestroy(pcap_send_queue* queue);

    //PCAP_API int pcap_sendqueue_queue(pcap_send_queue* queue, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data);
    //int PcapSendQueueQueue(pcap_send_queue* queue, const struct pcap_pkthdr *pkt_header, const u_char* pkt_data);

    //PCAP_API u_int pcap_sendqueue_transmit(pcap_t *p, pcap_send_queue* queue, int sync);
    //u_int PcapSendQueueTransmit(pcap_t *p, pcap_send_queue* queue, int sync);

    //PCAP_API struct pcap_stat *pcap_stats_ex(pcap_t *p, int *pcap_stat_size);
    //struct pcap_stat* PcapStatsEx(pcap_t* p, int *pcap_stat_size);

    //PCAP_API int pcap_setuserbuffer(pcap_t *p, int size);
    //int PcapSetUserBuffer(pcap_t* p, int size);

    //PCAP_API int pcap_live_dump(pcap_t *p, char *filename, int maxsize, int maxpacks);
    //int PcapLiveDump(pcap_t* p, char* filename, int maxsize, int maxpacks);

    //PCAP_API int pcap_live_dump_ended(pcap_t *p, int sync);
    //int PcapLiveDumpEnded(pcap_t* p, int sync);

    //PCAP_API int pcap_start_oem(char* err_str, int flags);
    //int PcapStartOem(char* err_str, int flags);

    //PCAP_API PAirpcapHandle pcap_get_airpcap_handle(pcap_t *p);
    //PAirpcapHandle PcapGetAirPcapHandle(pcap_t* p);

    //
    Int32 WpcapWrapper::PcapOpen(String^ source, Int32 snapLen, Int32 flags, Int32 readTimeout, Guid^ pcapInstId);

    //
    Int32 WpcapWrapper::PcapParseSrcStr(String^ source, Int32^ type, String^ host, String^ port, String^ name);

    // TODO: pcap_remoteact_accept

    // TODO: pcap_remoteact_cleanup

    // TODO: pcap_remoteact_close

    // TODO: pcap_remoteact_list

private:
    Dictionary<Guid, PcapInstance^>^ pcapInstances;
    Dictionary<Guid, BpfProgram^>^ bpfProgramInstances;
    Dictionary<Guid, DumperInstance^>^ dumperInstances;
    Dictionary<Guid, FileInstance^>^ fileInstances;
};
