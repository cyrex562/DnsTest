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

public ref class BpfInsn {
public:
    UInt16 code;
    Byte jt;
    Byte jf;
    UInt32 k;
};

public ref class BpfProgram {
public:
    UInt32 bfLen;
    BpfInsn^ bfInsns;
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

public ref class PcapLookupNetResult {
public:
    UInt32 network;
    UInt32 mask;
};

public delegate void PcapCallbackDelegate(Byte user[], PcapPktHdr^ pktHdr, Byte packet[]);

public ref class PcapWrapper
{
public:

    PcapWrapper();

    // obsolete
    // PCAP_API char	*pcap_lookupdev(char *);
    //String^ PcapLookupDev();

    // PCAP_API int	pcap_lookupnet(const char *, bpf_u_int32 *, bpf_u_int32 *, char *);
    Int32 PcapLookupNet(String^ device, PcapLookupNetResult^ result);

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

    // PCAP_API pcap_t	*pcap_open_live(const char *, int, int, int, char *);
    //pcap_t *PcapOpenLive(const char*, int, int, int, char *);

    // PCAP_API pcap_t	*pcap_open_dead(int, int);
    IntPtr PcapOpenDead(Int32 linkType, Int32 snaplen);

    // PCAP_API pcap_t	*pcap_open_dead_with_tstamp_precision(int, int, u_int);
    IntPtr PcapOpenDeadWithTstampPrecision(Int32 linkType, Int32 snaplen, UInt32 precision);

    // PCAP_API pcap_t	*pcap_open_offline_with_tstamp_precision(const char *, u_int, char *);
    IntPtr PcapOpenOfflineWithTstampPrecision(String^ fname, UInt32 precision);

    // PCAP_API pcap_t	*pcap_open_offline(const char *, char *);
    IntPtr PcapOpenOffline(String^ fname);

    // PCAP_API pcap_t  *pcap_hopen_offline_with_tstamp_precision(intptr_t, u_int, char *);
    IntPtr PcapHopenOfflineWithTstampPrecision(IntPtr desc, UInt32 precision);

    // PCAP_API pcap_t  *pcap_hopen_offline(intptr_t, char *);
    IntPtr PcapHopenOffline(IntPtr desc);

    // PCAP_API void	pcap_close(pcap_t *);
    void PcapClose(IntPtr pcapHandle);

    // PCAP_API int	pcap_loop(pcap_t *, int, pcap_handler, u_char *);
    Int32 PcapLoop(IntPtr pcapHandle, Int32 count, PcapCallbackDelegate^ d, Byte user[]);

    // PCAP_API int	pcap_dispatch(pcap_t *, int, pcap_handler, u_char *);
    Int32 PcapDispatch(IntPtr pcapHandle, Int32 count, PcapCallbackDelegate^ d, Byte user[]);

    // PCAP_API const u_char *pcap_next(pcap_t *, struct pcap_pkthdr *);
    array<Byte>^ PcapWrapper::PcapNext(IntPtr pcapHandle, PcapPktHdr^ pcapPktHdr);

    // PCAP_API int 	pcap_next_ex(pcap_t *, struct pcap_pkthdr **, const u_char **);
    Int32 PcapNextEx(IntPtr pcapHandle, PcapPktHdr^ pcapPktHdr, array<Byte>^ pktData);

    // PCAP_API void	pcap_breakloop(pcap_t *);
    void PcapBreakLoop(IntPtr pcapHandle);

    // PCAP_API int	pcap_stats(pcap_t *, struct pcap_stat *);
    Int32 PcapStats(IntPtr pcapHandle, PcapStat^ pcapStats);

    // PCAP_API int	pcap_setfilter(pcap_t *, struct bpf_program *);
    Int32 PcapSetFilter(IntPtr pcapHandle, IntPtr bpfProgram);

    // PCAP_API int 	pcap_setdirection(pcap_t *, pcap_direction_t);
    Int32 PcapSetDirection(IntPtr pcapHandle, PcapDirection pcapDirection);

    // PCAP_API int	pcap_getnonblock(pcap_t *, char *);
    Int32 PcapGetNonBLock(IntPtr pcapHandle);

    // PCAP_API int	pcap_setnonblock(pcap_t *, int, char *);
    Int32 PcapSetNonBlock(IntPtr pcapHandle, Int32 nonBlock);

    // PCAP_API int	pcap_inject(pcap_t *, const void *, size_t);
    Int32 PcapInject(IntPtr pcapHandle, array<Byte>^ packet);

    // PCAP_API int	pcap_sendpacket(pcap_t *, const u_char *, int);
    Int32 PcapSendPacket(IntPtr pcapHandle, array<Byte>^ packet);

    // PCAP_API const char *pcap_statustostr(int);
    String^ PcapStatusToStr(Int32 error);

    // PCAP_API const char *pcap_strerror(int);
    //const char* PcapStrError(int);
    
    // PCAP_API char	*pcap_geterr(pcap_t *);
    //char* PcapGetError(pcap_t*);
    
    // PCAP_API void	pcap_perror(pcap_t *, const char *);
    //void PcapPerror(pcap_t*);
    
    // PCAP_API int	pcap_compile(pcap_t *, struct bpf_program *, const char *, int, bpf_u_int32);
    Int32 PcapCompile(IntPtr pcapHandle, IntPtr bpfProgram, String^ filterStr, Int32 optimize, UInt32 netmask);
    
    // PCAP_API int	pcap_compile_nopcap(int, int, struct bpf_program *, const char *, int, bpf_u_int32);
    //int PcapCompileNoPcap(int, int, struct bpf_program*, const char*, int, bpf_u_int32);
    
    // PCAP_API void	pcap_freecode(struct bpf_program *);
    void PcapFreeCode(IntPtr bpfProgram);
    
    // PCAP_API int	pcap_offline_filter(const struct bpf_program *, const struct pcap_pkthdr *, const u_char *);
    Int32 PcapOfflineFilter(IntPtr bpfProgram, PcapPktHdr^ pktHdr, array<Byte>^ pkt);
    
    // PCAP_API int	pcap_datalink(pcap_t *);
    Int32 PcapDatalink(IntPtr pcapHandle);
    
    // PCAP_API int	pcap_datalink_ext(pcap_t *);
    //int PcapDataLinkExt(pcap_t*);
    
    // PCAP_API int	pcap_list_datalinks(pcap_t *, int **);
    Int32 PcapListDatalinks(IntPtr pcapHandle, List<Int32>^ datalinks);
    
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
    Int32 PcapIsSwapped(IntPtr pcapHandle);
    
    // PCAP_API int	pcap_major_version(pcap_t *);
    Int32 PcapMajorVersion(IntPtr pcapHandle);
    
    // PCAP_API int	pcap_minor_version(pcap_t *);
    Int32 PcapMinorVersion(IntPtr pcapHandle);
    
    // PCAP_API FILE	*pcap_file(pcap_t *);
    IntPtr PcapFile(IntPtr pcapHandle);
    
    // PCAP_API int	pcap_fileno(pcap_t *);
    //int PcapFileNo(pcap_t*);
    
    // PCAP_API int	pcap_wsockinit(void);
    //int PcapWsockInit(void);
    
    // PCAP_API pcap_dumper_t *pcap_dump_open(pcap_t *, const char *);
    IntPtr PcapDumpOpen(IntPtr pcapHandle, String^ fname);
    
    //PCAP_API pcap_dumper_t *pcap_dump_fopen(pcap_t *, FILE *fp);
    IntPtr PcapDumpFopen(IntPtr pcapHandle, IntPtr fp);
    
    //PCAP_API pcap_dumper_t *pcap_dump_open_append(pcap_t *, const char *);
    IntPtr PcapDumpOpenAppend(IntPtr pcapHandle, String^ fname);
    
    //PCAP_API FILE	*pcap_dump_file(pcap_dumper_t *);
    IntPtr PcapDumpFile(IntPtr pcapDumper);
    
    //PCAP_API long	pcap_dump_ftell(pcap_dumper_t *);
    Int32 PcapDumpFtell(IntPtr pcapDumper);
    
    //PCAP_API int	pcap_dump_flush(pcap_dumper_t *);
    Int32 PcapDumpFlush(IntPtr pcapDumper);
    
    //PCAP_API void	pcap_dump_close(pcap_dumper_t *);
    void PcapDumpClose(IntPtr pcapDumper);
    
    //PCAP_API void	pcap_dump(u_char *, const struct pcap_pkthdr *, const u_char *);
    void PcapDump(IntPtr user, PcapPktHdr^ pktHdr, array<Byte>^ pkt);
    
    //PCAP_API int	pcap_findalldevs(pcap_if_t **, char *);
    Int32 PcapFindAllDevs(List<PcapDeviceInfo^>^ devices);
    
    //PCAP_API void	pcap_freealldevs(pcap_if_t *);
    /*void PcapFreeAllDevs(pcap_if_t*);*/
    
    //PCAP_API const char *pcap_lib_version(void);
    String^ PcapLibVersion();
    
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
};
