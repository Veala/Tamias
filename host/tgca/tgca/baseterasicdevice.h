#ifndef BASETERASICDEVICE_H
#define BASETERASICDEVICE_H

#include "basedevice.h"
#include "socketdriver.h"
#include "connection.h"
#include "funclib.h"
#include "data_structs.h"

#include <QVector>
#include <QByteArray>

class BaseTerasicDevice : public BaseDevice
{
    Q_OBJECT

signals:
    void error(QAbstractSocket::SocketError err);

    void tcpInit();
    void stopAll();
    void tcpConnect(QString, ushort);
    void tcpDisconnect();
    void tcpExchange();

public:
    explicit BaseTerasicDevice(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    virtual ~BaseTerasicDevice();

    Connection connection;

    bool setSettingsFromFile(QString fileName) override;
    void tryToConnect();
    void tryToDisconnect();

    //-----------------Formats-----------------------------------
    void write_F1(char* writeArray, int size);
    void write_F2(int startAddr, char* writeArray, int size);
    void read_F1(char *writeArray, char *readArray, int wrsize);
    void read_F2(int startAddr, int count, char *readArray);
    void write_Echo(QString& text);
    void cpyOnHard(int fromAddr, int count, int toAddr);
    //-----------------Formats-----------------------------------

    void writeReg(BaseReg* reg);
    void readReg(BaseReg* reg);
    void writeRegs(QVector<BaseReg*>& regs);
    void readRegs(QVector<BaseReg*>& regs);

    REG_HSC_ram_tx_rx reg_hsc_ram_tx_rx; REG_HSC_id reg_hsc_id; REG_HSC_status reg_hsc_status; REG_HSC_cfg reg_hsc_cfg; REG_HSC_tx_cntr reg_hsc_tx_cntr; REG_HSC_rx_cntr reg_hsc_rx_cntr;
    REG_HSC_creg reg_hsc_creg; REG_HSC_cr_spi reg_hsc_cr_spi; REG_HSC_dr_spi_msw reg_hsc_dr_spi_msw; REG_HSC_dr_spi_lsw reg_hsc_dr_spi_lsw; REG_HSC_time_rsp reg_hsc_time_rsp;
    REG_HSC_cnt_pct_tx_msw reg_hsc_cnt_pct_tx_msw; REG_HSC_cnt_pct_tx_lsw reg_hsc_cnt_pct_tx_lsw; REG_HSC_cnt_pct_rx_msw reg_hsc_cnt_pct_rx_msw; REG_HSC_cnt_pct_rx_lsw reg_hsc_cnt_pct_rx_lsw;
    REG_HSC_lvl_sync_kf_rx_msw reg_hsc_lvl_sync_kf_rx_msw; REG_HSC_lvl_sync_kf_rx_lsw reg_hsc_lvl_sync_kf_rx_lsw; REG_HSC_prcs_max_sync_msw reg_hsc_prcs_max_sync_msw; REG_HSC_prcs_max_sync_lsw reg_hsc_prcs_max_sync_lsw;
    REG_HSC_lvl_sync_pre_rx_msw reg_hsc_lvl_sync_pre_rx_msw; REG_HSC_lvl_sync_pre_rx_lsw reg_hsc_lvl_sync_pre_rx_lsw; REG_HSC_prs_level_max_Rn_msw reg_hsc_prs_level_max_Rn_msw; REG_HSC_prs_level_max_Rn_lsw reg_hsc_prs_level_max_Rn_lsw;
    REG_HSC_lvl_qam16 reg_hsc_lvl_qam16; REG_HSC_lvl_qam64_low reg_hsc_lvl_qam64_low; REG_HSC_lvl_qam64_midle reg_hsc_lvl_qam64_midle; REG_HSC_lvl_qam64_high reg_hsc_lvl_qam64_high;
    REG_HSC_amplificatoin_factor reg_hsc_amplificatoin_factor; REG_HSC_amplitude_signal reg_hsc_amplitude_signal; REG_HSC_g_sp reg_hsc_g_sp; REG_HSC_g_1_sp_high reg_hsc_g_1_sp_high; REG_HSC_g_1_sp_low reg_hsc_g_1_sp_low; REG_HSC_pll_reg reg_hsc_pll_reg;

    REG_AUX_rtaddr reg_aux_rtaddr; REG_AUX_interruption reg_aux_interruption; REG_AUX_bus_select reg_aux_bus_select; REG_AUX_winmode_reset reg_aux_winmode_reset; REG_AUX_bulb reg_aux_bulb;

    REG_LSC_cfg1_BC reg_lsc_cfg1_BC; REG_LSC_cfg1_RT_st reg_lsc_cfg1_RT_st; REG_LSC_cfg1_RT_al reg_lsc_cfg1_RT_al; REG_LSC_cfg1_BM reg_lsc_cfg1_BM; REG_LSC_cfg2_ reg_lsc_cfg2_; REG_LSC_cfg3_ reg_lsc_cfg3_; REG_LSC_cfg4_ reg_lsc_cfg4_;
    REG_LSC_cfg5_ reg_lsc_cfg5_; REG_LSC_srr reg_lsc_srr;

protected:
    void showSettings() override;
    void saveSettings() override;

private:
    SocketDriver socketDriver;
    QThread toSocketDriver;

    int BaseRegLen = sizeof(BaseReg);
    int RegLen = 2*sizeof(BaseReg);

private slots:
    void doError(QAbstractSocket::SocketError err);
    void checkDevice();
};

#endif // BASETERASICDEVICE_H
