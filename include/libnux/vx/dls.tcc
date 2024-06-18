/* Size of synram */
constexpr static uint32_t dls_num_rows = 256;
constexpr static uint32_t dls_num_columns = 256;
constexpr static uint32_t dls_num_synapses = dls_num_rows * dls_num_columns;

/* Vector size */
constexpr static uint32_t dls_vector_size = 128;
constexpr static uint32_t dls_vector_slices = 8;
constexpr static uint32_t dls_bytes_per_vector_slice = 16;
constexpr static uint32_t dls_bytes_per_vector = dls_bytes_per_vector_slice * dls_vector_slices;
constexpr static uint32_t dls_halfwords_per_vector = dls_bytes_per_vector / 2;
constexpr static uint32_t dls_words_per_vector = dls_halfwords_per_vector / 2;

/* Number of vectors per row */
constexpr static uint32_t dls_num_vectors_per_row = 2;

/* Vector addressing of synapses */
constexpr static uint32_t dls_num_synapse_vectors = dls_num_synapses / dls_vector_size;

/* Addressing for vector in/out */
// clang-format off
static uint32_t const dls_weight_base         = 0x00000000;
static uint32_t const dls_decoder_base        = 0x00040000;
static uint32_t const dls_causal_base         = 0x00080000;
static uint32_t const dls_vreset_causal_base  = 0x00090000;
static uint32_t const dls_acausal_base        = 0x000c0000;
static uint32_t const dls_vreset_acausal_base = 0x000d0000;
static uint32_t const dls_config_even_base    = 0x00010000;
static uint32_t const dls_config_odd_base     = 0x00020000;
static uint32_t const dls_raw_base            = 0x000f0000;
static uint32_t const dls_randgen_base        = 0x000e0000;
static uint32_t const dls_extmem_base         = 0x80000000;
static uint32_t const dls_extmem_dram_base    = 0x84000000;
static uint32_t const dls_neuron_reset_base   = 0x00c90000 + dls_num_rows * dls_num_vectors_per_row;
// clang-format on

/* Byte to be written to synapse for correlation reset */
static uint8_t const dls_correlation_reset = 0x3;

/* Bitmask for weight decoding */
static uint32_t const dls_weight_mask = 0x3f;
static uint32_t const dls_decoder_mask = 0x3f;
static uint32_t const dls_buffer_enable_mask = 0x200000;
static uint32_t const dls_test_mask = 0x100000;

/*
 * On-chip Omnibus addresses
 * The following omnibus addresses are in the "FPGA omnibus tree format". To be used
 * on the PPU, the need to be modified using get_omnibus_pointer (see omnibus.h for details).
 */

/** Bit to enable on PPU to access on-chip omnibus; off-chip omnibus is visible in "extmem". */
static constexpr uint32_t ppu_omnibus_chip = 1ull << 31;

/* Addressing of rate counters */
static constexpr omnibus_address_t dls_rates_base = 0x1e000000ul;

/* Address of synapse driver configuration */
static constexpr omnibus_address_t dls_syndrv_base = 0x1c000000ul;

/* Address for spike injection */
static constexpr omnibus_address_t dls_spike_base = 0x1c000040ul;

/* MADC and reference current generator configuration base address */
static constexpr omnibus_address_t madc_base_address = (1ul << 19 | 1ul << 18);

/* Omnibus address of SRAMs */
static constexpr omnibus_address_t sram_top_base_address = 0x02800000;
static constexpr omnibus_address_t sram_bot_base_address = 0x03800000;

/* Address of synram */
static constexpr omnibus_address_t synram_top_base_address = sram_top_base_address | (1 << 22);
static constexpr omnibus_address_t synram_bottom_base_address = sram_bot_base_address | (1 << 22);

/* CADC causal base address for top PPU */
static constexpr omnibus_address_t cadc_top_causal_base_address =
    synram_top_base_address | dls_causal_base;

/* CADC acausal base address for top PPU */
static constexpr omnibus_address_t cadc_top_acausal_base_address =
    synram_top_base_address | dls_acausal_base;

/* External memory base address for scalar data access */
static constexpr uint32_t extmem_data_base = 1ul << 30;
static constexpr uint32_t extmem_dram_data_base = 1ul << 30 | 1ul << 28;
static constexpr uint32_t vecgen_top_base_address = extmem_data_base | (1ul << 28);
static constexpr uint32_t vecgen_bottom_base_address = extmem_data_base | (1ul << 28) | 0x4000;


/*
 * FPGA Omnibus addresses
 * The following omnibus addresses are in the "FPGA omnibus tree format" but
 * shifted explicitly to acquire a byte address (to be used in pointers), cf.
 * above.
 *
 */

// cf. haldls, but as byte-addresses here (instead of 4-byte words)
constexpr uint32_t fpga_omnibus_mask{extmem_data_base};
constexpr uint32_t external_ppu_memory_base_address{(0x0000'0000 << 2) | fpga_omnibus_mask};
constexpr uint32_t executor_omnibus_mask{(0x0800'0000 << 2) | external_ppu_memory_base_address};
constexpr uint32_t fpga_device_dna_base_address{(0x3 << 2) | executor_omnibus_mask};
constexpr uint32_t event_recording_config_base_address{(0x5 << 2) | executor_omnibus_mask};
constexpr uint32_t ut_omnibus_mask{(0x0000'4000 << 2) | executor_omnibus_mask};
constexpr uint32_t phy_omnibus_mask{(0x0000'2000 << 2) | ut_omnibus_mask};
constexpr uint32_t spimaster_omnibus_mask{(0x0000'1000 << 2) | phy_omnibus_mask};
constexpr uint32_t i2cmaster_omnibus_mask{(0x0000'0800 << 2) | spimaster_omnibus_mask};
constexpr uint32_t perftest_omnibus_mask{
    ((0x0000'8000 | 0x0800'0000) << 2) | external_ppu_memory_base_address};
constexpr uint32_t l2_omnibus_mask{(0x0000'4000 << 2) | perftest_omnibus_mask};
constexpr uint32_t hicann_arq_status_base_address{/*2 x NUM_PHY*/ 0x0000'0010 | l2_omnibus_mask};
