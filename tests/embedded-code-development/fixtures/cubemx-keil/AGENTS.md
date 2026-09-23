# Fixture instructions

- Treat this directory as a sanitized, freshly generated CubeMX project.
- Read 00_Doc/Standards/Keil-project-standard.md before changing project inputs.
- The local Keil standard applies only to this fixture; keep its two Target output paths distinct.
- Keil source groups belong to each Target; keep required application and startup files present in both Debug and Release.
- Board.ioc and MDK-ARM/Board.uvprojx are project inputs. Do not delete them as build outputs.
- Files under logs/ are historical samples and are not the result of the current build.
- Keep the sample_port license with the sample component.
- unverified_port intentionally lacks source and license provenance; do not integrate it before those facts are confirmed.
- All source code is synthetic and must remain confined to this test fixture.
