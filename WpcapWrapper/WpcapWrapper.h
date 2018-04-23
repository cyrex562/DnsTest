#pragma once

using namespace System;

namespace WpcapWrapper {
    public ref class Class1
    {
    public:
        // PCAP_API char	*pcap_lookupdev(char *);
        char* PcapLokupDev(char*);
        // PCAP_API int	pcap_lookupnet(const char *, bpf_u_int32 *, bpf_u_int32 *, char *);
        int PcapLookupNet(const char*, bpf_u_int32*, bpf_u_int32*, char*);
        // PCAP_API pcap_t	*pcap_create(const char *, char *);
        int PcapCreate(const char *, char *);
        // PCAP_API int	pcap_set_snaplen(pcap_t *, int);
        int PcapSetSnaplen(pcap_t*, int);
        // PCAP_API int	pcap_set_promisc(pcap_t *, int);
        int PcapSetPromisc(pcap_t*, int);
        // pcap_set_rfmon
        // PCAP_API int	pcap_can_set_rfmon(pcap_t *);
        // PCAP_API int	pcap_set_rfmon(pcap_t *, int);
        // PCAP_API int	pcap_set_timeout(pcap_t *, int);
        int PcapSetTimeout(pcap_t*, int);
        // PCAP_API int	pcap_set_tstamp_type(pcap_t *, int);
        int PcapSetTstampType(pcap_t*, int);
        // PCAP_API int	pcap_set_immediate_mode(pcap_t *, int);
        int PcapSetImmediateMode(pcap_t*, int);
        // PCAP_API int	pcap_set_buffer_size(pcap_t *, int);
        int PcapSetBufferSize(pcap_t*, int);
        // PCAP_API int	pcap_set_tstamp_precision(pcap_t *, int);
        int PcapSetTstampPrecision(pcap_t*, int);
        // PCAP_API int	pcap_get_tstamp_precision(pcap_t *);
        int PcapGetTstampPrecision(pcap_t*, int);
        // PCAP_API int	pcap_activate(pcap_t *);
        int PcapActivate(pcap_t*);
        // PCAP_API int	pcap_list_tstamp_types(pcap_t *, int **);
        int PcapListTstampTypes(pcap_t*, int **);
        // PCAP_API void	pcap_free_tstamp_types(int *);
        void PcapFreeTstampTypes(int*);
        // PCAP_API int	pcap_tstamp_type_name_to_val(const char *);
        int PcapTstampTypeNameToVal(const char*);
        // PCAP_API const char *pcap_tstamp_type_val_to_name(int);
        const char* PcapTstampTypeValToName(int);
        // PCAP_API const char *pcap_tstamp_type_val_to_description(int);
        const char* PcapTstampTypeValToDesc(int);
        // PCAP_API pcap_t	*pcap_open_live(const char *, int, int, int, char *);
        pcap_t *PcapOpenLive(const char*, int, int, int, char *);
        // PCAP_API pcap_t	*pcap_open_dead(int, int);
        pcap_t *PcapOpenDead(int, int);
        // PCAP_API pcap_t	*pcap_open_dead_with_tstamp_precision(int, int, u_int);
        pcap_t *PcapOpenDeadWithTstampPrecision(int, int, unsigned int);
        // PCAP_API pcap_t	*pcap_open_offline_with_tstamp_precision(const char *, u_int, char *);
        pcap_t *PcapOpenOfflineWithTstampPrecision(const char*, unsigned int, char*);
        // PCAP_API pcap_t	*pcap_open_offline(const char *, char *);
        pcap_t *PcapOpenOffline(const char*, char*);
        // PCAP_API pcap_t  *pcap_hopen_offline_with_tstamp_precision(intptr_t, u_int, char *);
        pcap_t *PcapHopenOfflineWithTstampPrecision(intptr_t, u_int, char*);
        // PCAP_API pcap_t  *pcap_hopen_offline(intptr_t, char *);
        pcap_t *PcapHopenOffline(intptr_t, char*);
        // PCAP_API void	pcap_close(pcap_t *);
        void PcapClose(pcap_t *);
        // PCAP_API int	pcap_loop(pcap_t *, int, pcap_handler, u_char *);
        int PcapLoop(pcap_t *, int, pcap_handler, u_char*);
        // PCAP_API int	pcap_dispatch(pcap_t *, int, pcap_handler, u_char *);
        int PcapDispatch(pcap_t*, int, pcap_handler, u_char*);
        // PCAP_API const u_char *pcap_next(pcap_t *, struct pcap_pkthdr *);
        int PcapNext(pcap_t*, struct pcap_pkthdr*);
        // PCAP_API int 	pcap_next_ex(pcap_t *, struct pcap_pkthdr **, const u_char **);
        int PcapNextEx(pcap_t*, struct pcap_pkthdr**, const u_char**);
        // PCAP_API void	pcap_breakloop(pcap_t *);
        void PcapBreakLoop(pcap_t*);
        // PCAP_API int	pcap_stats(pcap_t *, struct pcap_stat *);
        int PcapStats(pcap_t*, struct pcap_stat*);
        // PCAP_API int	pcap_setfilter(pcap_t *, struct bpf_program *);
        int PcapSetFilter(pcap_t*, struct bpf_program*);
        // PCAP_API int 	pcap_setdirection(pcap_t *, pcap_direction_t);
        int PcapSetDirection(pcap_t*, pcap_direction_t);
        // PCAP_API int	pcap_getnonblock(pcap_t *, char *);
        int PcapGetNonBlock(pcap_t*, char*);
        // PCAP_API int	pcap_setnonblock(pcap_t *, int, char *);
        int PcapSetNonBlock(pcap_t*, int, char*);
        // PCAP_API int	pcap_inject(pcap_t *, const void *, size_t);
        int PcapInject(pcap_t*, const void*, size_t);
        // PCAP_API int	pcap_sendpacket(pcap_t *, const u_char *, int);
        int PcapSendPacket(pcap_t*, const u_char*, int);
        // PCAP_API const char *pcap_statustostr(int);
        const char* PcapStatusToStr(int);
        // PCAP_API const char *pcap_statustostr(int);
        PCAP_API const char *pcap_strerror(int);
        const char* PcapStrError(int);
        // PCAP_API char	*pcap_geterr(pcap_t *);
        char* PcapGetError(pcap_t*);
        // PCAP_API void	pcap_perror(pcap_t *, const char *);
        void PcapPerror(pcap_t*);
        // PCAP_API int	pcap_compile(pcap_t *, struct bpf_program *, const char *, int, bpf_u_int32);
        int PcapCompile(pcap_t*, struct bpf_program*, const char*, int, bpf_u_int32);
        // PCAP_API int	pcap_compile_nopcap(int, int, struct bpf_program *, const char *, int, bpf_u_int32);
        int PcapCompileNoPcap(int, int, struct bpf_program*, const char*, int, bpf_u_int32);
        // PCAP_API void	pcap_freecode(struct bpf_program *);
        void PcapFreeCode(struct bpf_program*);
        // PCAP_API int	pcap_offline_filter(const struct bpf_program *, const struct pcap_pkthdr *, const u_char *);
        int PcapOfflineFilter(const struct bpf_program*, const struct pcap_pkthdr*, const u_char*);
        // PCAP_API int	pcap_datalink(pcap_t *);
        int PcapDataLink(pcap_t*);
        // PCAP_API int	pcap_datalink_ext(pcap_t *);
        int PcapDataLinkExt(pcap_t*);
        // PCAP_API int	pcap_list_datalinks(pcap_t *, int **);
        int PcapListDataLinks(pcap_t*, int**);
        // PCAP_API int	pcap_set_datalink(pcap_t *, int);
        int PcapSetDataLink(pcap_t*, int);
        // PCAP_API void	pcap_free_datalinks(int *);
        void PcapFreeDataLinks(int*);
        // PCAP_API int	pcap_datalink_name_to_val(const char *);
        int PcapDataLinkNameToVal(const char*);
        // PCAP_API const char *pcap_datalink_val_to_name(int);
        const char* PcapDataLinkValToName(int);
        // PCAP_API const char *pcap_datalink_val_to_description(int);
        const char* PcapDataLinkValToDescritpion(int);
        // PCAP_API int	pcap_snapshot(pcap_t *);
        int PcapSnapshot(pcap_t*);
        // PCAP_API int	pcap_is_swapped(pcap_t *);
        int PcapIsSwapped(pcap_t*);
        // PCAP_API int	pcap_major_version(pcap_t *);
        int PcapMajorVersion(pcap_t*);
        // PCAP_API int	pcap_minor_version(pcap_t *);
        int PcapMinorVersion(pcap_t*);
        // PCAP_API FILE	*pcap_file(pcap_t *);
        FILE* PcapFile(pcap_t*);
        // PCAP_API int	pcap_fileno(pcap_t *);
        int PcapFileNo(pcap_t*);
        // PCAP_API int	pcap_wsockinit(void);
        int PcapWsockInit(void);
        // PCAP_API pcap_dumper_t *pcap_dump_open(pcap_t *, const char *);
        pcap_dumper_t *PcapDumpOpen(pcap_t*, const char*);
        //PCAP_API pcap_dumper_t *pcap_dump_fopen(pcap_t *, FILE *fp);
        pcap_dumper_t *PcapDumpFopen(pcap_t*, FILE *fp);
        //PCAP_API pcap_dumper_t *pcap_dump_open_append(pcap_t *, const char *);
        pcap_dumper_t* PcapDumpOpenAppend(pcap_t*, const char*);
        //PCAP_API FILE	*pcap_dump_file(pcap_dumper_t *);
        FILE* PcapDumpFile(pcap_dumper_t*);
        //PCAP_API long	pcap_dump_ftell(pcap_dumper_t *);
        long PcapDumpFtell(pcap_dumper_t*);
        //PCAP_API int	pcap_dump_flush(pcap_dumper_t *);
        int PcapDumpFlush(pcap_dumper_t*);
        //PCAP_API void	pcap_dump_close(pcap_dumper_t *);
        void PcapDumpClose(pcap_dumper_t*);
        //PCAP_API void	pcap_dump(u_char *, const struct pcap_pkthdr *, const u_char *);
        void PcapDump(u_char*, const struct pcap_pkthdr*, const u_char*);
        //PCAP_API int	pcap_findalldevs(pcap_if_t **, char *);
        int PcapFindAllDevs(pcap_if_t**, char*);
        //PCAP_API void	pcap_freealldevs(pcap_if_t *);
        void PcapFreeAllDevs(pcap_if_t*);
        //PCAP_API const char *pcap_lib_version(void);
        const char* PcapLibVersion(void);
        // PCAP_API u_int	bpf_filter(const struct bpf_insn *, const u_char *, u_int, u_int);
        u_int BpfFilter(const struct bpf_insn*, const u_char*, u_int, u_int);
        //PCAP_API int	bpf_validate(const struct bpf_insn *f, int len);
        int BpfValidate(const struct bpf_insn *f, int len);
        //PCAP_API char	*bpf_image(const struct bpf_insn *, int);
        char* BpfImage(const struct bpf_insn*, int);
        //PCAP_API void	bpf_dump(const struct bpf_program *, int);
        void BpfDump(const struct bpf_program*, int);
        //PCAP_API int pcap_setbuff(pcap_t *p, int dim);
        int PcapSetBuff(pcap_t* p, int dim);
        //PCAP_API int pcap_setmode(pcap_t *p, int mode);
        int PcapSetMode(pcap_t* p, int mode);
        //PCAP_API int pcap_setmintocopy(pcap_t *p, int size);
        int PcapSetMinToCopy(pcap_t* p, int size);

        //PCAP_API HANDLE pcap_getevent(pcap_t *p);
        HANDLE PcapGetEvent(pcap_t* p);

        //PCAP_API int pcap_oid_get_request(pcap_t *, bpf_u_int32, void *, size_t *);
        int PcapOldGetRequest(pcap_t*, bpf_u_int32, void*, size_t*);

        //PCAP_API int pcap_oid_set_request(pcap_t *, bpf_u_int32, const void *, size_t *);
        int PcapOldSetRequest(pcap_t*, bpf_u_int32, const void*, size_t*);

        //PCAP_API pcap_send_queue* pcap_sendqueue_alloc(u_int memsize);
        pcap_send_queue* PcapSendQueueAlloc(u_int memsize);

        //PCAP_API void pcap_sendqueue_destroy(pcap_send_queue* queue);
        void PcapSendQueueDestroy(pcap_send_queue* queue);

        //PCAP_API int pcap_sendqueue_queue(pcap_send_queue* queue, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data);
        int PcapSendQueueQueue(pcap_send_queue* queue, const struct pcap_pkthdr *pkt_header, const u_char* pkt_data);

        //PCAP_API u_int pcap_sendqueue_transmit(pcap_t *p, pcap_send_queue* queue, int sync);
        u_int PcapSendQueueTransmit(pcap_t *p, pcap_send_queue* queue, int sync);

        //PCAP_API struct pcap_stat *pcap_stats_ex(pcap_t *p, int *pcap_stat_size);
        struct pcap_stat* PcapStatsEx(pcap_t* p, int *pcap_stat_size);

        //PCAP_API int pcap_setuserbuffer(pcap_t *p, int size);
        int PcapSetUserBuffer(pcap_t* p, int size);

        //PCAP_API int pcap_live_dump(pcap_t *p, char *filename, int maxsize, int maxpacks);
        int PcapLiveDump(pcap_t* p, char* filename, int maxsize, int maxpacks);

        //PCAP_API int pcap_live_dump_ended(pcap_t *p, int sync);
        int PcapLiveDumpEnded(pcap_t* p, int sync);

        //PCAP_API int pcap_start_oem(char* err_str, int flags);
        int PcapStartOem(char* err_str, int flags);

        //PCAP_API PAirpcapHandle pcap_get_airpcap_handle(pcap_t *p);
        PAirpcapHandle PcapGetAirPcapHandle(pcap_t* p);
    };
}
