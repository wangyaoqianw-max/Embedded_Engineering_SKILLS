# Sanitized CubeMX / Keil fixture

This fixture represents a small project immediately after CubeMX has generated its project inputs. It contains only synthetic application and startup code; it does not represent a real product or vendor component.

Project inputs are Board.ioc and MDK-ARM/Board.uvprojx. The fixture's Keil output and build rules are defined in 00_Doc/Standards/Keil-project-standard.md. That local standard controls this fixture only.

The Debug and Release Targets intentionally use different project-local output paths. Historical logs under logs/ are supplied evidence samples; they are not output from a build of the current fixture.

The Keil groups are defined per Target. Each Target includes the synthetic application and startup sources so a clean build exercises compilation, assembly, and linking.

The sample_port component and its license are synthetic fixture materials. The unverified_port directory is a placeholder with deliberately missing source provenance and license evidence.
