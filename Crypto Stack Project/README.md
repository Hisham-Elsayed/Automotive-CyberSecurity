# TM4C123 AUTOSAR Crypto Stack Project

Layered AUTOSAR-style crypto stack for the Texas Instruments TM4C123GH6PM:

```text
CSM -> CryIf -> Crypto Driver -> mbedTLS
```

The current validation application exercises:

- `Csm_Hash`: SHA-256 known-answer test
- `Csm_Encrypt`: AES-128-ECB known-answer test using the FIPS-197 vector

## Current Folder Layout

```text
Csm_Types.h                 Shared AUTOSAR-style types and constants
main.c                      TM4C123 UART test harness and known-answer tests

CSM/
  Csm.c                     CSM service implementation
  Csm.h                     CSM API
CryIf/
  CryIf.c                   CryIf routing implementation
  CryIf.h                   CryIf API
Crypto Driver/
  Crypto.c                  Software Crypto driver implementation
  Crypto.h                  Crypto driver API

mbedtls/
  include/mbedtls/          Trimmed mbedTLS headers and config.h
  library/
    aes.c                   AES implementation
    platform_util.c         Minimal platform zeroization implementation
    sha256.c                SHA-256 implementation

RTE/                        Keil-generated runtime/device support files
  _Target_1/
    RTE_Components.h
  Device/
    TM4C123GH6PM/
      startup_TM4C123.s
      system_TM4C123.c

Project.uvprojx             Keil uVision project file
Project.uvoptx              Keil debugger/options file
Project.uvguix.hisha        Keil user interface layout
Listings/                   Keil listing output
Objects/                    Keil build output
Scs/                        Project support directory
EventRecorderStub.scvd      Event Recorder stub description
```

`Objects/`, `Listings/`, and the files under `RTE/` are generated or managed by
Keil. The source files used by the target are listed in `Project.uvprojx` under
the `mbedtls` and `Source Group` groups.

## Implemented Services

- `Csm_Hash` -> `CryIf_ProcessJob` -> `Crypto_ProcessJob` -> mbedTLS SHA-256
- `Csm_SetKey` and `Csm_SetKeyValid` -> software Crypto driver key management
- `Csm_Encrypt` -> `CryIf_ProcessJob` -> `Crypto_ProcessJob` -> mbedTLS AES-ECB

The current Crypto driver supports SHA-256 and one-block AES-ECB encryption.
AES keys up to 32 bytes can be stored by the driver, while the validation
application uses a 16-byte AES-128 key.

## mbedTLS Configuration

This repository contains a trimmed mbedTLS implementation. The active config is:

```text
mbedtls/include/mbedtls/config.h
```

Only these primitives are enabled:

- `MBEDTLS_SHA256_C`
- `MBEDTLS_AES_C`
- `MBEDTLS_AES_ROM_TABLES`

The local `mbedtls/library/platform_util.c` provides only the platform
zeroization function required by the included AES and SHA-256 sources.

To add another mbedTLS primitive, enable its `MBEDTLS_xxx_C` option in
`mbedtls/include/mbedtls/config.h` and add the corresponding source and header
files from a matching mbedTLS checkout.

## Building in Keil MDK

1. Open `Project.uvprojx` in Keil uVision.
2. Confirm that the selected device is `TM4C123GH6PM` and that the
   `Keil.TM4C_DFP` device pack is installed.
3. The project already includes these source files:

   ```text
   .\CSM\Csm.c
   .\CryIf\CryIf.c
   .\Crypto Driver\Crypto.c
   .\main.c
   .\mbedtls\library\aes.c
   .\mbedtls\library\platform_util.c
   .\mbedtls\library\sha256.c
   ```

4. The project include paths are configured as:

   ```text
   .\CryIf
   .\Crypto Driver
   .\CSM
   .\mbedtls\include
   ```

   The project root is also included so `Csm_Types.h` can be found.
5. Build the target. The expected result is zero compilation errors.

If the project is recreated manually, add the same source files and include
paths through **Project -> Options for Target -> C/C++**.

## UART Test Output

`main.c` initializes UART0 on PA0 and PA1 and sends output using 115200 8N1.
Connect a serial terminal to the LaunchPad ICDI COM port with those settings.
The test harness prints results similar to:

```text
TM4C123 AUTOSAR Crypto Stack Test
----------------------------------

Csm_Hash SHA-256 test:
  Computed: 80C25EC1600587E7F28B18B1B18E3CDC89928E39CAB3BC25E4D4A4C139BCEDC4
  Expected: 80C25EC1600587E7F28B18B1B18E3CDC89928E39CAB3BC25E4D4A4C139BCEDC4
  Result:   PASS

Csm_Encrypt AES-ECB test:
  Computed: 3925841D02DC09FBDC118597196A0B32
  Expected: 3925841D02DC09FBDC118597196A0B32
  Result:   PASS
```
