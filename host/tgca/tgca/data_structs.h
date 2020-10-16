#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H
#include <QtGlobal>
#include "registers.h"

class BaseReg
{
public:
    BaseReg(quint32 address) : address(address) {  }
    const quint32 address;
    void setData(quint32& data) {
        *((quint32*)this+1) = data;
    }
    quint32 getData() {
        return *((quint32*)this+1);
    }
};

//------------------------------ HSC --------------------------------------

class REG_HSC_ram_tx_rx : public BaseReg
{
public:
    REG_HSC_ram_tx_rx(quint32 buf_1 = 0, quint32 buf_2 = 0, quint32 preamble_1_mem = 0, quint32 preamble_2_mem = 0, quint32 one_per_HF = 0, quint32 MSEQ = 0, quint32 K_per_M = 0, quint32 KF = 0, quint32 mem_QAM16_QAM64 = 0) : BaseReg(REG_VSK_ram_tx_rx),
        buf_1(buf_1), buf_2(buf_2), preamble_1_mem(preamble_1_mem), preamble_2_mem(preamble_2_mem), one_per_HF(one_per_HF), MSEQ(MSEQ), K_per_M(K_per_M), KF(KF), mem_QAM16_QAM64(mem_QAM16_QAM64) { }
    quint32 buf_1 : 1;
    quint32 buf_2 : 1;
    quint32 preamble_1_mem : 1;
    quint32 preamble_2_mem : 1;
    quint32 one_per_HF : 1;
    quint32 MSEQ : 1;
    quint32 K_per_M : 1;
    quint32 KF : 1;
    quint32 mem_QAM16_QAM64 : 1;
};

class REG_HSC_id : public BaseReg
{
public:
    REG_HSC_id(quint32 data = 0) : BaseReg(REG_VSK_id),
        data(data) { }
    quint32 data;
};

class REG_HSC_status : public BaseReg
{
public:
    REG_HSC_status(quint32 rt_bc_int = 0, quint32 rs_err = 0, quint32 no_aw_err = 0, quint32 yes_aw_gr_err = 0, quint32 tx_num_buf = 0, quint32 rx_num_buf = 0) : BaseReg(REG_VSK_status),
        rt_bc_int(rt_bc_int), rs_err(rs_err), no_aw_err(no_aw_err), yes_aw_gr_err(yes_aw_gr_err), tx_num_buf(tx_num_buf), rx_num_buf(rx_num_buf) { }
    quint32 rt_bc_int : 1;
    quint32 : 3;
    quint32 rs_err : 1;
    quint32 no_aw_err : 1;
    quint32 yes_aw_gr_err : 1;
    quint32 : 3;
    quint32 tx_num_buf : 1;
    quint32 rx_num_buf : 1;
};

class REG_HSC_cfg : public BaseReg
{
public:
    REG_HSC_cfg(quint32 type_man = 0, quint32 ena_codec = 0, quint32 ena_aru = 0, quint32 ena_mem_vsk = 0, quint32 rtavsk_ena = 0, quint32 rtavsk = 0, quint32 rt_bc = 0, quint32 en_rt_bc_int = 0) : BaseReg(REG_VSK_cfg),
        type_man(type_man), ena_codec(ena_codec), ena_aru(ena_aru), ena_mem_vsk(ena_mem_vsk), rtavsk_ena(rtavsk_ena), rtavsk(rtavsk), rt_bc(rt_bc), en_rt_bc_int(en_rt_bc_int) { }
    quint32 type_man : 2;
    quint32 ena_codec : 1;
    quint32 ena_aru : 1;
    quint32 ena_mem_vsk : 1;
    quint32 : 1;
    quint32 rtavsk_ena : 1;
    quint32 rtavsk : 5;
    quint32 rt_bc : 1;
    quint32 en_rt_bc_int : 1;
};

class REG_HSC_tx_cntr : public BaseReg
{
public:
    REG_HSC_tx_cntr(quint32 data = 0) : BaseReg(REG_VSK_tx_cntr),
        data(data) { }
    quint32 data;
};

