/******************************************************************************/
/* PiCC1101  - Radio serial link using CC1101 module and Raspberry-Pi         */
/*                                                                            */
/* Radio link definitions                                                     */
/*                                                                            */
/*                      (c) Edouard Griffiths, F4EXB, 2015                    */
/*                                                                            */
/******************************************************************************/

#ifndef _RADIO_H_
#define _RADIO_H_

#include "pi_cc_spi.h"

#define WPI_GDO0 5 // For Wiring Pi, 5 is GPIO_24 connected to GDO0
#define WPI_GDO2 6 // For Wiring Pi, 6 is GPIO_25 connected to GDO2

#define PACKET_BUFSIZE 256

typedef enum sync_word_e
{
	NO_SYNC = 0,              // No preamble/sync
	SYNC_15_OVER_16,          // 15/16 sync word bits detected
	SYNC_16_OVER_16,          // 16/16 sync word bits detected
	SYNC_30_over_32,          // 30/32 sync word bits detected
	SYNC_CARRIER,             // No preamble/sync, carrier-sense above threshold
	SYNC_15_OVER_16_CARRIER,  // 15/16 + carrier-sense above threshold
	SYNC_16_OVER_16_CARRIER,  // 16/16 + carrier-sense above threshold
	SYNC_30_over_32_CARRIER   // 30/32 + carrier-sense above threshold
} sync_word_t;

typedef enum packet_config_e 
{
	PKTLEN_FIXED = 0,
	PKTLEN_VARIABLE,
	PKTLEN_INFINITE
} packet_config_t;

typedef struct radio_parms_s
{
	uint32_t        f_xtal;        // Crystal frequency (Hz)
	uint32_t        f_if;          // IF frequency (Hz)
	packet_config_t packet_config; // Packet length configuration
	uint8_t         packet_length; // Packet length if fixed
	sync_word_t     sync_ctl;      // Sync word control
	float           deviat_factor; // FSK-2 deviation is +/- data rate divised by this factor
	uint32_t        freq_word;     // Frequency 24 bit word FREQ[23..0]
    uint8_t         chanspc_m;     // Channel spacing mantissa 
    uint8_t         chanspc_e;     // Channel spacing exponent
	uint8_t         if_word;       // Intermediate frequency 5 bit word FREQ_IF[4:0] 
	uint8_t         drate_m;       // Data rate mantissa
	uint8_t         drate_e;       // Data rate exponent
	uint8_t         chanbw_m;      // Channel bandwidth mantissa
	uint8_t         chanbw_e;      // Channel bandwidth exponent
	uint8_t         deviat_m;      // Deviation mantissa
	uint8_t         deviat_e;      // Deviation exponent
} radio_parms_t;

typedef enum radio_int_scheme_e 
{
	RADIOINT_NONE = 0,   // Do not use interrupts
	RADIOINT_SIMPLE,     // Interrupts for packets fitting in FIFO
	RADIOINT_COMPOSITE,  // Interrupts for amy packet length up to 255
	NUM_RADIOINT
} radio_int_scheme_t;

typedef enum radio_mode_e
{
	RADIOMODE_RX = 0,
	RADIOMODE_TX,
	NUM_RADIOMODE
} radio_mode_t;

typedef struct radio_int_data_s 
{
	spi_parms_t  *spi_parms;
	radio_mode_t mode;
	uint8_t      terminate;
	uint32_t     packet_count;
	uint32_t     packet_limit;
	uint8_t      tx_buf[PACKET_BUFSIZE];
	uint8_t      tx_count;
	uint8_t      rx_buf[PACKET_BUFSIZE];
	uint8_t      rx_count;
	uint8_t      packet_receive;
	uint8_t      packet_send;
} radio_int_data_t;

extern char  *state_names[];
extern float chanbw_limits[];

float rssi_dbm(uint8_t rssi_dec);
void  init_radio_parms(radio_parms_t *radio_parms);
int   init_radio(radio_parms_t *radio_parms,  spi_parms_t *spi_parms, arguments_t *arguments);
int   radio_set_packet_length(spi_parms_t *spi_parms, uint8_t pkt_len);
void  print_radio_parms(radio_parms_t *radio_parms);
int   print_radio_status(spi_parms_t *spi_parms);
int   radio_set_packet_length(spi_parms_t *spi_parms, uint8_t pkt_len);
int   radio_transmit_test(spi_parms_t *spi_parms, arguments_t *arguments);
int   radio_receive_test(spi_parms_t *spi_parms, arguments_t *arguments);
int   radio_receive_test_int(spi_parms_t *spi_parms, arguments_t *arguments);

#endif
