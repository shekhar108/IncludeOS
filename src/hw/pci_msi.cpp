#include <hw/pci_device.hpp>
#include <hw/pci.hpp>
#include <debug>

#define PCI_CMD_REG			0x04

// MSI and MSI-X capability registers
#define PCI_CAP_ID_MSI        0x05    /* Message Signalled Interrupts */
#define PCI_CAP_ID_MSIX       0x11    /* MSI-X */

// Message Signalled Interrupts registers
#define PCI_MSI_FLAGS_ENABLE  0x0001  /* MSI feature enabled */
#define PCI_MSI_FLAGS_QMASK   0x000e  /* Maximum queue size available */
#define PCI_MSI_FLAGS_QSIZE   0x0070  /* Message queue size configured */
#define PCI_MSI_FLAGS_64BIT   0x0080  /* 64-bit addresses allowed */
#define PCI_MSI_FLAGS_MASKBIT 0x0100  /* Per-vector masking capable */

#define PCI_MSI_FLAGS              2  /* Message Control */
#define PCI_MSI_RFU                3  /* Rest of capability flags */
#define PCI_MSI_ADDRESS_LO         4  /* Lower 32 bits */
#define PCI_MSI_ADDRESS_HI         8  /* Upper 32 bits (if PCI_MSI_FLAGS_64BIT set) */
#define PCI_MSI_DATA_32            8  /* 16 bits of data for 32-bit devices */
#define PCI_MSI_MASK_32           12  /* Mask bits register for 32-bit devices */
#define PCI_MSI_PENDING_32        16  /* Pending intrs for 32-bit devices */
#define PCI_MSI_DATA_64           12  /* 16 bits of data for 64-bit devices */
#define PCI_MSI_MASK_64           16  /* Mask bits register for 64-bit devices */
#define PCI_MSI_PENDING_64        20  /* Pending intrs for 64-bit devices */

namespace hw
{
  int PCI_Device::msi_cap()
  {
    return caps[PCI_CAP_ID_MSI];
  }
  
  int PCI_Device::msix_cap()
  {
    return caps[PCI_CAP_ID_MSIX];
  }
  
  uint8_t PCI_Device::init_msix()
  {
    // disable intx
    auto cmd = read16(PCI_CMD_REG);
    write16(PCI_CMD_REG, cmd | (1 << 10));
    // enable MSI-X
    this->msix = new msix_t(*this);
    return msix->vectors();
  }
  void PCI_Device::setup_msix_vector(uint8_t cpu, uint8_t irq)
  {
    msix->setup_vector(cpu, irq);
  }
  
}