class REG_HSC_rx_cntr : public BaseReg
{
public:
    REG_HSC_rx_cntr(quint32 max_Rn_sync_pre = 0, quint32 prcs_max_sync__sync_kf = 0) : BaseReg(REG_VSK_rx_cntr),
        max_Rn_sync_pre(max_Rn_sync_pre), prcs_max_sync__sync_kf(prcs_max_sync__sync_kf) { }
    quint32 max_Rn_sync_pre : 1;
    quint32 prcs_max_sync__sync_kf : 1;
};

class REG_HSC_creg : public BaseReg
{
public:
    REG_HSC_creg(quint32 com_res = 0, quint32 tx_res = 0, quint32 rx_res = 0, quint32 start_bc = 0) : BaseReg(REG_VSK_creg),
        com_res(com_res), tx_res(tx_res), rx_res(rx_res), start_bc(start_bc) { }
    quint32 com_res : 1;
    quint32 tx_res : 1;
    quint32 rx_res : 1;
    quint32 : 1;
    quint32 start_bc : 1;
};

class REG_HSC_cr_spi : public BaseReg
{
public:
    REG_HSC_cr_spi(quint32 spi_en = 0, quint32 dr8_16_32 = 0, quint32 spif = 0) : BaseReg(REG_VSK_cr_spi),
        spi_en(spi_en), dr8_16_32(dr8_16_32), spif(spif) { }
    quint32 spi_en : 2;
    quint32 : 6;
    quint32 dr8_16_32 : 2;
    quint32 : 5;
    quint32 spif : 1;
};

class REG_HSC_dr_spi_msw : public BaseReg
{
public:
    REG_HSC_dr_spi_msw(quint32 data = 0) : BaseReg(REG_VSK_dr_spi_msw),
        data(data) { }
    quint32 data;
};

class REG_HSC_dr_spi_lsw : public BaseReg
{
public:
    REG_HSC_dr_spi_lsw(quint32 data = 0) : BaseReg(REG_VSK_dr_spi_lsw),
        data(data) { }
    quint32 data;
};

class REG_HSC_time_rsp : public BaseReg
{
public:
    REG_HSC_time_rsp(quint32 time = 0) : BaseReg(REG_VSK_time_rsp),
        time(time) { }
    quint32 time;
};

class REG_HSC_cnt_pct_tx_msw : public BaseReg
{
public:
    REG_HSC_cnt_pct_tx_msw(quint32 counter = 0) : BaseReg(REG_VSK_cnt_pct_tx_msw),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_cnt_pct_tx_lsw : public BaseReg
{
public:
    REG_HSC_cnt_pct_tx_lsw(quint32 counter = 0) : BaseReg(REG_VSK_cnt_pct_tx_lsw),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_cnt_pct_rx_msw : public BaseReg
{
public:
    REG_HSC_cnt_pct_rx_msw(quint32 counter = 0) : BaseReg(REG_VSK_cnt_pct_rx_msw),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_cnt_pct_rx_lsw : public BaseReg
{
public:
    REG_HSC_cnt_pct_rx_lsw(quint32 counter = 0) : BaseReg(REG_VSK_cnt_pct_rx_lsw),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_lvl_sync_kf_rx_msw : public BaseReg
{
public:
    REG_HSC_lvl_sync_kf_rx_msw(quint32 top = 0) : BaseReg(REG_VSK_lvl_sync_kf_rx_msw),
        top(top) { }
    quint32 top;
};

class REG_HSC_prcs_max_sync_msw : public BaseReg
{
public:
    REG_HSC_prcs_max_sync_msw(quint32 top = 0) : BaseReg(REG_VSK_prcs_max_sync_msw),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_sync_kf_rx_lsw : public BaseReg
{
public:
    REG_HSC_lvl_sync_kf_rx_lsw(quint32 top = 0) : BaseReg(REG_VSK_lvl_sync_kf_rx_lsw),
        top(top) { }
    quint32 top;
};

class REG_HSC_prcs_max_sync_lsw : public BaseReg
{
public:
    REG_HSC_prcs_max_sync_lsw(quint32 top = 0) : BaseReg(REG_VSK_prcs_max_sync_lsw),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_sync_pre_rx_msw : public BaseReg
{
public:
    REG_HSC_lvl_sync_pre_rx_msw(quint32 top = 0) : BaseReg(REG_VSK_lvl_sync_pre_rx_msw),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_sync_pre_rx_lsw : public BaseReg
{
public:
    REG_HSC_lvl_sync_pre_rx_lsw(quint32 top = 0) : BaseReg(REG_VSK_lvl_sync_pre_rx_lsw),
        top(top) { }
    quint32 top;
};

class REG_HSC_prs_level_max_Rn_msw : public BaseReg
{
public:
    REG_HSC_prs_level_max_Rn_msw(quint32 top = 0) : BaseReg(REG_VSK_prs_level_max_rn_msw),
        top(top) { }
    quint32 top;
};

class REG_HSC_prs_level_max_Rn_lsw : public BaseReg
{
public:
    REG_HSC_prs_level_max_Rn_lsw(quint32 top = 0) : BaseReg(REG_VSK_prs_level_max_rn_lsw),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam16 : public BaseReg
{
public:
    REG_HSC_lvl_qam16(quint32 top = 0) : BaseReg(REG_VSK_lvl_qam16),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam64_low : public BaseReg
{
public:
    REG_HSC_lvl_qam64_low(quint32 top = 0) : BaseReg(REG_VSK_lvl_qam64_low),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam64_midle : public BaseReg
{
public:
    REG_HSC_lvl_qam64_midle(quint32 top = 0) : BaseReg(REG_VSK_lvl_qam64_middle),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam64_high : public BaseReg
{
public:
    REG_HSC_lvl_qam64_high(quint32 top = 0) : BaseReg(REG_VSK_lvl_qam64_high),
        top(top) { }
    quint32 top;
};

class REG_HSC_amplificatoin_factor : public BaseReg
{
public:
    REG_HSC_amplificatoin_factor(quint32 rx_ampl = 0, quint32 rx_oe_ampl = 0) : BaseReg(REG_VSK_amplification_factor),
        rx_ampl(rx_ampl), rx_oe_ampl(rx_oe_ampl) { }
    quint32 rx_ampl : 4;
    quint32 rx_oe_ampl : 1;
};

class REG_HSC_amplitude_signal : public BaseReg
{
public:
    REG_HSC_amplitude_signal(quint32 amplitude = 0) : BaseReg(REG_VSK_amplitude_signal),
        amplitude(amplitude) { }
    quint32 amplitude;
};

class REG_HSC_g_sp : public BaseReg
{
public:
    REG_HSC_g_sp(quint32 data = 0) : BaseReg(REG_VSK_g_sp),
        data(data) { }
    quint32 data;
};

class REG_HSC_g_1_sp_high : public BaseReg
{
public:
    REG_HSC_g_1_sp_high(quint32 data = 0) : BaseReg(REG_VSK_g_1_sp_high),
        data(data) { }
    quint32 data;
};

class REG_HSC_g_1_sp_low : public BaseReg
{
public:
    REG_HSC_g_1_sp_low(quint32 data = 0) : BaseReg(REG_VSK_g_1_sp_low),
        data(data) { }
    quint32 data;
};

class REG_HSC_pll_reg : public BaseReg
{
public:
    REG_HSC_pll_reg(quint32 ns = 0, quint32 ms = 0, quint32 frange = 0, quint32 pd = 0, quint32 test = 0) : BaseReg(REG_VSK_pll_reg),
        ns(ns), ms(ms), frange(frange), pd(pd), test(test) { }
    quint32 ns : 6;
    quint32 ms : 6;
    quint32 frange : 1;
    quint32 pd : 1;
    quint32 test : 1;
};

//------------------------------ AUX --------------------------------------

class REG_AUX_rtaddr : public BaseReg
{
public:
    REG_AUX_rtaddr(quint32 addr = 0) : BaseReg(REG_AUX_T_rtaddr),
        addr(addr) { }
    quint32 addr;
};

class REG_AUX_interruption : public BaseReg
{
public:
    REG_AUX_interruption(quint32 inter = 0) : BaseReg(REG_AUX_T_interruption),
        inter(inter) { }
    quint32 inter;
};

class REG_AUX_bus_select : public BaseReg
{
public:
    REG_AUX_bus_select(quint32 bus = 0) : BaseReg(REG_AUX_T_bus_select),
        bus(bus) { }
    quint32 bus;
};

class REG_AUX_winmode_reset : public BaseReg
{
public:
    REG_AUX_winmode_reset(quint32 mode = 0, quint32 reset = 0, quint32 t_r = 0) : BaseReg(REG_AUX_T_win_mode_reset),
        winmode(mode), reset(reset), t_r(t_r) { }
    quint32 winmode : 1;
    quint32 reset : 1;
    quint32 t_r : 1;
};

class REG_AUX_bulb : public BaseReg
{
public:
    REG_AUX_bulb(quint32 first = 0, quint32 second = 0, quint32 third = 0, quint32 fourth = 0) : BaseReg(REG_AUX_T_bulb),
        first(first), second(second), third(third), fourth(fourth) { }
    quint32 first : 1;
    quint32 second : 1;
    quint32 third : 1;
    quint32 fourth : 1;
};

//------------------------------ LSC --------------------------------------

class REG_LSC_cfg1_BC : public BaseReg
{
public:
    REG_LSC_cfg1_BC(quint32 bc_mip = 0, quint32 bc_fip = 0, quint32 bc_e = 0, quint32 ds_r = 0, quint32 re = 0, quint32 imgte = 0, quint32 ite = 0,
                    quint32 ete = 0, quint32 far_ = 0, quint32 ss_sof = 0, quint32 ss_som = 0, quint32 f_soe = 0, quint32 m_soe = 0, quint32 ca_b_a = 0, quint32 rt_bc_mt = 0) : BaseReg(0x04),
        bc_mip(bc_mip), bc_fip(bc_fip), bc_e(bc_e), ds_r(ds_r), re(re), imgte(imgte), ite(ite),
        ete(ete), far_(far_), ss_sof(ss_sof), ss_som(ss_som), f_soe(f_soe), m_soe(m_soe), ca_b_a(ca_b_a), rt_bc_mt(rt_bc_mt)  { }
    quint32 bc_mip : 1;
    quint32 bc_fip : 1;
    quint32 bc_e : 1;
    quint32 ds_r : 1;
    quint32 re : 1;
    quint32 imgte : 1;
    quint32 ite : 1;
    quint32 ete : 1;
    quint32 far_ : 1;
    quint32 ss_sof : 1;
    quint32 ss_som : 1;
    quint32 f_soe : 1;
    quint32 m_soe : 1;
    quint32 ca_b_a : 1;
    quint32 rt_bc_mt : 2;
};

class REG_LSC_cfg1_RT_st : public BaseReg
{
public:
    REG_LSC_cfg1_RT_st(quint32 rt_mip = 0, quint32 rtf = 0, quint32 sf = 0, quint32 sr = 0, quint32 busy = 0, quint32 dbca = 0, quint32 mme = 0, quint32 ca_b_a = 0, quint32 rt_bc_mt = 2) : BaseReg(0x04),
        rt_mip(rt_mip), rtf(rtf), sf(sf), sr(sr), busy(busy), dbca(dbca), mme(mme), ca_b_a(ca_b_a), rt_bc_mt(rt_bc_mt)  { }
    quint32 rt_mip : 1;
    quint32 : 6;
    quint32 rtf : 1;
    quint32 sf : 1;
    quint32 sr : 1;
    quint32 busy : 1;
    quint32 dbca : 1;
    quint32 mme : 1;
    quint32 ca_b_a : 1;
    quint32 rt_bc_mt : 2;
};

class REG_LSC_cfg1_RT_al : public BaseReg
{
public:
    REG_LSC_cfg1_RT_al(quint32 rt_mip = 0, quint32 s = 0, quint32 mme = 0, quint32 ca_b_a = 0, quint32 rt_bc_mt = 2) : BaseReg(0x04),
        rt_mip(rt_mip), s(s), mme(mme), ca_b_a(ca_b_a), rt_bc_mt(rt_bc_mt)  { }
    quint32 rt_mip : 1;
    quint32 s : 10;
    quint32 mme : 1;
    quint32 ca_b_a : 1;
    quint32 rt_bc_mt : 2;
};

class REG_LSC_cfg1_BM : public BaseReg
{
public:
    REG_LSC_cfg1_BM(quint32 m_a = 0, quint32 m_t = 0, quint32 m_e = 0, quint32 mete = 0, quint32 sp_ot = 0,
                    quint32 st_ot = 0, quint32 tew = 0, quint32 mme = 0, quint32 ca_b_a = 0, quint32 rt_bc_mt = 1) : BaseReg(0x04),
        m_a(m_a), m_t(m_t), m_e(m_e), mete(mete), sp_ot(sp_ot), st_ot(st_ot), tew(tew), mme(mme), ca_b_a(ca_b_a), rt_bc_mt(rt_bc_mt)  { }
    quint32 m_a : 1;
    quint32 m_t : 1;
    quint32 m_e : 1;
    quint32  : 4;
    quint32 mete : 1;
    quint32  : 1;
    quint32 sp_ot : 1;
    quint32 st_ot : 1;
    quint32 tew : 1;
    quint32 mme : 1;
    quint32 ca_b_a : 1;
    quint32 rt_bc_mt : 2;
};

class REG_LSC_srr : public BaseReg
{
public:
    REG_LSC_srr(quint32 reset = 0, quint32 bc_mt_st = 0, quint32 ir = 0, quint32 tt_rst = 0, quint32 tttc = 0, quint32 bc_sof = 0, quint32 bt_som = 0) : BaseReg(0x0C),
        reset(reset), bc_mt_st(bc_mt_st), ir(ir), tt_rst(tt_rst), tttc(tttc), bc_sof(bc_sof), bt_som(bt_som) { }
    quint32 reset : 1;
    quint32 bc_mt_st : 1;
    quint32 ir : 1;
    quint32 tt_rst : 1;
    quint32 tttc : 1;
    quint32 bc_sof : 1;
    quint32 bt_som : 1;
};

class REG_LSC_cfg2_ : public BaseReg
{
public:
    REG_LSC_cfg2_(quint32 sbd = 0, quint32 ermm = 0, quint32 csr = 0, quint32 l_p_ir = 0, quint32 isac = 0, quint32 l_ttos = 0, quint32 c_ttos = 0,
                  quint32 tt_rsl = 0, quint32 wbd_256 = 0, quint32 oid = 0, quint32 rsdbe = 0, quint32 blute = 0, quint32 rp_en = 0, quint32 ei = 0) : BaseReg(0x08),
        sbd(sbd), ermm(ermm), csr(csr), l_p_ir(l_p_ir), isac(isac), l_ttos(l_ttos), c_ttos(c_ttos),
        tt_rsl(tt_rsl), wbd_256(wbd_256), oid(oid), rsdbe(rsdbe), blute(blute), rp_en(rp_en), ei(ei)  { }
    quint32 sbd : 1;
    quint32 ermm : 1;
    quint32 csr : 1;
    quint32 l_p_ir : 1;
    quint32 isac : 1;
    quint32 l_ttos : 1;
    quint32 c_ttos : 1;
    quint32 tt_rsl : 3;
    quint32 wbd_256 : 1;
    quint32 oid : 1;
    quint32 rsdbe : 1;
    quint32 blute : 1;
    quint32 rp_en : 1;
    quint32 ei : 1;
};

class REG_LSC_cfg3_ : public BaseReg
{
public:
    REG_LSC_cfg3_(quint32 emch = 0, quint32 mce_1553a = 0, quint32 rt_f_we = 0, quint32 b_rx_td = 0, quint32 i_rx_td = 0, quint32 aswe = 0, quint32 ome = 0,
                  quint32 id = 0, quint32 mt_dsz = 0, quint32 mt_csz = 0, quint32 bt_csz = 0, quint32 eme = 0) : BaseReg(0x1C),
        emch(emch), mce_1553a(mce_1553a), rt_f_we(rt_f_we), b_rx_td(b_rx_td), i_rx_td(i_rx_td), aswe(aswe), ome(ome),
        id(id), mt_dsz(mt_dsz), mt_csz(mt_csz), bt_csz(bt_csz), eme(eme) { }
    quint32 emch : 1;
    quint32 mce_1553a : 1;
    quint32 rt_f_we : 1;
    quint32 b_rx_td : 1;
    quint32 i_rx_td : 1;
    quint32 aswe : 1;
    quint32 ome : 1;
    quint32 id : 1;
    quint32 mt_dsz : 3;
    quint32 mt_csz : 2;
    quint32 bt_csz : 2;
    quint32 eme : 1;
};

class REG_LSC_cfg4_ : public BaseReg
{
public:
    REG_LSC_cfg4_(quint32 tm = 0, quint32 l_rt_awcr5 = 0, quint32 mtgo = 0, quint32 v_b_nd = 0, quint32 v_me_nd = 0, quint32 asb_2r = 0, quint32 asb_1r = 0,
                  quint32 ri_ss = 0, quint32 ri_aame = 0, quint32 bm_ex = 0, quint32 e_bc_cwe = 0, quint32 mcob = 0, quint32 ibwib = 0, quint32 ebwe = 0) : BaseReg(0x20),
        tm(tm), l_rt_awcr5(l_rt_awcr5), mtgo(mtgo), v_b_nd(v_b_nd), v_me_nd(v_me_nd), asb_2r(asb_2r), asb_1r(asb_1r),
        ri_ss(ri_ss), ri_aame(ri_aame), bm_ex(bm_ex), e_bc_cwe(e_bc_cwe), mcob(mcob), ibwib(ibwib), ebwe(ebwe)  { }
    quint32 tm : 3;
    quint32 l_rt_awcr5 : 1;
    quint32 mtgo : 1;
    quint32 v_b_nd : 1;
    quint32 v_me_nd : 1;
    quint32 asb_2r : 1;
    quint32 asb_1r : 1;
    quint32 ri_ss : 1;
    quint32 ri_aame : 1;
    quint32 bm_ex : 1;
    quint32 e_bc_cwe : 1;
    quint32 mcob : 1;
    quint32 ibwib : 1;
    quint32 ebwe : 1;
};

class REG_LSC_cfg5_ : public BaseReg
{
public:
    REG_LSC_cfg5_(quint32 rt_a_p = 0, quint32 rt_a = 0, quint32 rt_al_t = 0, quint32 bd = 0, quint32 gce = 0, quint32 rts = 0,
                  quint32 ece = 0, quint32 e_tx_ib = 0, quint32 e_tx_ia = 0, quint32 ses = 0, quint32 mhz12 = 0) : BaseReg(0x24),
        rt_a_p(rt_a_p), rt_a(rt_a), rt_al_t(rt_al_t), bd(bd), gce(gce), rts(rts), ece(ece),
        e_tx_ib(e_tx_ib), e_tx_ia(e_tx_ia), ses(ses), mhz12(mhz12)  { }
    quint32 rt_a_p : 1;
    quint32 rt_a : 5;
    quint32 rt_al_t : 1;
    quint32 bd : 1;
    quint32 gce : 1;
    quint32 rts : 2;
    quint32 ece : 1;
    quint32 e_tx_ib : 1;
    quint32 e_tx_ia : 1;
    quint32 ses : 1;
    quint32 mhz12 : 1;
};

#endif // DATA_STRUCTS_H
